# AI Architecture Delivery Documentation Checklist

<a id="contents"></a>

## Contents

1. [Core Flow](#1-core-flow)
2. [Contract Model](#2-contract-model)
3. [PoC and Prototype](#3-poc-and-prototype)
4. [Main Architectural Design Documentation Package](#4-main-architectural-design-documentation-package)
5. [Codebase Implementation](#5-codebase-implementation)
6. [Pilot, MVP and Production](#6-pilot-and-production)
7. [Active Recall](#7-active-recall)

---

## 1. Core Flow

Architecture work should not start in a vacuum. Before PoC and technical work, there should be **market research / discovery** to identify opportunities, customer needs, pain points, and business value.

Market research is outside the scope of this architecture checklist, but it is critical. It costs time and resources, and if skipped, the team risks building something wrong or something nobody wants.

```text
Market Research / Discovery
  -> Technical Specification Inputs
  -> PoC
  -> Prototype
  -> Customer Engagement
  -> MVP Scope
  -> MVP Delivery
  -> Feedback and Improvement
  -> Pilot / Pre-Production
  -> Production
```

[Back to Contents](#contents)

---

## 2. Contract Model

| Stage | Contract / Commercial Model |
|-------|-----------------------------|
| Market Research / Discovery | Business cost; upstream product/business work before technical architecture |
| PoC | Part of business cost/work before creating the prototype |
| Prototype | Freely self-developed by the architect and pitched to the customer |
| MVP | Fixed Price after agreed scope |
| Production | Fixed Price or production/support model after agreed responsibilities |

[Back to Contents](#contents)

---

## 3. PoC and Prototype

### PoC

The **PoC** is an experimental stage where the architect validates architectural assumptions, tests hypotheses, and evaluates options such as technology stack, methodologies, design patterns, algorithms, integrations, infrastructure, and implementation strategy.

The PoC should answer:

- Can the proposed architecture satisfy product and project requirements?
- Which architectural decisions are justified by experiments?
- What trade-offs, risks, constraints, and observed behaviors matter?

### Prototype

After the PoC, the architect develops or assembles a working prototype that demonstrates:

- Product concept.
- Key workflows.
- Technical feasibility.
- Business value to the customer.

Customer engagement, feedback, data availability, integrations, and refined requirements are then used to define MVP scope.

[Back to Contents](#contents)

---

## 4. Main Architectural Design Documentation Package

### I. Product and Business

- Product Vision, Problem Statement, Business Value.
- Stakeholders, Personas, Use Cases, User Journeys.
- FRs, NFRs, Success Metrics.
- Module descriptions and high-level business flows.
- PoC/MVP scope, assumptions, constraints, and contract model.

### II. HLD

- C4 Context and Container diagrams.
- High-level integration, deployment, and data-flow diagrams.
- High-level sequence/activity diagrams for main journeys.
- Architecture overview, ADRs, ATAM/trade-off analysis.
- Diagrams in `.puml` and `.png`; docs in `.md`.

### III. LLD

- C4 Component diagrams, ER diagrams, sequence/state/activity diagrams.
- Detailed integration diagrams and useful C4 Code diagrams.
- RAG / GraphRAG / agentic workflow specification.
- API design in OpenAPI/Swagger `.yml`.
- ADRs for detailed technical decisions.

### IV. Deployment / Operations

- Detailed deployment, network, infrastructure, CI/CD, observability, and backup/recovery diagrams.
- `deployment-ops.md` covering deployment architecture, CI/CD, MLOps, HA/DR, observability, alerting, logging, backup/restore, runbooks, and ADRs.

### V. Codebase Documentation

- `README.md`, ADRs, and API documentation.
- `docs/coding-standards.md`, `docs/local-development.md`, and `docs/testing-strategy.md`.
- `docs/architecture-overview.md`, `docs/security-and-governance.md`, and `docs/deployment-ops.md`.
- `docs/training-strategy.md`, `docs/dataset-engineering.md`, and `docs/evaluation-strategy.md`.
- `docs/ai-agents-and-agentic-workflows.md` and `docs/tool-contracts.md`.

### VI. Pitch Deck / Prototype Presentation

- Problem, customer pain, value proposition.
- Prototype demo and key workflows.
- PoC findings and architecture decisions.
- MVP scope proposal, risks, delivery plan, success metrics, and commercial model.

[Back to Contents](#contents)

---

## 5. Codebase Implementation

### Actual Implementation vs Documentation

**Actual codebase implementation** means runnable/testable assets: app code, model code, training scripts, inference code, database files, tests, IaC, Docker/Kubernetes manifests, CI/CD config, environment templates, and sample data.

**Codebase documentation** explains how to develop, train, evaluate, deploy, operate, and maintain the implementation.

### Main Implementation Folders

- `src/` for application code.
- `models/` or model registry references.
- `training/` for LLM/SLM, retrieval embeddings, and distributed training templates.
- `agents/`, `workflows/`, `tools/` for AI agents, agentic workflows, tool use, and MCP integrations.
- `datasets/` for dataset schemas, samples, and processing scripts.
- `evaluation/` for model, retrieval, RAG, agent, and product evaluation.
- `inference/`, `pipelines/`, `infra/`, `deploy/`, `tests/`.

### AI/ML and Agentic Additions

- LLM/SLM and retrieval embedding training templates, including Unsloth and distributed training with PyTorch, Kubernetes, or Kubeflow Trainer.
- Dataset engineering docs for LLMs, embeddings, rerankers, classifiers, extractors, and agent tool-use models.
- Evaluation strategy for model quality, retrieval quality, RAG quality, hallucination/grounding, safety, regression, agent task success, and tool-use correctness.
- Agentic workflow templates based on LangChain, LangGraph, MCP, and the book **AI Agents and Applications: With LangChain, LangGraph, and MCP** by Roberto Infante.
- Useful learning references: **Domain-Specific Small Language Models: Efficient AI for local deployment** by Guglielmo Iozzia, Sebastian Raschka's **Build a Large Language Model (From Scratch)**, and Pere Marta's **Rearchitecting LLMs: Structural techniques for efficient models**.

[Back to Contents](#contents)

---

## 6. Pilot, MVP and Production

After MVP scope is agreed, the architect can:

- Hand over validated architecture, prototype, documentation, and guidance to a delivery/development team.
- Continue implementing directly as a solo engineer, entrepreneur, or startup team member.

The product then moves through:

```text
MVP Delivery
  -> Feedback Collection and Product Improvement
  -> Pilot / Pre-Production Battlefield Test
  -> Production Deployment
```

Production readiness must cover security, observability, reliability, HA/DR, CI/CD, MLOps, governance, API lifecycle, FinOps, and support/runbooks.

[Back to Contents](#contents)

---

## 7. Active Recall

- Why must market research happen before PoC?
- What does the PoC prove?
- How is the prototype different from the PoC?
- Which documents belong to Product/Business, HLD, LLD, Deployment/Ops, and Codebase?
- What is the difference between actual implementation folders and documentation folders?
- What must be added for LLMs, retrieval embeddings, and AI agents?
- What must be ready before Pilot and Production?

### One-Sentence Recall

> Market research identifies the right problem, PoC proves feasibility, prototype demonstrates value, MVP defines paid scope, pilot validates real-world use, and production makes the product operable and supportable.

[Back to Contents](#contents)
