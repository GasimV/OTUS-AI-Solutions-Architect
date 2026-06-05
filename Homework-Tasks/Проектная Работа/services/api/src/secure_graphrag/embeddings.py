from __future__ import annotations

import hashlib
import math
import os
import re
from collections.abc import Sequence
from typing import Protocol


TOKEN_PATTERN = re.compile(r"[\w\u0430-\u044f\u0410-\u042f\u0451\u0401-]+", re.UNICODE)


class EmbeddingModel(Protocol):
    dimensions: int

    def embed(self, text: str) -> list[float]: ...

    def embed_many(self, texts: Sequence[str]) -> list[list[float]]: ...


class HashEmbeddingModel:
    """Deterministic local embedding fallback for tests and air-gapped smoke runs."""

    def __init__(self, dimensions: int = 384) -> None:
        self.dimensions = dimensions

    def embed(self, text: str) -> list[float]:
        vector = [0.0] * self.dimensions
        for token in TOKEN_PATTERN.findall(text.lower()):
            digest = hashlib.blake2b(token.encode("utf-8"), digest_size=8).digest()
            idx = int.from_bytes(digest[:4], "big") % self.dimensions
            sign = 1.0 if digest[4] % 2 == 0 else -1.0
            vector[idx] += sign
        return _normalize(vector)

    def embed_many(self, texts: Sequence[str]) -> list[list[float]]:
        return [self.embed(text) for text in texts]


class SentenceTransformerEmbeddingModel:
    """Local semantic embedding model for production-like deployments."""

    def __init__(self, model_path: str, dimensions: int | None = None, local_files_only: bool = True) -> None:
        from sentence_transformers import SentenceTransformer

        self.model = SentenceTransformer(model_path, local_files_only=local_files_only)
        model_dimensions = int(self.model.get_sentence_embedding_dimension() or 0)
        self.dimensions = dimensions or model_dimensions
        if self.dimensions <= 0:
            raise ValueError("Embedding model dimension could not be determined")

    def embed(self, text: str) -> list[float]:
        return self.embed_many([text])[0]

    def embed_many(self, texts: Sequence[str]) -> list[list[float]]:
        vectors = self.model.encode(list(texts), normalize_embeddings=True)
        return [_fit_dimensions([float(value) for value in vector], self.dimensions) for vector in vectors]


def build_embedding_model_from_env() -> EmbeddingModel:
    dimensions = int(os.getenv("EMBEDDING_DIMENSIONS", "384"))
    backend = os.getenv("EMBEDDING_BACKEND", "hash").lower()
    if backend in {"semantic", "sentence-transformers", "sentence_transformers"}:
        return SentenceTransformerEmbeddingModel(
            model_path=os.getenv("EMBEDDING_MODEL_PATH", "/models/embeddings/bge-m3"),
            dimensions=dimensions if os.getenv("EMBEDDING_DIMENSIONS") else None,
            local_files_only=os.getenv("EMBEDDING_LOCAL_FILES_ONLY", "1") not in {"0", "false", "False"},
        )
    return HashEmbeddingModel(dimensions=dimensions)


def _fit_dimensions(vector: list[float], dimensions: int) -> list[float]:
    if len(vector) > dimensions:
        return _normalize(vector[:dimensions])
    if len(vector) < dimensions:
        return vector + [0.0] * (dimensions - len(vector))
    return vector


def _normalize(vector: list[float]) -> list[float]:
    norm = math.sqrt(sum(value * value for value in vector)) or 1.0
    return [value / norm for value in vector]
