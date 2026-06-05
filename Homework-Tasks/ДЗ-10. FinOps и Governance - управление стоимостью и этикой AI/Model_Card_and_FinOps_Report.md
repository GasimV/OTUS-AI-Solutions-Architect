# Model Card и FinOps-отчет для Banking Customer Support RAG Assistant

## 0. Контекст и допущения

Документ подготовлен для системы **Banking Customer Support RAG Assistant**. Система помогает клиентам и операторам контакт-центра банка отвечать на вопросы о продуктах, тарифах, правилах обслуживания и процедурах эскалации. Архитектурно это RAG-система: пользовательский запрос проходит через API Gateway, security/guardrail layer, retrieval по векторной базе знаний, генерацию ответа через LLM provider и аудит решения.

Финансовый сценарий для FinOps-части: месячный бюджет на AI-инфраструктуру был **1 000 000 руб.**, фактический счет составил **1 500 000 руб.**, то есть превышение бюджета на **50%**. Цель плана - вернуть run-rate ниже бюджета без недопустимой потери качества, безопасности и регуляторной контролируемости.

## 1. Governance: Model Card

### 1.1. Model Details

| Поле | Значение |
|---|---|
| Название системы | Banking Customer Support RAG Assistant |
| Тип модели | Retrieval-Augmented Generation assistant для клиентской поддержки |
| Базовая LLM | Внешний LLM provider, например Azure OpenAI, либо self-hosted Llama-3-70B INT4 для private deployment |
| Retrieval | Векторная база знаний по банковским документам, тарифам, продуктам, FAQ и процедурам |
| Embeddings | Sentence-transformer/managed embedding model для индексации документов и пользовательских запросов |
| Языки | Основной: русский. Дополнительно: английский для части технической и продуктовой документации |
| Версия карточки | v1.0 |
| Дата | 2026-04-29 |
| Владельцы | Product Owner клиентской поддержки, ML Lead, Security Officer, Compliance Officer, FinOps Owner |

Карточка построена по логике Model Cards: документация должна описывать модель, назначение, ограничения, данные, параметры обучения/эксплуатации и результаты оценки качества. Такой подход согласуется с рекомендациями Hugging Face по Model Cards и работой Mitchell et al. о прозрачной отчетности моделей.

### 1.2. Intended Use

**Разрешенные сценарии использования:**

- Ответы на вопросы клиентов о банковских продуктах, тарифах, лимитах, комиссиях, графике обслуживания, статусах типовых процессов и публичных правилах.
- Помощь операторам контакт-центра: подготовка черновика ответа, поиск релевантных документов, краткое резюме процедуры.
- Классификация обращений для маршрутизации: продукт, срочность, необходимость эскалации, наличие жалобы.
- Генерация ответа только на основе разрешенных фрагментов базы знаний с обязательной ссылкой на источники или внутренние document IDs.
- Эскалация на человека при неопределенности, конфликте источников, запросах на финансовое решение или признаках жалобы.

**Запрещенные сценарии использования:**

- Автоматическое одобрение кредита, изменение лимитов, блокировка/разблокировка счетов, проведение платежей или иных транзакций.
- Персонализированная финансовая, инвестиционная, налоговая или юридическая рекомендация.
- Использование модели как единственного источника истины без проверки retrieved-документов.
- Обработка чувствительных PII в открытом prompt без предварительной маскировки.
- Использование для скоринга, сегментации клиентов по чувствительным признакам или принятия решений, влияющих на доступ к финансовым услугам.

### 1.3. Limitations

| Ограничение | Риск | Контроль |
|---|---|---|
| Модель может галлюцинировать при неполном retrieval | Неверная консультация клиента | Ответ допускается только при достаточной retrieval confidence; иначе эскалация |
| База знаний может устареть | Ответ по старому тарифу или процедуре | Freshness SLA, версия документа в ответе, инвалидация индекса после обновления |
| LLM обучалась на широких публичных данных | Возможны стереотипы, нерелевантные обобщения | Output guardrails, fairness-тесты, запрет чувствительных признаков в policy decisions |
| Многоязычные запросы могут иметь разное качество | Дискриминация по языку коммуникации | Отдельная оценка качества RU/EN и fallback на оператора |
| Semantic cache может вернуть похожий, но не идентичный ответ | Ошибка в персонализированном или контекстном вопросе | Кэш отключается для персональных, стохастических и tool-use запросов; высокий similarity threshold |
| Система зависит от внешнего LLM provider | Риск приватности, доступности и стоимости | PII redaction, private endpoint/VPN, fallback model, cost caps |

### 1.4. Training Data и Bias Analysis

Система не должна дообучаться напрямую на сырых клиентских диалогах без отдельного согласования с privacy/compliance. В production-режиме основной механизм актуализации знаний - retrieval из управляемой базы документов.

**Источники данных:**

| Источник | Использование | Риски bias |
|---|---|---|
| Публичные и внутренние банковские FAQ | Retrieval context | Документы могут быть написаны сложным юридическим языком, хуже понятным части клиентов |
| Тарифы, продуктовые условия, регламенты | Retrieval context | Разные продукты могут быть описаны с разной полнотой, что влияет на качество ответов |
| Процедуры контакт-центра | Retrieval context и маршрутизация | Исторические процедуры могут отражать устаревшие операционные практики |
| Обезличенные логи обращений | Offline evaluation, тестовые наборы | Исторические паттерны поддержки могут содержать неравномерность по регионам, языку, каналам |
| Synthetic QA sets | Regression и safety testing | Синтетика может не покрывать реальные формулировки уязвимых групп |
| Базовая LLM | Генерация текста | Публичные pretraining data неизвестны полностью и могут содержать социальные, культурные и языковые смещения |

**Ключевые bias-риски:**

- **Языковой bias:** ответы на русском могут быть лучше, чем на английском или смешанных RU/EN запросах.
- **Региональный bias:** правила обслуживания и доступность продуктов могут отличаться по региону, а модель может обобщать федеральную информацию.
- **Digital literacy bias:** клиенты с простыми, неполными или эмоциональными формулировками могут получать менее точные ответы.
- **Product bias:** популярные продукты представлены в документах лучше, чем редкие продукты, премиальные пакеты или архивные тарифы.
- **Channel bias:** качество для операторского desktop-интерфейса может быть выше, чем для мобильного chat widget, из-за большего контекста.
- **Historical support bias:** если использовать прошлые диалоги для fine-tuning, модель может перенять некорректный тон, отказы или неравномерное качество обслуживания.

**Митигирующие меры:**

- Не использовать чувствительные признаки клиента для генерации решения, маршрутизации или приоритизации, кроме случаев явно требуемой регуляторной проверки.
- Хранить в тестовом наборе страты по языку, региону, каналу, типу продукта, длине запроса и уровню сложности формулировки.
- Проверять retrieval coverage по каждому продукту и региону.
- Требовать citations/document IDs для ответов.
- Включать human review для новых интентов, жалоб и сценариев с финансовым ущербом.
- Запретить fine-tuning на сырых диалогах без PII removal, consent/legal basis и fairness-аудита.

### 1.5. Evaluation Metrics

| Категория | Метрика | Целевое значение |
|---|---|---|
| Retrieval quality | Recall@5 по golden QA | >= 0.90 |
| Retrieval quality | MRR@10 | >= 0.80 |
| Generation quality | Faithfulness, например Ragas | >= 0.85 |
| Generation quality | Answer Relevancy | >= 0.90 |
| Safety | PII leakage rate | 0% |
| Safety | Toxicity / inappropriate tone | < 0.1% |
| Safety | Unauthorized action suggestion rate | 0% |
| Operations | Escalation precision | >= 0.90 |
| Operations | Escalation recall для risk-intents | >= 0.95 |
| UX | User helpfulness score | >= 4.2 / 5 |
| Cost | Cost per resolved request | <= 12 руб. |

### 1.6. Fairness Metrics

Для RAG-системы справедливость измеряется не только демографическими признаками, но и доступностью одинаково качественной поддержки для разных групп обращений. В продуктивной системе не нужно хранить лишние чувствительные атрибуты. Fairness-оценка строится на разрешенных прокси-стратах: язык, регион обслуживания, канал, продукт, тип клиента, сложность запроса, наличие эмоциональной жалобы.

| Метрика | Как измеряется | Порог |
|---|---|---|
| Quality parity | Разница Faithfulness между стратами | <= 5 п.п. |
| Retrieval parity | Разница Recall@5 между продуктами/регионами | <= 5 п.п. |
| Refusal parity | Доля отказов/блокировок по стратам | Разница <= 3 п.п. после нормализации по risk-intent |
| Escalation parity | Доля эскалаций на человека по стратам | Разница <= 5 п.п. после нормализации по сложности |
| Latency parity | p95 latency по каналам web/mobile/operator | Разница <= 20% |
| Helpfulness parity | Средняя оценка полезности по каналам и языкам | Разница <= 0.3 балла |
| Citation coverage parity | Доля ответов с валидной ссылкой на источник | >= 98% во всех стратах |
| Complaint handling parity | Recall эскалации жалоб | >= 0.95 во всех стратах |

Если порог нарушен, релиз блокируется или переводится в canary с ограниченным трафиком. Для критичных нарушений применяется rollback prompt/model/retrieval index.

### 1.7. Auditability и Governance Controls

Каждый AI-ответ должен иметь воспроизводимый audit trail:

- `request_id`, `trace_id`, timestamp, канал, tenant/region.
- Версия prompt template, model version, retrieval index version, embedding model version.
- Хэши prompt и ответа; PII в логах хранится только в редактированном виде.
- Список retrieved document IDs, scores, версии документов, дата публикации.
- Guardrail decisions: input block, output block, PII redaction, policy violation, escalation reason.
- Token usage: input tokens, output tokens, cached tokens, embedding calls.
- Cost allocation tags: product, channel, environment, team, model tier.
- Human override: кто изменил ответ, причина, итоговая категория.

Retention: сырые redacted-логи - 30 дней, агрегированные метрики - 13 месяцев, audit records для спорных обращений - согласно политике банка и требованиям регулятора.

## 2. FinOps: анализ превышения бюджета

### 2.1. Структура счета

| Категория | Бюджет, руб. | Факт, руб. | Отклонение | Комментарий |
|---|---:|---:|---:|---|
| GPU inference | 550 000 | 900 000 | +350 000 | Низкая утилизация GPU, always-on capacity, часть dev/stage не выключалась |
| LLM API tokens | 180 000 | 260 000 | +80 000 | Длинные prompts, отсутствие model routing, слабый cache hit ratio |
| Vector DB / embeddings | 90 000 | 110 000 | +20 000 | Частая переиндексация, хранение устаревших embeddings |
| Logs, traces, object storage | 80 000 | 150 000 | +70 000 | DEBUG-логи и полные payloads хранились дольше нужного |
| Data processing | 60 000 | 50 000 | -10 000 | В пределах плана |
| Network / cross-region | 40 000 | 30 000 | -10 000 | В пределах плана |
| Прочее | 0 | 0 | 0 | - |
| **Итого** | **1 000 000** | **1 500 000** | **+500 000** | **+50% к бюджету** |

### 2.2. Типичные причины перерасхода

1. **Забытые GPU-инстансы в dev/stage.** GPU-ноды работали ночью и в выходные, хотя нагрузка была только тестовой.
2. **Always-on inference capacity без учета профиля нагрузки.** Минимальный пул был завышен, а async-задачи использовали тот же дорогой класс GPU, что и синхронный пользовательский путь.
3. **Слишком мощная модель для простых запросов.** Простые FAQ и классификация интентов отправлялись в Llama-3-70B/GPT-4-class модель вместо small/medium модели или retrieval-only ответа.
4. **Недостаточный caching.** Exact cache и semantic cache не покрывали повторяющиеся вопросы, cache hit ratio был ниже целевого.
5. **Длинные prompts и избыточный retrieval context.** В prompt попадало слишком много фрагментов документов, включая нерелевантные chunk'и.
6. **Неэффективное хранение логов.** Полные prompts, responses, traces и DEBUG-события хранились в горячем хранилище без TTL и sampling.
7. **Переиндексация всего корпуса вместо инкрементального обновления.** Каждое изменение документа запускало дорогую генерацию embeddings для большого корпуса.
8. **Отсутствие showback/chargeback.** Команды не видели cost per product/channel/model и не управляли потреблением.
9. **Нет budget guardrails.** Не было автоматических алертов при аномальном росте токенов, GPU-hours или storage.

## 3. Cost Optimization Plan

План построен в логике FinOps Framework: **Inform** - обеспечить видимость затрат, **Optimize** - снизить unit cost, **Operate** - закрепить управление через политики и автоматизацию.

### 3.1. Quick Wins: 0-2 недели

| Мера | Действие | Ожидаемый эффект | Риск качества |
|---|---|---:|---|
| Автоостановка dev/stage GPU | Scheduler: выключать GPU-ноды вне 08:00-21:00, scale-to-zero для sandbox | 8-12% | Низкий |
| Spot/preemptible для batch и async | Перевести embedding jobs, offline eval и heavy async workers на spot | 5-10% | Низкий при retry |
| TTL для логов | DEBUG - 7 дней, redacted request logs - 30 дней, aggregates - 13 месяцев | 3-6% | Низкий |
| Sampling traces | 100% для ошибок и risk-intents, 5-10% для успешных типовых запросов | 2-4% | Низкий |
| Token budget alerts | Алерты по tokens/hour, cost/day, cost/request, cache hit ratio | Предотвращает повторение | Нет |
| Prompt trimming | Ограничить retrieval до top-k=3-5, убрать дубли и boilerplate | 5-8% | Средний, нужен regression test |

### 3.2. Среднесрочные меры: 2-6 недель

| Мера | Действие | Ожидаемый эффект | Риск качества |
|---|---|---:|---|
| Model routing | Простые FAQ -> small LLM или retrieval-only; сложные и risk-intents -> 70B/GPT-4-class | 15-25% | Средний |
| Model distillation | Обучить компактную модель для intent classification, FAQ drafting и summarization | 10-20% | Средний |
| Exact + semantic cache | L2 exact cache для deterministic prompts, L3 semantic cache с cosine >= 0.95 | 10-25% | Средний, нужен false-positive контроль |
| vLLM + continuous batching | Для self-hosted Llama использовать PagedAttention и dynamic batching | 20-40% GPU cost | Низкий/средний |
| INT4 quantization | AWQ/GPTQ для Llama-3-70B, контроль качества на golden set | 30-50% GPU memory/cost | Средний |
| Инкрементальные embeddings | Пересчитывать только измененные chunks, хранить hash документа | 3-8% | Низкий |
| Storage lifecycle | Hot -> warm -> cold, компрессия Parquet/ZSTD для audit и eval datasets | 5-10% storage | Низкий |

### 3.3. Долгосрочные меры: 6-12 недель

| Мера | Действие | Ожидаемый эффект | Риск качества |
|---|---|---:|---|
| Reserved capacity / committed use | Закупить baseline GPU/API capacity после стабилизации профиля нагрузки | 10-20% | Низкий |
| Multi-model serving | Один inference pool обслуживает small/medium/large модели с isolation и quotas | 10-15% | Средний |
| Unit economics dashboard | Cost per resolved request, cost per channel, cost per model, margin impact | Управляемость | Нет |
| Policy-as-code | OPA/Kyverno правила: обязательные tags, max TTL, запрет untagged GPU | Предотвращает drift | Нет |
| Quality-cost release gate | Релиз проходит только при соблюдении качества и cost/request budget | Предотвращает регрессии | Нет |

## 4. Целевая архитектура оптимизации

### 4.1. Model Routing

| Тип запроса | Обработка | Модель |
|---|---|---|
| Повторяющийся FAQ, deterministic answer | Exact cache или retrieval-only template | Без LLM или small model |
| Простая классификация интента | Distilled classifier | Small model |
| Стандартный вопрос по тарифу/процедуре | RAG + medium LLM | Medium model |
| Жалоба, спорная ситуация, регуляторный риск | RAG + large LLM + human review | Large model |
| Запрос на транзакцию или персональное решение | Отказ от автоматизации + эскалация | Human |

Правило: дорогая модель используется только там, где она дает измеримую прибавку качества или снижает операционный риск. Для каждого маршрута фиксируются метрики `quality`, `latency`, `cost/request`, `escalation rate`.

### 4.2. TTL и data lifecycle

| Данные | Хранилище | TTL / lifecycle |
|---|---|---|
| Raw prompts с PII | Не хранить в чистом виде | Только redacted logs |
| Redacted request/response logs | Hot object storage | 30 дней |
| Full traces | Observability backend | 7 дней, 100% errors/risk, sampling для success |
| Aggregated metrics | TSDB / warehouse | 13 месяцев |
| Retrieved document snapshots для audit | Object storage | По политике банка, минимум для спорных обращений |
| Embeddings старых версий индекса | Warm/cold storage | 30-90 дней после релиза нового индекса |
| Evaluation datasets | Versioned object storage | Долгосрочно, с доступом по RBAC |

### 4.3. Spot Instances

Spot/preemptible применять для:

- offline evaluation;
- batch embeddings;
- nightly regression tests;
- async summarization;
- backfill jobs;
- non-production inference.

Не применять spot для:

- синхронного клиентского пути с p99 SLA;
- risk-intents, где потеря запроса недопустима;
- минимального hot pool, который держит модель загруженной в VRAM.

### 4.4. Model Distillation

Кандидаты на distillation:

- intent classifier;
- routing classifier: `cache/retrieval-only/small/medium/large/human`;
- short answer drafting для FAQ;
- summarization of retrieved chunks;
- guardrail pre-check.

Критерий приемки distilled-модели:

- качество не ниже 98% от teacher-модели на low-risk сценариях;
- нет ухудшения fairness parity больше 2 п.п.;
- cost per request ниже минимум на 50%;
- все ошибки high-risk класса уходят в large model или human escalation.

## 5. KPI и контроль результата

| KPI | Текущее значение | Цель через 6 недель |
|---|---:|---:|
| Monthly AI cloud cost | 1 500 000 руб. | <= 950 000 руб. |
| Cost per resolved request | 18 руб. | <= 12 руб. |
| GPU utilization | 25-35% | >= 65% |
| Cache hit ratio | 20% | >= 50% |
| Доля запросов на large model | 100% для LLM path | <= 35% |
| Средний prompt tokens/request | 4 000 | <= 2 500 |
| Storage hot data volume | 100% логов | <= 30% логов, остальное warm/cold |
| Faithfulness | 0.85+ | Не ниже baseline - 1 п.п. |
| PII leakage rate | 0% | 0% |
| Fairness parity gap | Не более 5 п.п. | Не более 5 п.п. |

## 6. Приоритизация действий

1. Включить cost allocation tags, daily budget alerts и dashboard `cost/request` по channel/model/environment.
2. Остановить неиспользуемые GPU в dev/stage, ввести scheduler и TTL для sandbox-ресурсов.
3. Ввести storage lifecycle: TTL для DEBUG/full traces, warm/cold tiers для архивов.
4. Сократить prompt context и включить regression-тесты качества RAG.
5. Запустить exact cache и semantic cache для deterministic low-risk запросов.
6. Внедрить model routing: small/medium/large/human.
7. Перевести batch/async workloads на spot/preemptible.
8. Для self-hosted inference включить vLLM, continuous batching и INT4 quantization после quality gate.
9. Подготовить distilled-модели для классификации интентов, routing и FAQ.
10. Закрепить правила через policy-as-code и release gate: релиз не проходит при нарушении качества, safety или unit economics.

## 7. Баланс цены и качества

Оптимизация не должна сводиться к механическому удешевлению модели. Для банковского ассистента цена ошибки выше экономии на одном запросе. Поэтому стратегия такая:

- низкорисковые и повторяющиеся вопросы обслуживаются кэшем, retrieval-only шаблонами или small model;
- среднерисковые вопросы идут через RAG + medium model;
- высокорисковые, жалобы и неоднозначные ситуации идут через large model и/или human review;
- каждый перевод на более дешевый маршрут проходит offline golden set, canary и мониторинг fairness parity;
- экономия принимается только если Faithfulness, PII leakage, escalation recall и complaint handling parity остаются в целевых пределах.

Ожидаемый эффект после внедрения quick wins и среднесрочных мер: снижение месячного счета с **1 500 000 руб.** до **850 000-950 000 руб.** без ухудшения ключевых safety и fairness-метрик.

## 8. Источники

- Hugging Face Hub Documentation, "Model Cards": https://huggingface.co/docs/hub/model-cards
- Mitchell et al., "Model Cards for Model Reporting", arXiv:1810.03993: https://arxiv.org/abs/1810.03993
- FinOps Foundation, "FinOps Framework": https://www.finops.org/framework/
