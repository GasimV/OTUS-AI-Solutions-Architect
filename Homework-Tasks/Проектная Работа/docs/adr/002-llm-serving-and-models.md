# ADR-002: LLM serving and Russian-capable models

## Status

Accepted.

## Context

Нужен локальный high-throughput inference server, OpenAI-compatible API для простого клиента и поддержка consumer GPU. На машине доступна NVIDIA GeForce RTX 5080 Laptop GPU с 16 GB dedicated VRAM.

## Decision

Используем `vllm/vllm-openai:latest` как LLM serving engine для MVP. Основная модель для демо: `Qwen/Qwen3-8B-AWQ`, размещенная заранее в `./models/qwen3-awq` и опубликованная vLLM как served model `local-qwen`.

В production-релизе образ должен быть закреплен digest/tag в закрытом registry. `latest` допустим только для учебного MVP и локальной проверки.

Настройки:

- AWQ 4-bit quantization для попадания в 16 GB VRAM.
- `--gpu-memory-utilization 0.88` для ограничения риска OOM.
- `--max-model-len 8192` как практичный MVP-контекст.
- `--kv-cache-dtype auto` и `--enable-prefix-caching` для оптимизации KV-cache.
- `HF_HUB_OFFLINE=1` и `TRANSFORMERS_OFFLINE=1`, чтобы runtime не требовал внешнего интернета.

## Alternatives

- SGLang: сильная альтернатива для structured generation и throughput, но vLLM проще как OpenAI-compatible server и хорошо документирован для Docker.
- TGI: стабилен для Hugging Face моделей, но vLLM обычно удобнее для AWQ и continuous batching.
- GGUF/llama.cpp: хорош для CPU/offload, но не основной выбор для high-throughput GPU serving.

## Model trade-off

- `Qwen3-8B-AWQ`: реалистичен для 16 GB VRAM, поддерживает многоязычность и agent/tool сценарии.
- `Qwen3-30B/32B` или DeepSeek distill 32B: лучше качество reasoning, но для данной laptop GPU потребуют CPU offloading, меньший context или внешний GPU-сервер.
- T-lite/Saiga: полезны как российско-ориентированные альтернативы для демо, но Qwen3 дает более широкий open-source baseline.

## Consequences

В air-gapped контуре образы Docker и модельные веса должны быть заранее загружены во внутренний registry/artifact storage. В `.env` можно закрепить конкретный digest `VLLM_IMAGE` для воспроизводимого промышленного релиза.

API содержит `ExtractiveLlmFallback` только для smoke-тестов и деградации при транспортной ошибке vLLM. Финальная проверка выполняется командой `python scripts/e2e_smoke.py --require-vllm`, которая падает, если был использован fallback.
