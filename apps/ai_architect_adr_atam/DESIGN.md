# Design

## Goal

Give a working software architect a single, local, offline-friendly workbench for
two related artifacts that dominate day-to-day architecture work — **ADR**
(Architecture Decision Records) and **ATAM** (Architecture Tradeoff Analysis
Method) — with optional LLM assistance that *never* becomes a hard dependency.

The design choices below are optimized for:

- **Local-first, offline by default.** Ollama mode stays local; Gemini mode is
  opt-in and calls Google's hosted API.
- **Human-inspectable storage.** JSON files per artifact; diff in git; edit by hand.
- **Small, boring tech.** Single-header HTTP server + single-header JSON, plain
  HTML/CSS/JS. Zero package manager, fast clean build.
- **Graceful AI degradation.** All AI paths go through an `LlmClient`
  interface; availability is probed and surfaced in the UI.

## High-level architecture

```
┌──────────────────────────────────────────────────────────────┐
│                        Browser (localhost)                   │
│   index.html, css, vanilla JS modules (dashboard/ADR/ATAM)   │
└───────────▲───────────────────────▲──────────────────────────┘
            │ REST (JSON over HTTP) │
            │                       │ static assets (/css,/js,…)
┌───────────┴───────────────────────┴──────────────────────────┐
│                   C++ HTTP server (adr_atam_server)          │
│   ┌──────────────┐  ┌───────────────┐   ┌────────────────┐   │
│   │ api_routes   │──│ AdrService    │──▶│ AdrRepository  │   │
│   │ (http layer) │  │ AtamService   │──▶│ AtamRepository │   │
│   │              │  │ ExportService │  ▲└──────┬─────────┘   │
│   │              │  │ ReuseService  │──┘       │             │
│   │              │  │ TemplateSvc   │          │FileStore    │
│   │              │  │ AiService     │──┐       │(JSON files) │
│   └──────┬───────┘  └─────┬─────────┘  │       │             │
│          │                │            │       │             │
│          │         ┌──────┴───────┐    │       │             │
│          │         │  LlmClient   │◀───┘       │             │
│          │         │ (interface)  │            │             │
│          │         │ Provider LLM │            │             │
│          │         └──────────────┘            │             │
│   httplib.h (static+dynamic route table)       │             │
└────────────────────────────────────────────────┴─────────────┘
                       filesystem (data/, templates/, exports/)
```

## Layering

| Layer | Responsibility | Files |
|---|---|---|
| **domain** | Pure data types and their JSON round-trip. | `server/domain/` |
| **persistence** | `FileStore` primitive + one repository per aggregate. | `server/persistence/` |
| **services** | Use-case orchestration, validation, search, reuse, export. | `server/services/` |
| **ai** | Provider-neutral LLM client interface + Ollama/Gemini implementations + architect-level prompts. | `server/ai/` |
| **http** | REST routing, DTO shaping. | `server/http/api_routes.cpp` |
| **main** | Wiring and configuration. | `server/main.cpp` |
| **web** | Browser UI that consumes the API. | `web/` |

Dependencies point *inward*: `http → services → persistence/domain/ai`. The
domain knows nothing about JSON beyond `to_json/from_json` convenience free
functions. Services do not know about HTTP. The UI is the only thing that
knows about browsers.

## ADR model

Inspired by Michael Nygard's format with explicit, reusable extensions that
an architect actually fills in:

| Field | Notes |
|---|---|
| `id` | UUID v4, source of truth for identity. |
| `slug`, `number` | Derived for filenames and sort order. |
| `title`, `status`, `date` | Core header. Status is constrained to a fixed set. |
| `context`, `decision`, `consequences` | Narrative body. |
| `alternatives`, `assumptions`, `risks` | Bullet arrays. |
| `qualityAttributes`, `requirements` | For cross-linking to ATAM. |
| `stakeholders`, `tags` | Discoverability. |
| `links[]` | `{targetId, relation}` — supports supersede graphs. |
| `revisions[]` | Append-only change history. |

## ATAM model

Kept close to the canonical ATAM vocabulary so exports are immediately
recognizable to an architecture community:

- `businessDrivers`, `constraints`, `assumptions`, `systemContext` — framing.
- `approaches[]` — architectural approaches and tactics.
- `utilityTree[]` — hierarchical quality attribute tree with scenario ids.
- `scenarios[]` — six-part QA scenarios (source/stimulus/environment/artifact/response/measure) plus importance/difficulty.
- `findings[]` — unified shape for **risks, non-risks, sensitivity points,
  tradeoff points** (`kind` field discriminates). Each can carry a
  `linkedAdrId` for ATAM → ADR traceability.

## Persistence

- One JSON file per aggregate, keyed by UUID, written pretty-printed.
- `FileStore` handles atomic save/load under a mutex.
- `AdrRepository` assigns a monotonic ADR number (next = max+1).
- No migrations: `from_json` tolerates missing fields by returning defaults.

## Reuse (no-LLM)

`ReuseService` computes a Jaccard-style similarity over tokenized titles,
contexts, and scenario text, plus small boosts for shared tags/quality
attributes. Runs on every saved artifact and is used by:

- **"Find similar"** on the ADR editor.
- **"Find similar scenarios"** on the ATAM editor.

It's intentionally lightweight — no embeddings, no index — so it works without
ever touching an LLM provider.

## Optional LLM integration

### Interface

```cpp
class LlmClient {
    virtual LlmStatus check_status() = 0;
    virtual std::optional<std::string> generate(
        const std::string& prompt, const std::string& system = "") = 0;
    virtual const LlmConfig& config() const = 0;
    virtual void update_config(const LlmConfig&) = 0;
};
```

All AI features go through `AiService`, which composes architect-specific
prompts and returns `AiResult{ok,text,error}`. The UI shows failures as
toasts; nothing blocks.

### Provider specifics

- `GET /api/tags` is used to detect reachability and whether the configured
  Ollama model name appears in the inventory.
- Ollama uses `POST /api/generate` with `{"stream": false}` for one-shot
  generation.
- Gemini uses `GET /v1beta/models` for reachability/model checks and
  `POST /v1beta/models/{model}:generateContent` for one-shot generation.
  `GEMINI_API_KEY` is loaded from `.env` or the process environment and is not
  returned by the API.
- Connect timeout is short (3s) so a missing provider is noticed quickly; read
  timeout is long (120s) for actual generations.
- Config can be changed at runtime via `POST /api/ai/config` — no restart.

### Graceful degradation

- Every request hits the route handler; only AI-prefixed endpoints consult the
  LLM, and those return `503 {ok:false,error:…}` when it's unavailable.
- The UI polls `/api/ai/status` every 15 s to update the colored status dot.

## Concurrency model

`httplib::Server` is synchronous per connection but multi-threaded across
connections. The server owns one thread-safe `FileStore` and repositories
above it; writes serialize through the file-store mutex. LLM calls go through
the client, which creates a fresh `httplib::Client` per request — safe to call
concurrently.

For very long generations the UI does not block the user: other tabs and
operations continue on separate browser fetches.

## Security posture

- Default bind is `127.0.0.1`. Overridable via `--host`.
- CORS is wide-open because the expected consumer is the user's own browser
  loading from the same origin — kept simple, not exposed to the internet.
- No authentication; this is a local tool.
- No code execution, no template rendering from untrusted input — templates
  are data-only JSON under version control.

## Why this stack

- `httplib.h` and `json.hpp` — widely used single-header libs, already vendored
  in the sibling calculators app. Zero build-system surprises on Windows.
- **No external UI framework.** The UI is small enough that a framework would
  dominate the dependency footprint and add setup steps. Vanilla modules are
  enough for a productivity-focused tool.
- **File-based persistence.** The artifacts themselves *are* the product
  of architecture work; making them text files in git is the natural outcome.

## Testing strategy

A tiny header-only test harness registers tests with a macro, catches
exceptions, and reports a pass/fail summary. Compiled against the static
`adr_atam_core` library and run through `ctest`. Covers:

- Util helpers (uuid, slug, split/join, trim).
- Domain JSON round-trip for ADR and ATAM.
- `FileStore` save/load, repository CRUD, and numbering.
- Template application, export Markdown structure, similarity ranking.

No mocks or DI frameworks — services receive real dependencies in a temp
directory per test.

## REST reference

| Method | Path | Purpose |
|---|---|---|
| GET | `/api/status` | App + AI summary |
| GET | `/api/ai/status` | Detailed AI status |
| POST | `/api/ai/config` | Update LLM config (provider/host/port/model/enabled) |
| GET | `/api/adr/templates` | List ADR templates |
| GET | `/api/adr?q&status&tag&qa` | List ADRs with filters |
| POST | `/api/adr` | Create (body: `{title, template, author}`) |
| GET | `/api/adr/:id` | Load |
| PUT | `/api/adr/:id` | Save (full body; merges with existing created_at/revisions) |
| DELETE | `/api/adr/:id` | Delete |
| GET | `/api/adr/:id/export?format=md|json&save=true` | Export |
| GET | `/api/atam/templates` | List ATAM templates |
| GET | `/api/atam?q&status&tag` | List sessions |
| POST | `/api/atam` | Create |
| GET/PUT/DELETE | `/api/atam/:id` | Load / save / delete |
| GET | `/api/atam/:id/export?format=md|json&save=true` | Export |
| POST | `/api/reuse/adrs` | Similar-ADR search |
| POST | `/api/reuse/scenarios` | Similar-scenario search |
| POST | `/api/ai/draft-adr` | Draft from architect notes |
| POST | `/api/ai/improve-section` | Rewrite a section |
| POST | `/api/ai/summarize-context` | Context summary |
| POST | `/api/ai/suggest-alternatives` | Suggest 3 alternatives |
| POST | `/api/ai/suggest-consequences` | Suggest positive/negative consequences |
| POST | `/api/ai/suggest-risks` | Suggest risk bullets |
| POST | `/api/ai/suggest-scenarios` | QA scenarios for a given attribute |
| POST | `/api/ai/atam/:id/infer-tradeoffs` | Propose sensitivity + tradeoff points |
| POST | `/api/ai/atam/:id/summary` | Markdown executive summary |
| POST | `/api/ai/atam/:id/adr-candidates` | ATAM → ADR candidate list |
