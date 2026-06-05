# Banking Customer Support RAG Assistant — Пакет обеспечения качества

## Обзор системы

Ассистент **Retrieval-Augmented Generation (RAG)**, развернутый для поддержки клиентов розничного банка. Система обслуживает две группы пользователей:

| Канал | Пользователи | Интерфейс |
|---------|-------|-----------|
| Внутренний | Агенты contact-центра | Desktop web app |
| Внешний | Конечные клиенты | Web и mobile chat widget |

Ассистент отвечает на вопросы о продуктах, тарифах, политиках и процедурах эскалации, извлекая релевантные фрагменты из внутренней базы знаний и генерируя ответы через **внешнего LLM provider** (Azure OpenAI). Все документы хранятся в управляемом document store и индексируются в **vector database** для семантического retrieval.

Так как домен относится к **регулируемым финансовым услугам**, система должна обеспечивать строгие меры контроля вокруг приватности данных (PII), безопасности prompt, корректности ответов и auditability.

## Состав пакета

| Файл | Описание |
|------|-------------|
| `architecture_c1.puml` | Диаграмма C4 System Context (C1) — акторы, системы, границы доверия |
| `architecture_c2.puml` | Диаграмма C4 Container (C2) — внутренние контейнеры, потоки данных, компоненты безопасности |
| `security_layer.md` | Дизайн security-layer: карта угроз, контроли, жизненный цикл запроса, guardrail-политики |
| `testing_strategy.md` | План тестирования качества RAG (Ragas/DeepEval), safety-тесты, load-тесты |
| `observability_dashboard.md` | Дизайн Grafana dashboard: Golden Signals + AI-специфичные метрики, alerts, tracing plan |

## Ключевые предположения

1. Доступ к LLM provider (Azure OpenAI) идет через **private endpoint / VPN** из VPC банка, но для целей data-flow он все равно рассматривается как **внешняя, недоверенная** граница.
2. Документы в базе знаний **предварительно классифицированы** по уровню чувствительности (Public, Internal, Confidential). PII sanitization применяется ко всем уровням перед отправкой в LLM.
3. Vector database (например, Qdrant / Weaviate / pgvector) работает **внутри VPC банка**.
4. Аутентификация агентов использует корпоративный **OIDC / SSO** провайдер банка; конечные клиенты аутентифицируются через существующий mobile/web auth банка (OAuth 2.0 + MFA).
5. Все взаимодействия с LLM **логируются и доступны для аудита**, при этом PII редактируется в логах.
6. Система **не** выполняет транзакции: по отношению к Core Banking / CRM она работает в режиме **read-only**. Любое действие, требующее аутентификации (например, сброс пароля, перевод), эскалируется на человека-агента.
7. Управление стоимостью критично: использование токенов и стоимость на запрос должны быть наблюдаемыми и покрытыми alerts.

## Service Level Objectives (SLOs)

| SLO | Цель | Измерение |
|-----|--------|-------------|
| **Availability** | 99.9 % (месячно) | Успешные ответы / все запросы |
| **End-to-end latency (p95)** | ≤ 3 000 ms | От получения в API Gateway до выдачи ответа |
| **LLM call latency (p95)** | ≤ 2 000 ms | От отправки prompt до получения ответа LLM |
| **Retrieval latency (p95)** | ≤ 300 ms | Vector search + reranking |
| **Error rate (5xx)** | < 0.1 % | Ошибки сервера / все запросы |
| **Guardrail block rate** | < 5 % от всех запросов | Заблокировано output validation (слишком высоко → проблема prompt/данных) |
| **Faithfulness (offline eval)** | ≥ 0.85 | Метрика Ragas Faithfulness на gold test set |
| **Answer Relevancy (offline eval)** | ≥ 0.90 | Метрика Ragas Answer Relevancy на gold test set |
| **PII leakage rate** | 0 % | PII обнаружен в ответах LLM после sanitization |
| **Monthly LLM cost** | ≤ лимита бюджета | Отслеживается через token counters × pricing |
