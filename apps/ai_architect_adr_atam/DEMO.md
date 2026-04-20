# Demo walkthrough

A 5-minute tour of the features. The app is at http://127.0.0.1:8090/ after
`./build/adr_atam_server`.

## 0. Sanity check

- **Dashboard** shows "ADR-1: Adopt Postgres as primary datastore" and one
  ATAM session ("Q2 ATAM review — order-service platform") — these are the
  bundled samples under `data/`.
- Top-right badge: **AI: ready (gemma4:e2b)** if Ollama is up, or the
  configured Gemini model if `ADRA_LLM_PROVIDER=gemini`. **AI: offline** means
  the app is still usable without generation.

## 1. Create an ADR from a template

1. Click **ADR** → **+ New**.
2. Title: `Switch to managed Postgres on-prem`, template: `default`.
3. The form opens with the template context/decision/consequences skeleton
   already filled in.
4. Fill in Context and Decision with real content.
5. Click **Save**.

Behind the scenes: `POST /api/adr` created `data/adr/<uuid>.json`; subsequent
`PUT /api/adr/:id` rewrote it. The `revisions[]` array appended one entry each
time.

## 2. Use AI to draft and polish (requires Ollama or Gemini)

1. In the right AI panel, paste notes:

   > Need to move off self-managed Postgres because the team is small and the
   > operational burden is too high. Considering managed Postgres from the
   > cloud provider vs. staying self-managed with automation.

2. Click **Draft**. The generated Markdown appears in the AI output pane.
3. Click **Apply to editor** to drop the draft into Context, then split into
   Decision / Consequences by hand.
4. Hover the **Consequences** section and click **AI suggest** —
   `/api/ai/suggest-consequences` returns positive/negative bullets that are
   appended to the textarea.
5. Click **AI improve** next to *Decision* to rewrite it more tightly.
6. **Save**.

## 3. Reuse prior decisions

1. With the new ADR open, click **Find similar**.
2. You should see ADR-1 ranked by a score (>0). It matched on token overlap
   in the context and the shared `storage`/`Modifiability` signal.
3. Click the match to open it — this is how prior decisions surface during
   authoring, even if the LLM is off.

## 4. Export

1. Click **Preview markdown** — the server renders `adr_to_markdown()` and
   shows the formatted version in-page.
2. Click **Export .md** — the file downloads to your browser's Downloads
   folder *and* lands in `apps/ai_architect_adr_atam/exports/`:

   ```
   exports/adr-0002-switch-to-managed-postgres-on-prem.md
   ```

## 5. ATAM session

1. Switch to **ATAM** and open the bundled "Q2 ATAM review" session.
2. Add a scenario via **+ Add** under *Quality attribute scenarios*. Fill in a
   row, e.g.:
   - QA: `Security`
   - Stimulus source: `External attacker`
   - Stimulus: `scans exposed endpoints`
   - Environment: `Internet-facing production`
   - Artifact: `API gateway`
   - Response: `blocks after 100 failed auth attempts in 1 min`
   - Measure: `< 1 s detection, 0 successful bypass`
3. **Save**.
4. Paste raw review notes into the ATAM **AI assist** panel and click
   **Draft fields**. Review the JSON output, then click **Apply to editor** to
   fill system context, drivers, constraints, assumptions, approaches,
   scenarios, and findings.
5. Click **AI suggest** next to the scenario section — the tool calls
   `/api/ai/suggest-scenarios` with the QA of the first row and the system
   context, producing three more candidate scenarios in the AI panel.
6. Click **AI infer S/T** to ask the model to propose sensitivity and
   tradeoff points using the whole session state.
7. Click **AI summary** for a final report summary, and
   **AI → ADR candidates** for a list of ADRs you should write from the
   findings.

## 6. Cross-linking ATAM → ADR

On any finding card you can paste the id of an existing ADR into
**linked ADR id (optional)**. That becomes part of the ATAM JSON and appears
in the exported Markdown. The id is the UUID in `data/adr/<uuid>.json`.

## 7. Disable the LLM

Click the **AI** badge, uncheck *Enable AI features*, **Save**. The badge
turns amber and every AI-only button now yields a clear toast ("AI
unavailable"). All ADR/ATAM editing, saving, searching, exporting still
works. Re-check to re-enable without restarting.

## 8. Run tests

```
cmake --build build --target adr_atam_tests
./build/tests/adr_atam_tests
```

```
[ OK ] util_generate_uuid_is_v4_and_unique
[ OK ] util_slugify_basic
…
[ OK ] reuse_service_finds_similar_adr

N passed, 0 failed
```

That's the product.
