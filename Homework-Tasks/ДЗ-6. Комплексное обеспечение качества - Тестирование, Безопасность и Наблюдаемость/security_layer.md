# Дизайн Security Layer — Banking RAG Assistant

## 1. Обзор

Security layer спроектирован как pipeline **defense-in-depth**, встроенный в жизненный цикл каждого запроса. Так как система работает в регулируемой банковской среде и использует внешний LLM provider, каждый поток данных, пересекающий границу доверия, проходит sanitization, validation и auditing.

Ключевые принципы:
- **Zero-trust для LLM**: LLM рассматривается как недоверенная третья сторона; никакие PII, credentials или сырые внутренние данные не покидают VPC банка без sanitization.
- **Defense-in-depth**: несколько независимых контролей, чтобы отказ одного слоя не компрометировал систему.
- **Аудит всего**: неизменяемые PII-redacted логи для каждого взаимодействия.
- **Least privilege**: каждый сервис имеет минимально необходимые права; secrets запрашиваются во время выполнения из Vault.

---

## 2. Карта security controls

| # | Угроза | OWASP LLM Top 10 | Митигация | Где реализовано | Telemetry / Alert |
|---|--------|-------------------|------------|-------------------|-------------------|
| 1 | **Direct Prompt Injection** — атакующий формирует ввод для переопределения system prompt | LLM01: Prompt Injection | Input classifier (regex + ML) в Policy Engine; изоляция system prompt; sandwich defense (повтор инструкций после контекста) | Policy Engine → проверка до Prompt Builder | Счетчик `security_prompt_injection_detected_total`; alert при всплеске > 5 / min |
| 2 | **Indirect Prompt Injection** — вредоносные инструкции в извлеченных документах | LLM01: Prompt Injection | Integrity checksums на уровне документа; content scanning при ingestion; маркеры-разделители инструкций в prompt template | Pipeline ingestion документов + Prompt Builder | `security_indirect_injection_flagged_total`; alert при любом срабатывании |
| 3 | **PII / Data Leakage в LLM** — персональные данные отправляются внешнему provider | LLM06: Sensitive Information Disclosure | PII Sanitizer (Presidio + custom banking NER): заменяет PII на токены `<PLACEHOLDER_xxx>` перед вызовом LLM; mapping хранится в эфемерном in-memory store с коротким TTL | PII Sanitizer (pre-LLM) | `pii_tokens_replaced_total`; alert если PII найден после sanitization (признак обхода) |
| 4 | **PII в ответе LLM** — модель генерирует или повторяет PII | LLM06: Sensitive Information Disclosure | Output Guardrails сканирует ответ на PII before de-sanitization; блокировка при неожиданном PII | Guardrails / Output Validator | `guardrails_pii_in_response_blocked_total` |
| 5 | **Чувствительные данные в logs / traces** — утечка PII в observability stack | LLM06 | Audit Service применяет PII-redaction перед записью; log pipeline использует DLP filter; Langfuse настроен на маскирование чувствительных полей | Logging/Audit Service + Telemetry Exporters | Периодический DLP scan хранилища логов; alert при обнаружении PII |
| 6 | **Hallucination модели / небезопасные финансовые советы** — модель выдумывает информацию или дает actionable financial advice | LLM09: Misinformation | Guardrails checks: (a) ответ должен быть grounded в извлеченном контексте, (b) обязательные disclaimer для финансовых тем, (c) запрет конкретных инструкций действий (например, "переведите деньги на…") | Guardrails / Output Validator + Policy Engine | `guardrails_hallucination_blocked_total`; `guardrails_policy_violation_total` |
| 7 | **Утечка secrets (API keys)** — раскрытие LLM API keys или DB credentials | LLM06 | Все secrets хранятся в HashiCorp Vault; получение во время выполнения с short-lived leases; не хранятся в env vars, коде или config; политика rotation secrets (90 дней) | Secrets Manager / Vault + deployment config | Vault audit log; alert на неавторизованные попытки доступа |
| 8 | **Abuse: Prompt Flooding / Cost Attack** — атакующий отправляет высокий объем запросов, чтобы исчерпать бюджет LLM | LLM04: Model Denial of Service | WAF rate limiting (per-IP, per-user); token-based rate limits в API Gateway; дневной token budget на пользователя; circuit breaker по расходу LLM | WAF + API Gateway + Orchestrator | `rate_limit_exceeded_total`; `llm_daily_cost_usd` с budget alert |
| 9 | **Abuse: Data Scraping через RAG** — систематические запросы для извлечения базы знаний | LLM06 | Per-session query rate limiting; anomaly detection паттернов запросов; ограничение scope retrieval по роли пользователя | API Gateway + Policy Engine | `security_scraping_anomaly_detected_total` |
| 10 | **Insecure Plugin / Tool Use** — если в системе есть tool-calling | LLM07: Insecure Plugin Design | Система по дизайну **read-only**; нет выполнения tool; действия, требующие аутентификации, всегда эскалируются на человека-агента | Архитектурное ограничение (нет write-capable tools) | N/A — обеспечено на уровне дизайна |

---

## 3. Жизненный цикл запроса — security touchpoints

```
Запрос пользователя
  │
  ▼
┌─────────────────────────────────────────────────────────────────┐
│ 1. WAF / CDN Edge                                               │
│    • DDoS protection, geo-blocking, bot detection               │
│    • IP-based rate limiting                                     │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 2. API Gateway / BFF                                            │
│    • TLS termination                                            │
│    • JWT verification (signature + expiry + audience)           │
│    • Per-user / per-role rate limiting                          │
│    • Request size validation (max tokens cap)                   │
│    • Input schema validation (JSON schema)                      │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 3. AuthN / AuthZ Service                                        │
│    • OIDC token introspection с IdP                             │
│    • RBAC: роль agent vs customer → разные scope retrieval      │
│    • Session binding (защита от replay token)                   │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 4. Conversation Orchestrator                                    │
│    • Проверка cache (если hit, перейти к шагу 10)               │
│    • Проверка rate/budget на пользователя                       │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 5. Retriever + Re-ranker                                        │
│    • Retrieval с ограничением scope (role-based access)         │
│    • Проверка integrity документов (валидация checksum)         │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 6. Prompt Builder                                               │
│    • Сборка system prompt + context + query                     │
│    • Разделители инструкций вокруг retrieved context            │
│    • Sandwich defense (повтор критичных инструкций после ctx)   │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 7. PII Sanitizer (PRE-LLM) ◄── КРИТИЧЕСКИЙ SECURITY GATE       │
│    • NER-based обнаружение PII (Presidio + banking rules)       │
│    • Замена PII на токены <PLACEHOLDER_xxxx>                    │
│    • Хранение mapping в эфемерной памяти (TTL = lifetime запроса)│
│    • Лог: количество замененных entities (без значений)         │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 8. Policy Engine — Pre-LLM Check                                │
│    • Prompt injection classifier (regex + ML model)             │
│    • Проверка границ темы (входит ли query в scope?)            │
│    • Проверка token budget                                      │
│    • Решение: ALLOW / BLOCK / ESCALATE                          │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 9. LLM Call (Azure OpenAI)                                      │
│    • API key запрашивается из Vault (short-lived lease)         │
│    • mTLS / private endpoint                                    │
│    • Request timeout + circuit breaker                          │
│    • Фиксация token usage                                       │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 10. Guardrails / Output Validator ◄── КРИТИЧЕСКИЙ SECURITY GATE │
│    • PII scan ответа LLM (block при неожиданном PII)            │
│    • Оценка в Policy Engine:                                    │
│      - Запрет раскрытия account number                          │
│      - Запрет actionable financial instructions                 │
│      - Обязательные disclaimer для investment/insurance тем      │
│      - Запрет ответов с упоминанием конкурентов                 │
│    • Проверка hallucination (есть grounding в контексте?)       │
│    • Фильтр toxicity / harmful content                          │
│    • Решение: PASS / REDACT / BLOCK / FALLBACK                  │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 11. PII De-sanitizer (POST-LLM)                                 │
│    • Восстановление <PLACEHOLDER_xxxx> → исходные значения PII  │
│    • Только для user-facing ответа; никогда не в logs           │
│    • Очистка эфемерного mapping после восстановления            │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│ 12. Audit Log + Telemetry                                       │
│    • Запись PII-redacted audit record (immutable, append-only)  │
│    • Экспорт trace spans, metrics, LLM analytics в observability│
│    • DLP filter контента логов как финальная защита             │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
                    Ответ пользователю
```

---

## 4. Соответствие OWASP Top 10 for LLM Applications

| OWASP LLM Category | Покрытие | Основные controls |
|---------------------|-----------|-----------------|
| **LLM01 — Prompt Injection** | Да | Policy Engine (regex + ML classifier), sandwich defense, input validation |
| **LLM02 — Insecure Output Handling** | Да | Guardrails / Output Validator, PII scan ответа, content-type enforcement |
| **LLM03 — Training Data Poisoning** | Частично | Вне scope (внешний LLM), но integrity checks документов защищают retrieval data |
| **LLM04 — Model Denial of Service** | Да | Rate limiting (WAF + API GW), token budgets на пользователя, circuit breaker |
| **LLM05 — Supply Chain Vulnerabilities** | Да | Vault для secrets, pinned model versions, dependency scanning в CI/CD |
| **LLM06 — Sensitive Information Disclosure** | Да | PII Sanitizer, PII scan на выходе, DLP в logs, Vault для secrets |
| **LLM07 — Insecure Plugin Design** | N/A | Нет выполнения tools — read-only система по дизайну |
| **LLM08 — Excessive Agency** | Да | Система read-only; все действия эскалируются человеку; нет автономных транзакций |
| **LLM09 — Overreliance / Misinformation** | Да | Guardrails (grounding check, disclaimer), source citations в ответах |
| **LLM10 — Model Theft** | N/A | Внешний LLM — риск model theft на стороне provider; prompts санитизируются |

---

## 5. Рекомендуемый tooling

| Функция | Tool(s) | Примечания |
|----------|---------|-------|
| WAF / Edge protection | Azure Front Door / AWS WAF / Cloudflare | DDoS, bot detection, geo-blocking |
| API Gateway | Kong / Envoy / AWS API Gateway | Rate limiting, JWT validation, routing |
| PII Detection & Sanitization | Microsoft Presidio + custom banking NER | Open-source, расширяемые recognizers для account numbers, card numbers и т.д. |
| Secrets Management | HashiCorp Vault | Dynamic secrets, short-lived leases, audit log |
| Prompt Injection Detection | Custom ML classifier (fine-tuned DistilBERT) + regex rules | Обучение на prompt injection datasets; обновление ежеквартально |
| Output Guardrails | Guardrails AI / NVIDIA NeMo Guardrails | Настраиваемые rails: topic, factuality, PII, toxicity |
| Policy Engine | Open Policy Agent (OPA) | Декларативные rules, удобно versioning и testing |
| DLP в логах | Custom Fluentd plugin + Presidio | Сканирование log entries перед хранением |
| LLM Observability | Langfuse (self-hosted) | Prompt/completion logging, cost tracking, evaluation scores |
| Auth | Keycloak / Okta / Azure AD | OIDC + OAuth 2.0, RBAC |

---

## 6. Примеры guardrail-политик

### Политика 1: Запрет раскрытия account number

```yaml
# guardrails_policy.yaml
- id: deny_account_number_disclosure
  description: >
    Ассистент НИКОГДА не должен включать полные номера банковских счетов,
    карт или IBAN в ответы, даже если пользователь об этом просит.
  trigger: output_contains_pattern
  patterns:
    - '\b\d{16,20}\b'                 # Card numbers (16-20 digits)
    - '\b\d{10,12}\b'                 # Account numbers (10-12 digits)
    - '\b[A-Z]{2}\d{2}[A-Z0-9]{11,30}\b'  # IBAN
  action: REDACT
  replacement: "[REDACTED — please contact your branch or use secure banking app]"
  severity: CRITICAL
  alert: true
```

### Политика 2: Отказ от действий, требующих authentication

```yaml
- id: refuse_authenticated_actions
  description: >
    Ассистент НЕ должен инструктировать пользователей выполнять действия,
    которые требуют банковской authentication (переводы, сброс пароля,
    изменение аккаунта). Вместо этого нужна эскалация на человека-агента.
  trigger: output_intent_classification
  denied_intents:
    - money_transfer
    - password_reset
    - account_modification
    - card_block
    - loan_application_submit
  action: REPLACE
  replacement: >
    I cannot perform this action directly. Let me connect you with a
    specialist who can help you securely. Please hold while I transfer you.
  escalate: true
  severity: HIGH
```

### Политика 3: Обязательный disclaimer для тем financial advice

```yaml
- id: financial_advice_disclaimer
  description: >
    Любой ответ по investment, insurance или loan products должен
    включать стандартный disclaimer.
  trigger: topic_classification
  topics:
    - investment
    - insurance
    - loan_comparison
  action: APPEND
  disclaimer: >
    Disclaimer: This information is for general guidance only and does not
    constitute personal financial advice. Please consult with a qualified
    financial advisor for decisions regarding your specific situation.
  severity: MEDIUM
```

---

## 7. Протокол secrets management

1. **Без secrets в коде или environment variables** — все secrets запрашиваются из Vault при старте сервиса или на каждый запрос (для LLM API keys).
2. **Short-lived leases** — Vault выдает secrets с TTL 1 час; сервисы продлевают lease или получают заново при необходимости.
3. **Rotation policy** — LLM API keys ротируются каждые 90 дней; database credentials каждые 30 дней.
4. **Audit trail** — каждый доступ к secret логируется в audit backend Vault.
5. **Least privilege** — у каждого сервиса уникальная Vault role с доступом только к нужным secrets.
6. **Emergency revocation** — администратор Vault может отозвать все leases сервиса в случае компрометации.
