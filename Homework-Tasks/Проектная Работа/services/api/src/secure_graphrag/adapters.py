from __future__ import annotations

import os
import json
from collections.abc import Iterable

from .embeddings import EmbeddingModel, build_embedding_model_from_env
from .schemas import Chunk, RetrievedChunk, UserContext
from .stores import InMemoryGraphStore, InMemoryVectorStore


class QdrantVectorStore:
    def __init__(self, url: str, collection: str, embeddings: EmbeddingModel) -> None:
        from qdrant_client import QdrantClient
        from qdrant_client.models import Distance, VectorParams

        self.client = QdrantClient(url=url)
        self.collection = collection
        self.embeddings = embeddings
        existing = {item.name for item in self.client.get_collections().collections}
        if collection not in existing:
            self.client.create_collection(
                collection_name=collection,
                vectors_config=VectorParams(size=embeddings.dimensions, distance=Distance.COSINE),
            )

    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None:
        from qdrant_client.models import PointStruct

        points = [
            PointStruct(
                id=chunk.id,
                vector=self.embeddings.embed(chunk.text),
                payload={
                    "tenant_id": chunk.tenant_id,
                    "document_id": chunk.document_id,
                    "acl_groups": list(chunk.acl_groups),
                    "text": chunk.text,
                    "ordinal": chunk.ordinal,
                    "metadata": chunk.metadata,
                    "entities": list(chunk.entities),
                },
            )
            for chunk in chunks
        ]
        if points:
            self.client.upsert(collection_name=self.collection, points=points)

    def search(self, query: str, user: UserContext, top_k: int) -> list[RetrievedChunk]:
        from qdrant_client.models import FieldCondition, Filter, MatchAny, MatchValue

        query_filter = Filter(
            must=[
                FieldCondition(key="tenant_id", match=MatchValue(value=user.tenant_id)),
                FieldCondition(key="acl_groups", match=MatchAny(any=list(user.groups))),
            ]
        )
        response = self.client.query_points(
            collection_name=self.collection,
            query=self.embeddings.embed(query),
            query_filter=query_filter,
            limit=top_k,
            with_payload=True,
        )
        points = getattr(response, "points", response)
        return [self._point_to_result(point) for point in points]

    def _point_to_result(self, point: object) -> RetrievedChunk:
        payload = getattr(point, "payload")
        chunk = Chunk(
            id=str(getattr(point, "id")),
            document_id=payload["document_id"],
            tenant_id=payload["tenant_id"],
            text=payload["text"],
            acl_groups=tuple(payload["acl_groups"]),
            ordinal=payload["ordinal"],
            metadata=payload.get("metadata", {}),
            entities=tuple(payload.get("entities", [])),
        )
        return RetrievedChunk(chunk=chunk, score=float(getattr(point, "score", 0.0)), source="vector")


class Neo4jGraphStore:
    def __init__(self, uri: str, user: str, password: str) -> None:
        from neo4j import GraphDatabase

        self.driver = GraphDatabase.driver(uri, auth=(user, password))

    def upsert_chunks(self, chunks: Iterable[Chunk]) -> None:
        rows = [
            {
                "id": chunk.id,
                "document_id": chunk.document_id,
                "tenant_id": chunk.tenant_id,
                "text": chunk.text,
                "acl_groups": list(chunk.acl_groups),
                "ordinal": chunk.ordinal,
                "metadata_json": json.dumps({"document_id": chunk.document_id, **chunk.metadata}, ensure_ascii=False),
                "entities": list(chunk.entities),
            }
            for chunk in chunks
        ]
        if not rows:
            return
        query = """
        UNWIND $rows AS row
        MERGE (d:Document {id: row.document_id, tenant_id: row.tenant_id})
        MERGE (c:Chunk {id: row.id})
        SET c.tenant_id = row.tenant_id,
            c.text = row.text,
            c.acl_groups = row.acl_groups,
            c.ordinal = row.ordinal,
            c.metadata_json = row.metadata_json
        MERGE (d)-[:HAS_CHUNK]->(c)
        WITH c, row
        UNWIND row.entities AS entity_name
        MERGE (e:Entity {name: entity_name, tenant_id: row.tenant_id})
        MERGE (c)-[:MENTIONS]->(e)
        """
        with self.driver.session() as session:
            session.run(query, rows=rows)

    def expand(self, seed_chunk_ids: Iterable[str], user: UserContext, limit: int) -> list[RetrievedChunk]:
        query = """
        MATCH (c:Chunk {tenant_id: $tenant_id})-[:MENTIONS]->(e:Entity)<-[:MENTIONS]-(n:Chunk {tenant_id: $tenant_id})
        WHERE c.id IN $seed_ids AND NOT n.id IN $seed_ids AND any(g IN n.acl_groups WHERE g IN $groups)
        WITH n, count(e) AS shared_entities
        ORDER BY shared_entities DESC
        LIMIT $limit
        RETURN n.id AS id, n.text AS text, n.acl_groups AS acl_groups, n.ordinal AS ordinal,
               n.metadata_json AS metadata_json, shared_entities AS score
        """
        with self.driver.session() as session:
            rows = list(
                session.run(
                    query,
                    seed_ids=list(seed_chunk_ids),
                    tenant_id=user.tenant_id,
                    groups=list(user.groups),
                    limit=limit,
                )
            )
        results: list[RetrievedChunk] = []
        for row in rows:
            metadata = json.loads(row["metadata_json"] or "{}")
            chunk = Chunk(
                id=row["id"],
                document_id=metadata.get("document_id", "unknown"),
                tenant_id=user.tenant_id,
                text=row["text"],
                acl_groups=tuple(row["acl_groups"]),
                ordinal=row["ordinal"],
                metadata=metadata,
                entities=(),
            )
            results.append(RetrievedChunk(chunk=chunk, score=float(row["score"]) / 10.0, source="graph"))
        return results


def build_stores_from_env() -> tuple[object, object]:
    backend = os.getenv("STORE_BACKEND", "memory").lower()
    embeddings = build_embedding_model_from_env()
    if backend == "external":
        return (
            QdrantVectorStore(
                url=os.getenv("QDRANT_URL", "http://qdrant:6333"),
                collection=os.getenv("QDRANT_COLLECTION", "corp_chunks"),
                embeddings=embeddings,
            ),
            Neo4jGraphStore(
                uri=os.getenv("NEO4J_URI", "bolt://neo4j:7687"),
                user=os.getenv("NEO4J_USER", "neo4j"),
                password=os.getenv("NEO4J_PASSWORD", "change-me"),
            ),
        )
    return InMemoryVectorStore(embeddings=embeddings), InMemoryGraphStore()
