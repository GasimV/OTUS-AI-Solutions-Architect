from __future__ import annotations

from collections.abc import Iterator
from dataclasses import dataclass
from typing import Any

from .chunking import chunk_document
from .llm import ExtractiveLlmFallback, is_llm_transport_error
from .schemas import DocumentInput, IngestionResult, QueryRequest, QueryResponse, RetrievedChunk
from .security import Guardrails
from .stores import GraphStore, VectorStore


@dataclass
class AgentState:
    request: QueryRequest
    sanitized_query: str = ""
    retrieved: list[RetrievedChunk] | None = None
    answer: str = ""
    citations: list[str] | None = None
    events: list[str] | None = None
    blocked: bool = False


class SecureGraphRagAgent:
    def __init__(
        self,
        vector_store: VectorStore,
        graph_store: GraphStore,
        llm: Any | None = None,
        guardrails: Guardrails | None = None,
    ) -> None:
        self.vector_store = vector_store
        self.graph_store = graph_store
        self.llm = llm or ExtractiveLlmFallback()
        self.guardrails = guardrails or Guardrails()
        self._graph = self._build_langgraph()

    def ingest(self, document: DocumentInput) -> IngestionResult:
        chunks = chunk_document(document)
        self.vector_store.upsert_chunks(chunks)
        self.graph_store.upsert_chunks(chunks)
        entities = {entity for chunk in chunks for entity in chunk.entities}
        return IngestionResult(
            document_id=document.document_id,
            chunk_ids=tuple(chunk.id for chunk in chunks),
            entity_count=len(entities),
        )

    def answer(self, request: QueryRequest) -> QueryResponse:
        initial = AgentState(request=request, events=[], citations=[])
        if self._graph is None:
            final = self._run_without_langgraph(initial)
        else:
            final_dict = self._graph.invoke(initial.__dict__)
            final = AgentState(**final_dict)
        return QueryResponse(
            answer=final.answer,
            citations=tuple(final.citations or []),
            blocked=final.blocked,
            guardrail_events=tuple(final.events or []),
        )

    def stream_answer(self, request: QueryRequest) -> Iterator[dict[str, object]]:
        state = self._prepare_generation_state(request)
        if state.blocked:
            yield {"event": "data", "data": state.answer}
            yield {"event": "citations", "data": []}
            yield {"event": "guardrail_events", "data": state.events or []}
            return

        retrieved = list(state.retrieved or [])
        citations = [item.chunk.id for item in retrieved]
        if not retrieved:
            answer = "No accessible knowledge chunks are available for this query."
            yield {"event": "data", "data": answer}
            yield {"event": "citations", "data": []}
            yield {"event": "guardrail_events", "data": state.events or []}
            return

        answer_parts: list[str] = []
        events = list(state.events or [])
        try:
            generator = getattr(self.llm, "stream_generate")
            for token in generator(state.sanitized_query, retrieved):
                answer_parts.append(token)
                safe_token = self.guardrails.redact_pii(token)
                if safe_token:
                    yield {"event": "data", "data": safe_token}
        except Exception as error:
            if not is_llm_transport_error(error):
                raise
            events.append("llm_transport_fallback")
            fallback = ExtractiveLlmFallback().generate(state.sanitized_query, retrieved)
            answer_parts = [fallback]
            yield {"event": "data", "data": fallback}

        allowed = {item.chunk.id for item in retrieved}
        final = self.guardrails.inspect_output("".join(answer_parts), allowed, citations)
        events.extend(final.events)
        yield {"event": "final", "data": final.text}
        yield {"event": "citations", "data": [citation for citation in citations if citation in allowed]}
        yield {"event": "guardrail_events", "data": events}

    def _build_langgraph(self) -> Any | None:
        try:
            from langgraph.graph import END, StateGraph
        except Exception:
            return None

        graph = StateGraph(dict)
        graph.add_node("guard_input", self._guard_input_node)
        graph.add_node("retrieve", self._retrieve_node)
        graph.add_node("graph_expand", self._graph_expand_node)
        graph.add_node("generate", self._generate_node)
        graph.add_node("guard_output", self._guard_output_node)
        graph.set_entry_point("guard_input")
        graph.add_conditional_edges(
            "guard_input",
            lambda state: "blocked" if state.get("blocked") else "retrieve",
            {"blocked": END, "retrieve": "retrieve"},
        )
        graph.add_edge("retrieve", "graph_expand")
        graph.add_edge("graph_expand", "generate")
        graph.add_edge("generate", "guard_output")
        graph.add_edge("guard_output", END)
        return graph.compile()

    def _run_without_langgraph(self, state: AgentState) -> AgentState:
        data = self._guard_input_node(state.__dict__)
        if data.get("blocked"):
            return AgentState(**data)
        for node in (self._retrieve_node, self._graph_expand_node, self._generate_node, self._guard_output_node):
            data = node(data)
        return AgentState(**data)

    def _prepare_generation_state(self, request: QueryRequest) -> AgentState:
        data: dict[str, Any] = AgentState(request=request, events=[], citations=[]).__dict__
        data = self._guard_input_node(data)
        if data.get("blocked"):
            return AgentState(**data)
        data = self._retrieve_node(data)
        data = self._graph_expand_node(data)
        return AgentState(**data)

    def _guard_input_node(self, state: dict[str, Any]) -> dict[str, Any]:
        request: QueryRequest = state["request"]
        result = self.guardrails.inspect_input(request.query)
        events = list(state.get("events") or []) + list(result.events)
        if result.blocked:
            return {
                **state,
                "blocked": True,
                "answer": "Запрос заблокирован политикой безопасности.",
                "events": events,
                "citations": [],
            }
        return {**state, "sanitized_query": result.text, "events": events}

    def _retrieve_node(self, state: dict[str, Any]) -> dict[str, Any]:
        request: QueryRequest = state["request"]
        retrieved = self.vector_store.search(state["sanitized_query"], request.user, request.top_k)
        return {**state, "retrieved": retrieved}

    def _graph_expand_node(self, state: dict[str, Any]) -> dict[str, Any]:
        request: QueryRequest = state["request"]
        retrieved: list[RetrievedChunk] = list(state.get("retrieved") or [])
        seed_ids = [item.chunk.id for item in retrieved]
        expanded = self.graph_store.expand(seed_ids, request.user, limit=max(2, request.top_k))
        merged: dict[str, RetrievedChunk] = {item.chunk.id: item for item in retrieved}
        for item in expanded:
            existing = merged.get(item.chunk.id)
            if existing is None or item.score > existing.score:
                merged[item.chunk.id] = item
        ranked = sorted(merged.values(), key=lambda item: item.score, reverse=True)[: request.top_k + 2]
        return {**state, "retrieved": ranked}

    def _generate_node(self, state: dict[str, Any]) -> dict[str, Any]:
        retrieved: list[RetrievedChunk] = list(state.get("retrieved") or [])
        if not retrieved:
            return {**state, "answer": "Нет доступных фрагментов знаний для ответа.", "citations": []}
        try:
            answer = self.llm.generate(state["sanitized_query"], retrieved)
        except Exception as error:
            if not is_llm_transport_error(error):
                raise
            events = list(state.get("events") or []) + ["llm_transport_fallback"]
            answer = ExtractiveLlmFallback().generate(state["sanitized_query"], retrieved)
            return {**state, "answer": answer, "citations": [item.chunk.id for item in retrieved], "events": events}
        return {**state, "answer": answer, "citations": [item.chunk.id for item in retrieved]}

    def _guard_output_node(self, state: dict[str, Any]) -> dict[str, Any]:
        retrieved: list[RetrievedChunk] = list(state.get("retrieved") or [])
        allowed = {item.chunk.id for item in retrieved}
        result = self.guardrails.inspect_output(state["answer"], allowed, list(state.get("citations") or []))
        return {
            **state,
            "answer": result.text,
            "citations": [citation for citation in (state.get("citations") or []) if citation in allowed],
            "events": list(state.get("events") or []) + list(result.events),
        }
