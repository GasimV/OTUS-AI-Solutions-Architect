# ADR-001: On-prem closed contour and plane separation

## Status

Accepted.

## Context

Система должна работать автономно в закрытом контуре банка или промышленного предприятия РФ. Запрещены облачные LLM API. Критерий оценки требует явного разделения Control Plane и Data Plane.

## Decision

Разделяем платформу на:

- Control Plane: FastAPI, LangGraph agent, guardrails, orchestration, session/request policy.
- Data Plane: vLLM, Qdrant, Neo4j, Postgres.
- Security/Operations Plane: Vault, Prometheus, Grafana, future OpenTelemetry Collector/SIEM.

В MVP API container подключен к сетям `dmz`, `internal` и `model`, чтобы принимать внешние запросы и вызывать внутренние сервисы. Базы изолированы во внутренней сети `internal`, модельный backend vLLM - во внутренней сети `model`. Admin/UI endpoints опубликованы наружу только для локальной демонстрации.

Для документации фиксируются две deployment-модели:

- MVP deployment: локальная рабочая станция с Docker Engine или Docker Desktop, Docker Compose и NVIDIA RTX 5080 Laptop GPU 16 GB.
- Production deployment: on-prem Kubernetes GPU-кластер в дата-центре с GPU node pool, StatefulSets для баз, NetworkPolicy, Vault HA и observability namespace.

Контейнерные границы и разделение плоскостей логически одинаковы в обеих моделях, поэтому локальный Compose является воспроизводимым демо-слоем, а Kubernetes - целевой промышленной топологией.

## Trade-off

- Плюсы: проще применить network segmentation, audit, zero-trust доступ к данным и независимое масштабирование LLM/data services.
- Минусы: больше сервисов и operational overhead по сравнению с монолитом.
- Отклонено: единый Python-скрипт с локальными файлами. Он не показывает production boundaries, RBAC и observability.

## Consequences

Минимальный MVP запускается локально, но архитектурно переносится на Kubernetes/on-prem GPU cluster: API replicas отдельно, vLLM replicas на GPU nodes, Qdrant/Neo4j/Postgres на storage nodes, Vault и observability в отдельных operational namespaces.
