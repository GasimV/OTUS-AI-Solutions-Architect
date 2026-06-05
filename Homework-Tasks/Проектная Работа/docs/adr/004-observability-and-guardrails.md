# ADR-004: Observability and guardrails

## Status

Accepted.

## Context

Для промышленной эксплуатации нужны трассировка запросов, метрики latency, throughput, tokens/sec и проверки безопасности.

## Decision

MVP включает:

- Prometheus scrape `/metrics` API и Qdrant.
- Grafana datasource provisioning.
- OpenTelemetry dependencies в API container для дальнейшего OTLP export.
- Input guardrails: prompt injection patterns, PII redaction.
- Output guardrails: PII redaction, removal of `<think>...</think>`, citation allow-list.
- Unit-тесты на security boundary.

В текущем Compose Prometheus собирает API и Qdrant. vLLM публикует собственный `/metrics`, но его scrape job оставлен как production next step, чтобы не усложнять MVP-конфигурацию. Production deployment diagram фиксирует сбор vLLM tokens/sec, latency и GPU metrics.

## Trade-off

- Плюсы: метрики и guardrails встроены в pipeline, а не добавлены внешним proxy.
- Минусы: regex guardrails не заменяют полноценный DLP/classifier.
- Отклонено: проверять безопасность только в prompt. Prompt не является надежной границей доступа.

## Consequences

Следующий production шаг: добавить OPA/Keycloak для централизованной авторизации, immutable audit log, OTLP collector, vLLM metrics scraping, prompt regression suite и LLM-as-a-Judge тесты на внутренней judge-модели.
