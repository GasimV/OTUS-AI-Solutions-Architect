# ADR-003: GraphRAG storage and ACL on chunks/nodes

## Status

Accepted.

## Context

Простой vector search не принимается по заданию. Нужен GraphRAG, который снижает потерю контекста и ограничивает выдачу секретных документов.

## Decision

Используем две базы:

- Qdrant: dense vector retrieval по chunk embeddings с payload-фильтрами `tenant_id` и `acl_groups`.
- Neo4j: knowledge graph `Document -> Chunk -> Entity`, где `Chunk` содержит `tenant_id`, `acl_groups` и metadata.

Retrieval flow:

1. Векторный поиск возвращает только доступные пользователю chunks.
2. Graph expansion находит соседние chunks через общие entities.
3. Graph expansion повторно фильтрует соседние chunks по `tenant_id` и `acl_groups` до generation.
4. Output guardrail удаляет недоступные citations.

## Trade-off

- Плюсы: символические связи сохраняют контекст между фрагментами; ACL применяется до LLM, а не после факта.
- Минусы: нужно поддерживать согласованность Qdrant и Neo4j.
- Отклонено: хранить ACL только на уровне документа. Для сложных PDF один документ может содержать разные уровни секретности.

## Consequences

Минимальная MVP-онтология в Neo4j остается простой: `Document`, `Chunk`, `Entity`, связи `HAS_CHUNK` и `MENTIONS`. Группы доступа в MVP хранятся как свойство `acl_groups` на chunk и как payload в Qdrant.

В ER-модели и production-дизайне `AccessGroup` выделяется в отдельную сущность для синхронизации с AD/LDAP/Keycloak. Граф можно расширить типами `Table`, `Figure`, `DrawingElement`, `Procedure`, не меняя внешний API.
