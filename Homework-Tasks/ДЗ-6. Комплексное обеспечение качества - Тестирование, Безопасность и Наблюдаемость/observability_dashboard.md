# Дизайн Observability Dashboard — Banking RAG Assistant

## 1. Toolchain

| Tool | Назначение | Protocol |
|------|---------|----------|
| **Prometheus** | Сбор и хранение metrics | OTLP / Prometheus remote-write |
| **Tempo** | Distributed tracing | OTLP (через OpenTelemetry Collector) |
| **Loki** | Агрегация логов | Fluentd / Promtail → Loki |
| **Grafana** | Dashboards и alerting | Запросы к Prometheus, Tempo, Loki как к data sources |
| **Langfuse** (self-hosted) | LLM-специфичные traces: логирование prompt/completion, token counts, tracking cost, evaluation scores | SDK instrumentation в Orchestrator |
| **OpenTelemetry Collector** | Центральный telemetry pipeline | Получает OTLP от всех сервисов, экспортирует в Prometheus/Tempo/Loki |

---

## 2. Grafana Dashboard: "Banking RAG Assistant — Operations"

### Layout dashboard (14 виджетов, 5 секций)

---

### Секция A: Golden Signals — Latency

#### Виджет A1: End-to-End Latency (p50 / p95 / p99)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart |
| **Metric** | `histogram_quantile(0.5, rate(rag_request_duration_seconds_bucket[5m]))` (повторить для 0.95, 0.99) |
| **Labels** | `method`, `endpoint`, `user_role` |
| **SLO line** | Горизонтальная аннотация на 3 s (цель p95) |
| **Time range** | Последние 6 часов |

#### Виджет A2: Разбивка latency по этапам

| Свойство | Значение |
|----------|-------|
| **Type** | Stacked bar chart (среднее по минутам) |
| **Metrics** | `rag_stage_duration_seconds{stage="api_gateway"}`, `…{stage="retrieval"}`, `…{stage="reranker"}`, `…{stage="pii_sanitization"}`, `…{stage="llm_call"}`, `…{stage="guardrails"}` |
| **Labels** | `stage` |
| **Purpose** | Определить этап, который доминирует в latency |

#### Виджет A3: LLM Provider Latency (p50 / p95)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart |
| **Metric** | `histogram_quantile(0.95, rate(rag_llm_call_duration_seconds_bucket[5m]))` |
| **Labels** | `model`, `provider` |
| **SLO line** | 2 s (цель p95) |

---

### Секция B: Golden Signals — Traffic

#### Виджет B1: Requests Per Second (RPS)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart |
| **Metric** | `rate(rag_requests_total[1m])` |
| **Labels** | `user_role` (agent / customer), `endpoint` |
| **Purpose** | Объем трафика и обнаружение паттернов |

#### Виджет B2: Active Users (Gauge)

| Свойство | Значение |
|----------|-------|
| **Type** | Stat panel (текущее значение) + sparkline |
| **Metric** | `rag_active_sessions_current` |
| **Labels** | `user_role` |
| **Purpose** | Мониторинг concurrency в реальном времени |

---

### Секция C: Golden Signals — Errors

#### Виджет C1: Error Rate (4xx / 5xx / LLM Errors)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart со stacked areas |
| **Metrics** | `rate(rag_requests_total{status=~"4.."}[5m])`, `rate(rag_requests_total{status=~"5.."}[5m])`, `rate(rag_llm_errors_total[5m])` |
| **Labels** | `status`, `error_type` |
| **SLO line** | 0.1% от общего RPS (цель для 5xx) |

#### Виджет C2: Guardrail Block Rate

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart + percentage stat |
| **Metric** | `rate(rag_guardrail_blocks_total[5m]) / rate(rag_requests_total[5m]) * 100` |
| **Labels** | `block_reason` (pii_leak, policy_violation, hallucination, toxicity, injection) |
| **Threshold** | Warning при > 5% (указывает на проблему качества prompt или данных) |

---

### Секция D: Golden Signals — Saturation

#### Виджет D1: Ресурсы системы (CPU / Memory)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart (2 y-axes) |
| **Metrics** | `process_cpu_seconds_total` (rate), `process_resident_memory_bytes` |
| **Labels** | `service` (orchestrator, retriever и т.д.) |
| **Threshold** | Alert при CPU > 80%, Memory > 85% |

#### Виджет D2: Queue Depth и Concurrency

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart |
| **Metrics** | `rag_request_queue_depth`, `rag_llm_concurrent_calls_current` |
| **Labels** | `service` |
| **Purpose** | Обнаружить back-pressure до скачков latency |

---

### Секция E: AI-специфичные метрики

#### Виджет E1: Token Usage на запрос (Input / Output)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart (dual y-axis: input tokens, output tokens) |
| **Metrics** | `rate(rag_llm_tokens_total{direction="input"}[5m])`, `rate(rag_llm_tokens_total{direction="output"}[5m])` |
| **Labels** | `model`, `direction` |
| **Purpose** | Отслеживание тренда потребления токенов, детекция аномалий |

#### Виджет E2: Средняя стоимость на запрос (USD)

| Свойство | Значение |
|----------|-------|
| **Type** | Stat panel (текущее среднее) + time-series trend |
| **Metric** | `rate(rag_llm_cost_usd_total[1h]) / rate(rag_requests_total[1h])` |
| **Labels** | `model` |
| **Alert** | Дневная стоимость > порога бюджета |

#### Виджет E3: Сигналы качества retrieval

| Свойство | Значение |
|----------|-------|
| **Type** | Multi-stat panel (3 значения) |
| **Metrics** | `rag_retrieval_topk_hit_rate` (% запросов, где ≥1 релевантный chunk в top-k), `rag_reranker_latency_seconds` (p95), `rag_retrieval_empty_context_rate` (% запросов с 0 chunks выше порога) |
| **Labels** | `retriever_version` |
| **Alert** | Empty-context rate > 3% |

#### Виджет E4: Тренды качества RAG (Offline Eval Scores)

| Свойство | Значение |
|----------|-------|
| **Type** | Time-series line chart (одна линия на метрику) |
| **Metrics** | `rag_eval_faithfulness_score`, `rag_eval_answer_relevancy_score`, `rag_eval_context_precision_score`, `rag_eval_context_recall_score` |
| **Labels** | `eval_run_id`, `model`, `prompt_version` |
| **Source** | Публикуется в Prometheus после каждого offline eval run |
| **Purpose** | Отслеживание трендов качества при изменениях model/prompt |

#### Виджет E5: Security — Prompt Injection Detection

| Свойство | Значение |
|----------|-------|
| **Type** | Stat panel (счетчик) + time-series trend |
| **Metrics** | `rate(rag_prompt_injection_detected_total[1h])`, `rate(rag_prompt_injection_blocked_total[1h])` |
| **Labels** | `detection_method` (regex, ml_classifier), `action` (blocked, flagged) |
| **Alert** | Любой всплеск > 5 detections / min |

---

## 3. Имена метрик Prometheus и labels

### 3.1 Полный реестр метрик

```
# ── Request metrics ────────────────────────────────────
rag_requests_total                    counter    {method, endpoint, status, user_role}
rag_request_duration_seconds          histogram  {method, endpoint, user_role}
rag_request_queue_depth               gauge      {service}
rag_active_sessions_current           gauge      {user_role}

# ── Stage-level latency ───────────────────────────────
rag_stage_duration_seconds            histogram  {stage}
  # stage: api_gateway, auth, retrieval, reranker,
  #        prompt_build, pii_sanitization, llm_call,
  #        guardrails, pii_desanitization

# ── LLM metrics ───────────────────────────────────────
rag_llm_call_duration_seconds         histogram  {model, provider}
rag_llm_tokens_total                  counter    {model, direction}  # direction: input|output
rag_llm_cost_usd_total               counter    {model}
rag_llm_errors_total                  counter    {model, error_type}
rag_llm_concurrent_calls_current      gauge      {model}

# ── Retrieval metrics ─────────────────────────────────
rag_retrieval_topk_hit_rate           gauge      {retriever_version}
rag_retrieval_empty_context_rate      gauge      {retriever_version}
rag_reranker_latency_seconds          histogram  {reranker_model}

# ── Guardrails metrics ────────────────────────────────
rag_guardrail_blocks_total            counter    {block_reason}
  # block_reason: pii_leak, policy_violation,
  #              hallucination, toxicity, off_topic

# ── Security metrics ──────────────────────────────────
rag_prompt_injection_detected_total   counter    {detection_method}
rag_prompt_injection_blocked_total    counter    {detection_method, action}
rag_pii_tokens_replaced_total         counter    {entity_type}  # name, account, ssn, etc.
rag_rate_limit_exceeded_total         counter    {user_role, limit_type}

# ── Quality metrics (from offline eval) ───────────────
rag_eval_faithfulness_score           gauge      {eval_run_id, model, prompt_version}
rag_eval_answer_relevancy_score       gauge      {eval_run_id, model, prompt_version}
rag_eval_context_precision_score      gauge      {eval_run_id, model, prompt_version}
rag_eval_context_recall_score         gauge      {eval_run_id, model, prompt_version}

# ── Cache metrics ─────────────────────────────────────
rag_cache_hit_total                   counter    {}
rag_cache_miss_total                  counter    {}
```

---

## 4. Alert Rules (6 правил)

### Правило 1: Высокая End-to-End Latency (нарушение SLO)

```yaml
alert: RAGHighLatencyP95
expr: |
  histogram_quantile(0.95,
    rate(rag_request_duration_seconds_bucket[5m])
  ) > 3.0
for: 5m
labels:
  severity: critical
  team: rag-platform
annotations:
  summary: "Задержка p95 превышает 3с SLO в течение 5 минут"
  runbook: "Проверьте значение параметра rag_stage_duration_seconds, чтобы определить медленный этап; проверьте статус поставщика LLM."
```

### Правило 2: Высокий Error Rate (нарушение SLO)

```yaml
alert: RAGHighErrorRate
expr: |
  (
    rate(rag_requests_total{status=~"5.."}[5m])
    / rate(rag_requests_total[5m])
  ) > 0.001
for: 5m
labels:
  severity: critical
  team: rag-platform
annotations:
  summary: "Частота ошибок 5xx превышает 0.1% SLO"
  runbook: "Проверьте журналы Loki для получения подробной информации об ошибках; проверьте состояние нижестоящих служб."
```

### Правило 3: Аномалия стоимости LLM

```yaml
alert: RAGCostAnomaly
expr: |
  rate(rag_llm_cost_usd_total[1h]) > 50
for: 10m
labels:
  severity: warning
  team: rag-platform
annotations:
  summary: "Расходы на обучение по программе LLM превышают 50 долларов в час — потенциальная угроза роста стоимости или утечка токенов."
  runbook: "Проверьте использование токенов каждым пользователем; убедитесь в отсутствии переполнения кэша запросами; проверьте коэффициент попадания в кэш."
```

### Правило 4: Всплеск Prompt Injection

```yaml
alert: RAGPromptInjectionSpike
expr: |
  rate(rag_prompt_injection_detected_total[5m]) > 0.083
for: 2m
labels:
  severity: warning
  team: security
annotations:
  summary: "Высокая скорость обнаружения инъекций (>5/мин) — возможна атака"
  runbook: "Проанализируйте трассировки Langfuse; проверьте исходные IP-адреса; рассмотрите возможность временного ужесточения ограничений на количество запросов."
```

### Правило 5: Обнаружение утечки PII

```yaml
alert: RAGPIILeakageDetected
expr: |
  increase(rag_guardrail_blocks_total{block_reason="pii_leak"}[10m]) > 0
for: 0m
labels:
  severity: critical
  team: security
annotations:
  summary: "В выходных данных LLM обнаружены персональные данные — защитный механизм (guardrail) заблокировал ответ."
  runbook: "Необходимо незамедлительно провести расследование; проверить sanitizer logs; выяснить, нужно ли добавить новые PII шаблоны."
```

### Правило 6: Error Budget Burn Rate

```yaml
alert: RAGErrorBudgetFastBurn
expr: |
  (
    1 - (
      sum(rate(rag_requests_total{status!~"5.."}[1h]))
      / sum(rate(rag_requests_total[1h]))
    )
  ) > 0.001 * 24 * 30 / 2
  # Burns >50% of monthly budget in 1 hour
for: 5m
labels:
  severity: critical
  team: rag-platform
annotations:
  summary: "Критически важен темп расходования бюджета на ошибки — более 50% месячного бюджета расходуется за 1 час."
  runbook: "Оцените последствия инцидента; рассмотрите возможность отмены изменений (rollback); привлеките дежурного специалиста."
```

---

## 5. План Distributed Tracing

### 5.1 Instrumentation: OpenTelemetry SDK

Все сервисы инструментированы OpenTelemetry SDK. Traces экспортируются по OTLP в OpenTelemetry Collector, который отправляет данные в Tempo (traces) и Langfuse (LLM-specific traces).

### 5.2 Ключевые spans

```
[Trace: rag_request]
├── [Span: api_gateway]
│   ├── attr: http.method, http.route, http.status_code
│   └── attr: user_role, request_size_bytes
│
├── [Span: auth]
│   └── attr: auth_method, auth_result
│
├── [Span: cache_lookup]
│   └── attr: cache_hit (bool)
│
├── [Span: retrieval]
│   ├── attr: query_embedding_time_ms, vector_db_query_time_ms
│   ├── attr: top_k, chunks_returned, max_similarity_score
│   └── [Span: reranker]
│       └── attr: reranker_model, chunks_in, chunks_out, rerank_time_ms
│
├── [Span: prompt_build]
│   └── attr: prompt_version, template_id, total_tokens_estimate
│
├── [Span: pii_sanitization]
│   └── attr: entities_found, entities_replaced, sanitization_time_ms
│
├── [Span: policy_check_pre_llm]
│   └── attr: injection_score, policy_decision (allow/block)
│
├── [Span: llm_call]
│   ├── attr: model, provider, input_tokens, output_tokens
│   ├── attr: finish_reason, llm_latency_ms
│   └── attr: estimated_cost_usd
│
├── [Span: guardrails_validation]
│   ├── attr: checks_run[], checks_passed[], block_reason (if any)
│   └── attr: validation_time_ms
│
├── [Span: pii_desanitization]
│   └── attr: tokens_restored, desanitization_time_ms
│
└── [Span: audit_log_write]
    └── attr: log_record_id, redaction_applied (bool)
```

### 5.3 Общие trace attributes (пробрасываются во все spans)

| Attribute | Описание | Пример |
|-----------|-------------|---------|
| `request_id` | Уникальный идентификатор запроса (UUID) | `req_a1b2c3d4-...` |
| `user_id_hash` | SHA-256 хэш ID пользователя (privacy) | `sha256:7f83b1...` |
| `session_id` | ID сессии диалога | `sess_xyz789` |
| `user_role` | Agent или customer | `agent` |
| `model` | Используемая LLM model | `gpt-4o-2024-08-06` |
| `prompt_version` | Версия prompt template | `v2.4.1` |
| `retriever_version` | Версия конфигурации retriever | `v1.2` |
| `environment` | Среда деплоя | `production` |

### 5.4 Интеграция Langfuse

Langfuse собирает LLM-специфичные trace-данные, которых нет в Tempo:

- Полные тексты prompt и completion (PII-redacted).
- Количество токенов и расчет стоимости.
- User feedback scores (thumbs up/down от агентов).
- Evaluation scores (когда включен online eval).
- Lineage версий prompt.

Traces Langfuse связываются с traces Tempo по `request_id` для кросс-сопоставления.

---

## 6. Сводка dashboard (визуальный layout)

```
┌─────────────────────────────────────────────────────────────────────────┐
│  BANKING RAG ASSISTANT — OPERATIONS DASHBOARD                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ┌─── A: LATENCY ──────────────────────────────────────────────────┐   │
│  │ [A1 E2E Latency p50/p95]  [A2 Stage Breakdown]  [A3 LLM p95]  │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│                                                                         │
│  ┌─── B: TRAFFIC ──────────────────┐ ┌─── C: ERRORS ──────────────┐   │
│  │ [B1 RPS]      [B2 Active Users] │ │ [C1 Error Rate]  [C2 Blocks]│  │
│  └─────────────────────────────────┘ └─────────────────────────────┘   │
│                                                                         │
│  ┌─── D: SATURATION ──────────────────────────────────────────────┐   │
│  │ [D1 CPU / Memory]              [D2 Queue Depth / Concurrency]  │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│                                                                         │
│  ┌─── E: AI METRICS ──────────────────────────────────────────────┐   │
│  │ [E1 Token Usage]  [E2 Cost/Req]  [E3 Retrieval Quality]       │   │
│  │ [E4 Quality Trends]              [E5 Injection Detection]      │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```
