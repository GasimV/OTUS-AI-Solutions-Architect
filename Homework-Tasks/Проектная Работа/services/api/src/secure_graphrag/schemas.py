from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any


@dataclass(frozen=True)
class UserContext:
    user_id: str
    tenant_id: str
    groups: tuple[str, ...]


@dataclass(frozen=True)
class Chunk:
    id: str
    document_id: str
    tenant_id: str
    text: str
    acl_groups: tuple[str, ...]
    ordinal: int
    metadata: dict[str, Any] = field(default_factory=dict)
    entities: tuple[str, ...] = ()


@dataclass(frozen=True)
class RetrievedChunk:
    chunk: Chunk
    score: float
    source: str


@dataclass(frozen=True)
class QueryRequest:
    query: str
    user: UserContext
    top_k: int = 5


@dataclass(frozen=True)
class QueryResponse:
    answer: str
    citations: tuple[str, ...]
    blocked: bool = False
    guardrail_events: tuple[str, ...] = ()


@dataclass(frozen=True)
class DocumentInput:
    document_id: str
    tenant_id: str
    title: str
    text: str
    acl_groups: tuple[str, ...]
    metadata: dict[str, Any] = field(default_factory=dict)


@dataclass(frozen=True)
class IngestionResult:
    document_id: str
    chunk_ids: tuple[str, ...]
    entity_count: int
