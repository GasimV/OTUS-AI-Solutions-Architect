# Architecture Diagrams

Этот раздел связывает PlantUML-диаграммы в единый архитектурный пакет для MVP и целевой production-топологии.

Главная архитектурная идея проекта: закрытая on-prem GraphRAG-платформа с разделением на Control Plane, Data Plane и Security/Operations Plane. MVP запускается локально через Docker Compose, но диаграммы также фиксируют целевое production-развертывание в Kubernetes GPU-кластере.

## C4 Model

- `docs/plantuml/01-c4-context.puml` - C4 Level 1 Context: место GraphRAG-платформы в корпоративном ландшафте.
- `docs/plantuml/02-c4-container.puml` - C4 Level 2 Container: контейнеры платформы, Control Plane, Data Plane и Observability.
- `docs/plantuml/03-c4-component-agent.puml` - C4 Level 3 Component: внутреннее устройство LangGraph-оркестратора.
- `docs/plantuml/10-c4-component-fastapi-backend.puml` - C4 Level 3 Component: FastAPI Backend, REST/SSE API, validation, metrics, audit hooks.
- `docs/plantuml/11-c4-component-guardrails.puml` - C4 Level 3 Component: Guardrails service / policy module.
- `docs/plantuml/04-c4-code.puml` - C4 Level 4 Code/Class: ключевые классы ядра `SecureGraphRagAgent`, `Guardrails`, storage protocols, LLM clients и DTO.

## Specialized Diagrams

- `docs/plantuml/05-deployment-local-compose.puml` - фактическое MVP-развертывание на локальной рабочей станции через Docker Compose.
- `docs/plantuml/06-deployment-prod-k8s.puml` - целевое production-развертывание в on-prem Kubernetes GPU-кластере.
- `docs/plantuml/07-sequence-query.puml` - sequence flow сложного защищенного GraphRAG-запроса.
- `docs/plantuml/08-data-flow.puml` - data flow загрузки документов и выполнения запроса.
- `docs/plantuml/09-er.puml` - ER-модель данных: tenants, users, documents, chunks, ACL, graph entities, sessions, logs.

## API Contract

- `docs/api/openapi.yaml` - OpenAPI 3.1 контракт для текущего FastAPI MVP: `/health`, `/ingest`, `/query`, `/query/stream`.

## Implementation Mapping

В текущем MVP FastAPI Backend, LangGraph-оркестратор и Guardrails выполняются в одном Python API container. Это отражено в component-диаграммах через in-process вызовы:

- FastAPI вызывает `SecureGraphRagAgent.ingest()` и `SecureGraphRagAgent.answer()`;
- LangGraph-агент вызывает Guardrails, Qdrant, Neo4j и vLLM;
- Qdrant хранит vectors и ACL payload;
- Neo4j хранит graph `Document -> Chunk -> Entity`;
- vLLM обслуживает локальную модель `local-qwen` из `models/qwen3-awq`.

В C4 Container Guardrails выделен как отдельный logical/production container. Это целевая архитектурная граница: политики безопасности могут быть вынесены в отдельный сервис без изменения внешнего API и data plane.

Postgres и Vault подняты в Docker Compose как infrastructure boundary для production design. В текущем MVP критичная проверяемая логика GraphRAG и ACL проходит через FastAPI, LangGraph, Qdrant, Neo4j, Guardrails и vLLM. Prometheus/Grafana используются для observability: текущий scrape включает API и Qdrant, а сбор vLLM tokens/sec и GPU metrics зафиксирован в production deployment как следующий шаг.

## Проверяемые Критерии

- No cloud API: ответы генерируются через локальный vLLM, без OpenAI/Anthropic.
- GraphRAG: используется не только vector search, но и Neo4j graph expansion.
- Security-by-design: `tenant_id` и `acl_groups` фильтруются до передачи контекста в LLM.
- Guardrails: prompt injection блокируется до retrieval/generation, output очищается от PII и `<think>...</think>`.
- Streaming: `POST /query/stream` возвращает SSE-события.
