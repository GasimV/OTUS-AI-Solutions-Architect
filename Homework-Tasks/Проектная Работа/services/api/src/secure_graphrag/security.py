from __future__ import annotations

import re
from dataclasses import dataclass

from .schemas import Chunk, UserContext


PROMPT_INJECTION_PATTERNS = (
    re.compile(r"\bignore\s+(all\s+)?previous\s+instructions\b", re.I),
    re.compile(r"\bdeveloper\s+message\b", re.I),
    re.compile(r"\bsystem\s+prompt\b", re.I),
    re.compile(r"\bdisable\s+(security|guardrails|filters)\b", re.I),
    re.compile(r"\bрасскажи\s+системн(ый|ые)\s+промпт", re.I),
    re.compile(r"\bигнорируй\s+(все\s+)?предыдущ(ие|ую)\s+инструкц", re.I),
)

PII_PATTERNS = (
    (re.compile(r"\b\d{4}\s?\d{6}\b"), "[PASSPORT_REDACTED]"),
    (re.compile(r"\b\d{3}-\d{3}-\d{3}\s?\d{2}\b"), "[SNILS_REDACTED]"),
    (re.compile(r"\b(?:\d[ -]*?){13,19}\b"), "[CARD_REDACTED]"),
    (re.compile(r"\b[\w.+-]+@[\w-]+\.[\w.-]+\b"), "[EMAIL_REDACTED]"),
)

THINK_BLOCK_PATTERN = re.compile(r"<think>.*?</think>", re.I | re.S)


@dataclass(frozen=True)
class GuardrailResult:
    text: str
    blocked: bool
    events: tuple[str, ...]


class Guardrails:
    def inspect_input(self, text: str) -> GuardrailResult:
        events: list[str] = []
        if any(pattern.search(text) for pattern in PROMPT_INJECTION_PATTERNS):
            events.append("prompt_injection_detected")
            return GuardrailResult(text="", blocked=True, events=tuple(events))
        sanitized = self.redact_pii(text)
        if sanitized != text:
            events.append("input_pii_redacted")
        return GuardrailResult(text=sanitized, blocked=False, events=tuple(events))

    def inspect_output(self, text: str, allowed_chunks: set[str], citations: list[str]) -> GuardrailResult:
        events: list[str] = []
        allowed_citations = [citation for citation in citations if citation in allowed_chunks]
        if len(allowed_citations) != len(citations):
            events.append("inaccessible_citation_removed")
        sanitized = THINK_BLOCK_PATTERN.sub("", text).strip()
        if sanitized != text:
            events.append("reasoning_trace_removed")
        redacted = self.redact_pii(sanitized)
        if redacted != sanitized:
            events.append("output_pii_redacted")
        sanitized = redacted
        return GuardrailResult(text=sanitized, blocked=False, events=tuple(events))

    def can_read_chunk(self, user: UserContext, chunk: Chunk) -> bool:
        return chunk.tenant_id == user.tenant_id and bool(set(user.groups).intersection(chunk.acl_groups))

    def redact_pii(self, text: str) -> str:
        sanitized = text
        for pattern, replacement in PII_PATTERNS:
            sanitized = pattern.sub(replacement, sanitized)
        return sanitized
