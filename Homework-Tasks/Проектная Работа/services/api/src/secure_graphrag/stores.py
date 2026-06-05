from __future__ import annotations

import math
from collections import defaultdict
from collections.abc import Iterable
from typing import Protocol

from .embeddings import EmbeddingModel, HashEmbeddingModel
from .schemas import Chunk, RetrievedChunk, UserContext


class VectorStore(Protocol):
    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None: ...

    def search(self, query: str, user: UserContext, top_k: int) -> list[RetrievedChunk]: ...


class GraphStore(Protocol):
    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None: ...

    def expand(self, seed_chunk_ids: Iterable[str], user: UserContext, limit: int) -> list[RetrievedChunk]: ...


class InMemoryVectorStore:
    def __init__(self, embeddings: EmbeddingModel | None = None) -> None:
        self.embeddings = embeddings or HashEmbeddingModel()
        self._vectors: dict[str, list[float]] = {}
        self._chunks: dict[str, Chunk] = {}

    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None:
        for chunk in chunks:
            self._chunks[chunk.id] = chunk
            self._vectors[chunk.id] = self.embeddings.embed(chunk.text)

    def search(self, query: str, user: UserContext, top_k: int) -> list[RetrievedChunk]:
        query_vector = self.embeddings.embed(query)
        results: list[RetrievedChunk] = []
        user_groups = set(user.groups)
        for chunk_id, vector in self._vectors.items():
            chunk = self._chunks[chunk_id]
            if chunk.tenant_id != user.tenant_id or not user_groups.intersection(chunk.acl_groups):
                continue
            score = cosine(query_vector, vector)
            results.append(RetrievedChunk(chunk=chunk, score=score, source="vector"))
        return sorted(results, key=lambda item: item.score, reverse=True)[:top_k]


class InMemoryGraphStore:
    def __init__(self) -> None:
        self._chunks: dict[str, Chunk] = {}
        self._entity_to_chunks: dict[str, set[str]] = defaultdict(set)

    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None:
        for chunk in chunks:
            self._chunks[chunk.id] = chunk
            for entity in chunk.entities:
                self._entity_to_chunks[entity.lower()].add(chunk.id)

    def expand(self, seed_chunk_ids: Iterable[str], user: UserContext, limit: int) -> list[RetrievedChunk]:
        seed_ids = set(seed_chunk_ids)
        user_groups = set(user.groups)
        scores: dict[str, float] = defaultdict(float)
        for seed_id in seed_ids:
            seed = self._chunks.get(seed_id)
            if seed is None:
                continue
            for entity in seed.entities:
                for neighbor_id in self._entity_to_chunks[entity.lower()]:
                    if neighbor_id in seed_ids:
                        continue
                    neighbor = self._chunks[neighbor_id]
                    if neighbor.tenant_id == user.tenant_id and user_groups.intersection(neighbor.acl_groups):
                        scores[neighbor_id] += 0.15
        results = [
            RetrievedChunk(chunk=self._chunks[chunk_id], score=score, source="graph")
            for chunk_id, score in scores.items()
        ]
        return sorted(results, key=lambda item: item.score, reverse=True)[:limit]


def cosine(left: list[float], right: list[float]) -> float:
    denominator = (math.sqrt(sum(v * v for v in left)) * math.sqrt(sum(v * v for v in right))) or 1.0
    return sum(a * b for a, b in zip(left, right, strict=False)) / denominator
