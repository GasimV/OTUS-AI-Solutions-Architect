from __future__ import annotations

import json
from collections.abc import Iterator

from fastapi import FastAPI
from fastapi.responses import StreamingResponse
from pydantic import BaseModel, Field

from .adapters import build_stores_from_env
from .agent import SecureGraphRagAgent
from .llm import build_llm_from_env
from .schemas import DocumentInput, QueryRequest, UserContext


class UserPayload(BaseModel):
    user_id: str
    tenant_id: str = "bank"
    groups: list[str] = Field(default_factory=list)


class IngestPayload(BaseModel):
    document_id: str
    tenant_id: str = "bank"
    title: str
    text: str
    acl_groups: list[str]
    metadata: dict[str, object] = Field(default_factory=dict)


class QueryPayload(BaseModel):
    query: str
    user: UserPayload
    top_k: int = 5


vector_store, graph_store = build_stores_from_env()
agent = SecureGraphRagAgent(vector_store=vector_store, graph_store=graph_store, llm=build_llm_from_env())
app = FastAPI(title="Secure GraphRAG Platform", version="0.1.0")

try:
    from prometheus_fastapi_instrumentator import Instrumentator

    Instrumentator().instrument(app).expose(app, endpoint="/metrics")
except Exception:
    pass


@app.get("/")
def root() -> dict[str, object]:
    return {
        "service": "Secure GraphRAG Platform",
        "status": "ok",
        "docs": "/docs",
        "openapi": "/openapi.json",
        "health": "/health",
        "endpoints": ["/ingest", "/query", "/query/stream", "/metrics"],
    }


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok"}


@app.post("/ingest")
def ingest(payload: IngestPayload) -> dict[str, object]:
    result = agent.ingest(
        DocumentInput(
            document_id=payload.document_id,
            tenant_id=payload.tenant_id,
            title=payload.title,
            text=payload.text,
            acl_groups=tuple(payload.acl_groups),
            metadata=payload.metadata,
        )
    )
    return {
        "document_id": result.document_id,
        "chunk_ids": list(result.chunk_ids),
        "entity_count": result.entity_count,
    }


@app.post("/query")
def query(payload: QueryPayload) -> dict[str, object]:
    response = agent.answer(_to_request(payload))
    return {
        "answer": response.answer,
        "citations": list(response.citations),
        "blocked": response.blocked,
        "guardrail_events": list(response.guardrail_events),
    }


@app.post("/query/stream")
def query_stream(payload: QueryPayload) -> StreamingResponse:
    def events() -> Iterator[str]:
        for item in agent.stream_answer(_to_request(payload)):
            event = str(item["event"])
            data = item["data"]
            if event == "citations" and isinstance(data, list):
                stream_payload = ",".join(str(value) for value in data)
            elif isinstance(data, (dict, list, bool)):
                stream_payload = json.dumps(data, ensure_ascii=False)
            else:
                stream_payload = str(data)
            yield _sse(stream_payload, event=None if event == "data" else event)

    return StreamingResponse(events(), media_type="text/event-stream")


def _to_request(payload: QueryPayload) -> QueryRequest:
    return QueryRequest(
        query=payload.query,
        user=UserContext(
            user_id=payload.user.user_id,
            tenant_id=payload.user.tenant_id,
            groups=tuple(payload.user.groups),
        ),
        top_k=payload.top_k,
    )


def _sse(data: str, event: str | None = None) -> str:
    lines = []
    if event:
        lines.append(f"event: {event}")
    for line in data.splitlines() or [""]:
        lines.append(f"data: {line}")
    return "\n".join(lines) + "\n\n"
