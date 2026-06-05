# PlantUML diagrams

Исходники диаграмм:

- `01-c4-context.puml` - C4 Level 1 Context.
- `02-c4-container.puml` - C4 Level 2 Container.
- `03-c4-component-agent.puml` - C4 Level 3 Component.
- `04-c4-code.puml` - C4 Level 4 Code/Class.
- `05-deployment-local-compose.puml` - Deployment фактического MVP на локальном Docker Compose.
- `06-deployment-prod-k8s.puml` - Deployment целевого production-развертывания в on-prem Kubernetes GPU-кластере.
- `07-sequence-query.puml` - Sequence.
- `08-data-flow.puml` - Data Flow.
- `09-er.puml` - ER.
- `10-c4-component-fastapi-backend.puml` - C4 Level 3 Component для FastAPI Backend.
- `11-c4-component-guardrails.puml` - C4 Level 3 Component для Guardrails service / policy module.

Рендер всех PNG:

```powershell
plantuml -tpng -o out docs/plantuml/*.puml
```

Рендер SVG:

```powershell
plantuml -tsvg -o out docs/plantuml/*.puml
```

Сгенерированные PNG лежат в `docs/plantuml/out`.
