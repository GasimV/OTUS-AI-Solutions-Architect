from __future__ import annotations

import json
import os
import re
import urllib.request
from collections.abc import Sequence
from dataclasses import dataclass

from .schemas import RetrievedChunk


FACT_PATTERN = re.compile(r"\b(?:\d{3,}|[A-Z]{2,}-\d{2,})\b")


@dataclass(frozen=True)
class JudgeResult:
    passed: bool
    score: float
    reasons: tuple[str, ...]


class HeuristicAnswerJudge:
    """Offline quality gate for CI; production can replace it with LocalLlmJudge."""

    def evaluate(self, query: str, answer: str, chunks: Sequence[RetrievedChunk]) -> JudgeResult:
        reasons: list[str] = []
        if not answer.strip():
            reasons.append("empty_answer")
        context = "\n".join(item.chunk.text for item in chunks)
        for fact in FACT_PATTERN.findall(answer):
            if fact not in context:
                reasons.append(f"unsupported_fact:{fact}")
        if chunks and not any(item.chunk.id in answer for item in chunks):
            reasons.append("missing_chunk_citation")
        score = 1.0 if not reasons else max(0.0, 1.0 - 0.25 * len(reasons))
        return JudgeResult(passed=score >= 0.75 and not reasons, score=score, reasons=tuple(reasons))


class LocalLlmJudge:
    def __init__(self, base_url: str, model: str, timeout_seconds: int = 60) -> None:
        self.base_url = base_url.rstrip("/")
        self.model = model
        self.timeout_seconds = timeout_seconds

    def evaluate(self, query: str, answer: str, chunks: Sequence[RetrievedChunk]) -> JudgeResult:
        context = "\n\n".join(f"[{item.chunk.id}] {item.chunk.text}" for item in chunks)
        payload = {
            "model": self.model,
            "messages": [
                {
                    "role": "system",
                    "content": (
                        "You are an independent answer quality judge. Return compact JSON only with keys "
                        "passed, score, reasons. Check grounding in context, relevance, and hallucinations."
                    ),
                },
                {
                    "role": "user",
                    "content": f"Question:\n{query}\n\nAnswer:\n{answer}\n\nContext:\n{context}",
                },
            ],
            "temperature": 0,
            "max_tokens": 180,
        }
        request = urllib.request.Request(
            f"{self.base_url}/chat/completions",
            data=json.dumps(payload, ensure_ascii=False).encode("utf-8"),
            headers={"Content-Type": "application/json"},
            method="POST",
        )
        with urllib.request.urlopen(request, timeout=self.timeout_seconds) as response:
            body = json.loads(response.read().decode("utf-8"))
        content = body["choices"][0]["message"]["content"]
        return _parse_judge_json(content)


def build_judge_from_env() -> HeuristicAnswerJudge | LocalLlmJudge:
    if os.getenv("JUDGE_MODE", "heuristic").lower() == "llm":
        return LocalLlmJudge(
            base_url=os.getenv("JUDGE_LLM_BASE_URL", os.getenv("LLM_BASE_URL", "http://vllm:8000/v1")),
            model=os.getenv("JUDGE_LLM_MODEL", os.getenv("LLM_MODEL", "local-qwen")),
            timeout_seconds=int(os.getenv("JUDGE_TIMEOUT_SECONDS", "60")),
        )
    return HeuristicAnswerJudge()


def _parse_judge_json(content: str) -> JudgeResult:
    start = content.find("{")
    end = content.rfind("}")
    if start == -1 or end == -1 or end <= start:
        return JudgeResult(passed=False, score=0.0, reasons=("invalid_judge_json",))
    data = json.loads(content[start : end + 1])
    reasons = tuple(str(reason) for reason in data.get("reasons", []))
    return JudgeResult(passed=bool(data.get("passed")), score=float(data.get("score", 0.0)), reasons=reasons)
