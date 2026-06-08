# AI Architecture Delivery Documentation Checklist

<a id="contents"></a>

## Contents

1. [Core Idea](#1-core-idea)
2. [Market Research Input](#2-market-research-input)
3. [Delivery Flow](#3-delivery-flow)
4. [PoC Purpose and Output](#4-poc-purpose-and-output)
5. [Documentation by Stage](#5-documentation-by-stage)
6. [Architecture Deliverables](#6-architecture-deliverables)
7. [Deployment and Operations](#7-deployment-and-operations)
8. [Codebase Implementation](#8-codebase-implementation)
9. [Prototype Pitch and Product Evolution](#9-prototype-pitch-and-product-evolution)
10. [AI Architecture Lecture Additions](#10-ai-architecture-lecture-additions)
11. [Active Recall Checklist](#11-active-recall-checklist)

---

## 1. Core Idea

The architecture delivery path starts from **market research input**, then moves to **experimental validation**, **customer-visible prototype**, **agreed MVP scope**, **pilot validation**, and finally **production deployment**.

The architect is responsible for proving that the architecture is feasible, documenting why decisions were made, and handing over enough product, architecture, operations, and implementation material for the next delivery stage.

```text
Market Research Input -> PoC -> Prototype -> MVP Scope -> MVP Delivery -> Pilot -> Production
```

[Back to Contents](#contents)

---

## 2. Market Research Input

Before the PoC and before the architect's technical work, there should be **market research work** for opportunity discovery and customer needs identification.

This market research activity is out of scope for this architecture checklist, but it is a critical upstream input. It is important, necessary, valuable, and requires real investment of time and resources to produce useful results.

Market research should be documented and converted into precise technical specifications that the architect can build upon.

Typical outputs that should reach the architect:

- Opportunity discovery findings.
- Customer needs and pain points.
- Target customer segments.
- Market and competitor insights.
- Problem validation evidence.
- Customer interviews or discovery notes.
- Prioritized product opportunities.
- Initial product requirements.
- Business constraints.
- Success criteria.
- Technical specification inputs.

If this work is skipped, the team risks building the wrong product, solving the wrong problem, or developing something that customers do not actually want.

[Back to Contents](#contents)

---

## 3. Delivery Flow

```text
Market Research / Opportunity Discovery
  -> Customer Needs Identification
  -> Technical Specification Inputs
  -> PoC
  -> Prototype
  -> Customer Engagement
  -> MVP Scope Definition
  -> MVP Delivery
  -> Feedback Collection and Product Improvement
  -> Pilot / Pre-Production Battlefield Test
  -> Production Deployment
```

### Contract Model

| Stage | Contract / Commercial Model |
|-------|-----------------------------|
| Market Research / Discovery | Business cost; upstream product/business activity before technical architecture work |
| PoC | Part of the business cost/work before coming up with the prototype to be presented to customers |
| Prototype | Freely self-developed by the architect and pitched to the customer |
| MVP | Fixed Price after agreed scope |
| Production | Fixed Price or production support model after agreed scope and operating responsibilities |

[Back to Contents](#contents)

---

## 4. PoC Purpose and Output

The **PoC** is an experimental stage where the architect validates architectural assumptions, tests hypotheses, and evaluates preliminary options.

Typical PoC evaluation areas:

- Technology stack.
- Methodologies.
- Design patterns.
- Algorithms.
- AI/ML model approach.
- RAG, GraphRAG, agentic, or hybrid AI patterns.
- Integration approaches.
- Infrastructure choices.
- Security and governance constraints.
- Implementation strategies.

The goal of the PoC is to determine whether the proposed architecture can satisfy the product and project requirements.

The PoC should document selected architectural decisions with clear justifications based on:

- Real experimental results.
- Trade-offs.
- Constraints.
- Observed behavior.
- Risks and mitigations.
- Cost and resource implications.
- Data availability and quality.

After the PoC, the architect develops or assembles a working prototype that demonstrates:

- Product concept.
- Key workflows.
- Technical feasibility.
- Business value to the customer.

[Back to Contents](#contents)

---

## 5. Documentation by Stage

### I. Product and Business Documentation

Core product/business documents:

- Product Vision.
- Problem Statement.
- Business Value.
- Stakeholders.
- Personas.
- Use Cases.
- Functional Requirements (FRs).
- Non-Functional Requirements (NFRs).
- User Journeys.
- Module Descriptions.
- High-Level Business Process Flows.
- PoC Scope.
- MVP Scope.
- Contract Model.
- Success Metrics.
- Assumptions and Constraints.

Additional AI architecture items to include:

- Data sources and data ownership.
- Internal data availability.
- Data sensitivity and PII classification.
- AI safety expectations.
- Human-in-the-loop requirements.
- Evaluation criteria for AI output quality.
- Compliance, ethics, and governance expectations.
- Target SLA, SLO, and SLI assumptions.
- Cost boundaries and FinOps assumptions.

### MVP Scope Definition Inputs

MVP scope is defined after validating:

- Customer engagement.
- Customer feedback.
- Internal data availability.
- Integration possibilities.
- Refined requirements.
- Business value priorities.
- Operational constraints.
- Risk level and implementation effort.

[Back to Contents](#contents)

---

## 6. Architecture Deliverables

### II. High-Level Design (HLD)

HLD diagrams:

- C4 Context Diagram.
- C4 Container Diagram.
- High-Level Integration Diagram.
- High-Level Deployment Diagram.
- High-Level Data Architecture & Flow Diagrams.
- High-Level Sequence Diagrams for main journeys.
- End-to-End Process Overview via Activity Diagrams or Swimlanes.

Diagram file expectations:

- `.puml` source files.
- `.png` rendered diagrams.

HLD documentation:

- `architecture-overview.md`.
- ADR documents.
- ATAM or architecture trade-off analysis documents.
- Risk register.
- NFR mapping.
- Technology radar notes for major technology choices.

### III. Low-Level Design (LLD)

LLD diagrams and specs:

- C4 Component Diagrams.
- ER Diagrams.
- Sequence Diagrams.
- State Machine Diagrams.
- Activity Diagrams.
- Detailed Integration Diagrams.
- C4 Code Diagrams where useful.
- RAG / GraphRAG specification.
- Agent and multi-agent workflow specification if applicable.
- API design/spec in `.yml` OpenAPI / Swagger format.
- ADRs.

Additional LLD details for AI systems:

- Prompt templates and prompt versioning approach.
- Model selection rationale.
- Embedding model and vector database design.
- Retrieval strategy, chunking, reranking, and grounding rules.
- Evaluation datasets and quality metrics.
- Guardrails, safety filters, and escalation logic.
- Data lineage and model lineage.
- Feature store or vector index update process.
- Integration error handling and retry strategy.

[Back to Contents](#contents)

---

## 7. Deployment and Operations

### IV. Deployment / Operations Documentation

Required diagrams:

- Detailed Deployment Diagram.
- Network Diagram.
- Infrastructure Diagram.
- CI/CD Pipeline Diagram.
- Observability Diagram.
- Backup / Recovery Diagram.
- MLOps Pipeline Diagram.

Create `deployment-ops.md` with these sections:

- Deployment architecture.
- Runtime topology.
- Environment strategy: local, dev, test, staging, pre-production, production.
- CI/CD pipeline.
- MLOps pipelines:
  - Training.
  - Evaluation.
  - Retraining.
  - Model registry.
  - Rollout.
  - Rollback.
  - Shadow deployment.
  - Canary deployment.
  - Blue/green deployment.
- Active-Passive and Active-Active options for HA/DR.
- Monitoring, observability, alerting, and logging:
  - Metrics.
  - Logs.
  - Traces.
  - Dashboards.
  - Alerts.
  - Error budgets.
- Backup and restore.
- Disaster recovery:
  - RTO.
  - RPO.
  - Failover.
  - Failback.
- Runbooks.
- Incident response.
- Security operations.
- Capacity planning and sizing.
- Cost monitoring and FinOps controls.
- ADRs for deployment and operations decisions.

[Back to Contents](#contents)

---

## 8. Codebase Implementation

### V. Codebase Deliverables

Required repository artifacts:

- `README.md`.
- ADRs.
- Source code.
- AI/ML models or model loading references.
- Database files.
- Tests.
- IaC, such as Terraform configuration files.
- Seed/sample data.
- CI/CD pipeline configuration.
- Docker manifests.
- Kubernetes manifests.
- Environment configuration templates.
- API documentation.

Required `docs/` files:

- `docs/coding-standards.md`.
- `docs/local-development.md`.
- `docs/testing-strategy.md`.
- `docs/deployment-ops.md`.
- `docs/architecture-overview.md`.
- `docs/api-design.md` or OpenAPI reference.
- `docs/security-and-governance.md`.
- `docs/mlops-strategy.md` if AI/ML lifecycle is part of the system.

Testing expectations:

- Unit tests.
- Integration tests.
- Contract/API tests.
- E2E tests for main workflows.
- Load/performance tests.
- Security tests.
- AI evaluation tests for accuracy, relevance, hallucination, toxicity, bias, retrieval quality, and regression.

[Back to Contents](#contents)

---

## 9. Prototype Pitch and Product Evolution

### VI. Pitch Deck / Presentation of the Prototype

The prototype presentation should demonstrate:

- Product problem and customer pain.
- Target users and stakeholders.
- Business value.
- Main workflow.
- Architecture feasibility.
- Key PoC findings.
- Selected architectural decisions.
- Main risks and mitigations.
- MVP scope proposal.
- Delivery plan.
- Success metrics.
- Commercial model.

### Product Evolution Path

```text
Pitch Deck / Prototype Presentation
  -> Customer Engagement and MVP Scope Definition
  -> MVP Delivery
  -> Feedback Collection, Analysis, and Product Improvement
  -> Pilot / Pre-Production Battlefield Test
  -> Production Deployment
```

### Handover Options

After the MVP scope is agreed, the architect can either:

- Hand over the validated architecture, prototype, documentation, and implementation guidance to a delivery/development team.
- Continue implementing the product directly if acting as a solo engineer, entrepreneur, or member of a startup team.

[Back to Contents](#contents)

---

## 10. AI Architecture Lecture Additions

The AI Architecture lectures suggest several important artifacts that are easy to overlook in a PoC-to-production checklist.

### Requirements, Risk, and Estimation

- Requirement prioritization.
- Architecture risk register.
- Assumption log.
- Estimation model.
- Delivery plan.
- CTO challenge / architecture review preparation.

### Architecture Decision and Review Practice

- ADR index.
- ADR template.
- ATAM trade-off analysis.
- Decision traceability from requirement to architecture choice.
- Architecture governance and technical debt register.

### AI-Specific Architecture

- RAG and advanced RAG design.
- GraphRAG design.
- Agent and multi-agent architecture.
- AI integration patterns.
- Guardrails and controlled AI behavior.
- GenAI quality evaluation strategy.
- Human approval and escalation flow.
- Ethical AI and governance controls.

### Data Architecture

- Source systems.
- Data contracts.
- Data quality rules.
- Data retention.
- Data privacy.
- Batch, streaming, Lambda, or Kappa architecture choice.
- Vector index lifecycle.
- Model and data lineage.

### Security by Design

- Threat model.
- Trust boundaries.
- IAM/RBAC.
- Secrets management.
- PII handling.
- Prompt injection controls.
- Data leakage controls.
- Model abuse controls.
- Audit logging.
- Compliance mapping.

### Infrastructure and Runtime Choices

- Serverless vs Kubernetes decision.
- Hybrid/multi-cloud strategy.
- Multi-tenancy model.
- High-load and low-latency inference strategy.
- LLM sizing and inference optimization.
- Capacity planning.
- Autoscaling and quota design.

### Operations, Reliability, and Cost

- Observability architecture.
- Prometheus/Grafana or equivalent monitoring plan.
- SLA/SLO/SLI definitions.
- CI/CD and IaC strategy.
- MLOps lifecycle.
- HA/DR design.
- Deployment and rollout strategy.
- FinOps model.
- Cost allocation and budget alarms.

### API and Productization

- API-as-a-product strategy.
- OpenAPI / Swagger spec.
- Versioning and compatibility policy.
- Rate limiting.
- Developer documentation.
- Integration test plan.
- MCP or tool integration plan if relevant.

[Back to Contents](#contents)

---

## 11. Active Recall Checklist

Use these prompts to test understanding:

- What does the PoC validate before MVP scope is agreed?
- Why should market research and customer needs identification happen before PoC?
- What technical inputs should market research produce for the architect?
- Which PoC results should become ADR evidence?
- What is the difference between PoC output and prototype output?
- Which product/business documents are needed before defining MVP scope?
- Which HLD diagrams are expected before moving into detailed design?
- Which LLD artifacts are needed for RAG, GraphRAG, or agentic systems?
- What belongs in `deployment-ops.md`?
- Which AI-specific risks must be covered before pilot or production?
- How do SLA, SLO, SLI, HA, DR, and FinOps change the production architecture?
- What artifacts should be handed over to a delivery team?
- When can the architect continue as the direct product implementer instead of handing over?

### One-Sentence Recall

> The architect uses the PoC to prove architectural feasibility, the prototype to demonstrate product value, the MVP scope to define paid delivery, the pilot to validate real-world operation, and production documentation to make the system supportable, governable, secure, and scalable.

[Back to Contents](#contents)
