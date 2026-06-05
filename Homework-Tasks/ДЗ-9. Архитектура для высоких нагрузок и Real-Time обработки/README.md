# ДЗ-9. Архитектура для высоких нагрузок и Real-Time обработки

## Исходные требования

| Параметр | Значение |
|---|---|
| Целевой RPS | **10 000** |
| Latency p99 | **< 200 ms** (синхронный путь) |
| Профиль нагрузки | LLM inference (микс коротких и длинных prompt'ов) |
| Пользователи | глобальные (multi-region) |

Решение описано в виде C4 Container-диаграммы — см. [high_load_architecture.puml](high_load_architecture.puml).

---

## 1. Caching Strategy

Кэш реализован **как многоуровневая пирамида** — каждый слой отсекает максимум трафика до того, как он доберётся до дорогого GPU-инференса.

| Уровень | Где живёт | Что кэшируется | Ожидаемый hit ratio |
|---|---|---|---|
| L1 — Edge / CDN | Cloudflare PoP | статика, публичные GET, идемпотентные ответы | ~15 % трафика |
| L2 — Exact Cache | Redis Cluster (региональный) | `SHA256(prompt + model + params) → response` | ~25 % |
| L3 — Semantic Cache | Redis Vector / Milvus | эмбеддинги + ответы для «похожих» запросов | ~20 % |
| L4 — KV-cache vLLM | GPU memory | префиксы токенов внутри vLLM | внутренняя оптимизация |

Итого **до 60 %** запросов не доходят до модели — при целевых 10 k RPS только ~4 k RPS реально нагружают inference-пулы.

### Алгоритм Semantic Cache

```
on request(prompt, params):
    key = sha256(normalize(prompt) + params)

    # 1. Exact lookup — O(1), ~1 ms
    if hit = redis.get(key):
        return hit

    # 2. Embed — ~5 ms на bge-small (CPU)
    q_vec = embedding_model.encode(prompt)

    # 3. ANN search по HNSW-индексу — ~3 ms
    candidates = vector_store.search(q_vec, top_k=5)

    # 4. Similarity + policy gate
    best = max(candidates, key=lambda c: c.cosine)
    if best.cosine >= 0.95 and best.model == params.model \
       and not params.stochastic:        # temp=0, без tool-use
        metrics.inc("semantic_cache.hit")
        return best.response

    # 5. MISS — дергаем LLM
    response = llm.generate(prompt, params)

    # 6. Write-through в оба кэша
    redis.setex(key, TTL_1H, response)
    vector_store.upsert(q_vec, response, ttl=TTL_24H)
    return response
```

Ключевые защиты от «ложных попаданий»:
- **порог cosine ≥ 0.95** подобран эмпирически (golden-set из 1k пар); ниже — слишком много ложных хитов;
- кэш **отключается для стохастических запросов** (`temperature > 0`, tool-use, function calling, streaming с personalization);
- **namespace по tenant_id + model_version** — исключаем утечки между клиентами и между версиями модели;
- TTL скользящий (sliding), инвалидация при релизе новой версии модели через `model_version` в ключе.

---

## 2. Scaling — KEDA + горизонтальное масштабирование inference

Обычный HPA по CPU бесполезен: GPU-поды не нагружают CPU, а метрика «занятости GPU» плохо коррелирует с реальной очередью. Поэтому масштабируемся **по бизнес-метрике — глубине очереди запросов**.

### Синхронный пул

```yaml
apiVersion: keda.sh/v1alpha1
kind: ScaledObject
metadata:
  name: inference-sync
spec:
  scaleTargetRef:
    name: inference-sync
  minReplicaCount: 6        # баз-мощность, чтобы держать "тёплый" пул
  maxReplicaCount: 80
  cooldownPeriod: 120
  pollingInterval: 10
  triggers:
    - type: prometheus
      metadata:
        serverAddress: http://prometheus:9090
        metricName: gateway_request_queue_depth
        query: |
          sum(rate(gateway_request_queue_depth{path="/v1/chat"}[30s]))
        threshold: "30"     # >30 ждущих req/pod → scale out
```

### Асинхронный пул — scale по lag Kafka

```yaml
triggers:
  - type: kafka
    metadata:
      bootstrapServers: kafka:9092
      consumerGroup: heavy-inference
      topic: heavy-inference
      lagThreshold: "50"
```

Почему именно так:
- **queue depth / consumer lag** — прямая метрика давления, реагирует за секунды;
- **минимум 6 реплик** в sync-пуле исключает cold-start (загрузка 70B весов в VRAM = десятки секунд) — scale-to-zero недопустим для p99 < 200 ms;
- **cooldown 120 s** гасит флаппинг при пиках;
- на кластере включён **cluster-autoscaler + Karpenter** для заказа GPU-нод под новые поды.

---

## 3. Async Processing — очередь для тяжёлых запросов

Запросы с `max_tokens > 1024`, batch-inference или multi-step агентных цепочек **не могут** уложиться в 200 ms. Смешивать их с лёгким трафиком в одном пуле — значит «отравить» p99.

**Решение — разделение путей на уровне Request Router:**

```
if estimate_tokens(prompt) + params.max_tokens > 1024
   or params.mode == "agentic"
   or params.stream == false and params.timeout > 5s:
       enqueue(kafka, "heavy-inference", payload)
       return 202 Accepted { job_id, status_url, ws_channel }
else:
       return sync_inference(payload)       # p99 < 200 ms
```

Компоненты async-пути:
1. **Kafka / NATS JetStream** — партиционирование по `tenant_id`, гарантирует fairness между клиентами и at-least-once доставку;
2. **Async Workers** — отдельный Deployment, свой KEDA-скейлер по lag;
3. **Result Store** — маленькие ответы в Redis, крупные (> 200 KB) в S3 + presigned URL;
4. **Notifier** — WebSocket / webhook, чтобы клиент получил ответ push'ом, а не long-polling'ом.

Результат: sync-путь **изолирован** от длинных задач, его p99 стабилен; heavy-трафик буферизуется и сглаживает пики.

---

## 4. Global Distribution (Geo-DNS + Edge)

Пользователи по всему миру → один регион = RTT 200+ ms только на сеть. Стратегия:

| Слой | Технология | Задача |
|---|---|---|
| Geo-DNS | Route 53 latency-based routing | направляет в ближайший region (eu-west-1 / us-east-1 / ap-southeast-1) |
| Edge PoP | Cloudflare / CloudFront | TLS termination, 0-RTT, HTTP/3 |
| Edge Workers | Cloudflare Workers / Lambda@Edge | JWT-валидация, rate limiting, **lookup Exact Cache через KV** — ответ отдаётся прямо с edge за ~10 ms |
| Regional Cluster | K8s + vLLM | full inference только для cache miss |
| Cross-region sync | Redis Active-Active (CRDT) | репликация «горячего» кэша между регионами |

Эффект: для ~40 % запросов пользователь вообще не доходит до региона — ответ отдаётся с ближайшего PoP. Остальные ходят в ближайший регион по кратчайшему сетевому пути.

---

## 5. Как решение закрывает требования

| Требование | Механизм | Контрибуция в Latency / Throughput |
|---|---|---|
| 10 000 RPS | Edge + L2/L3 caches снимают ~60 % | на модель реально идёт ~4 000 RPS |
| p99 < 200 ms (sync) | hot pool `min=6`, разделение sync/async, KEDA по queue depth | нет cold-start, нет «отравления» длинными запросами |
| Global latency | Geo-DNS + Edge Workers + cross-region Redis | RTT ≤ 30 ms до ближайшего PoP |
| Эластичность | KEDA по Prometheus/Kafka lag + Karpenter | автоматический scale-out за ~30 с |
| Защита от спайков | Kafka-буфер на async-пути | нет каскадных отказов GPU-подов |
| Наблюдаемость | Prometheus + OpenTelemetry | SLO-мониторинг p99, hit ratio, queue depth |

---

## 6. Риски и компромиссы

- **Semantic cache false positives** — митигируется порогом cosine ≥ 0.95, namespace'ом по модели и отключением для стохастических запросов. Golden-set для регрессионного тестирования.
- **Stampede при инвалидации кэша** (релиз модели) — single-flight + probabilistic early expiration (XFetch).
- **Стоимость GPU «горячего резерва»** — `min=6` подов всегда платные; компромисс ради отсутствия cold-start. Можно удешевить через spot-инстансы для async-пула.
- **Cross-region консистентность кэша** — eventual consistency ок, т.к. cache-miss всегда деградирует к модели.
