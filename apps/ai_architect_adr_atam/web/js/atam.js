const ATAM = (() => {
  const { $, $$, splitLines, joinLines, splitCsv, joinCsv } = ui;
  const escapeHtml = (s) => String(s || '').replace(/[&<>"']/g, (c) =>
    ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' })[c]);

  let current = null;
  let lastAiDraft = '';

  async function refreshList() {
    try {
      const f = $('#atam-filter').value;
      const items = await api.get('/api/atam' + (f ? '?q=' + encodeURIComponent(f) : ''));
      const list = $('#atam-list');
      list.innerHTML = '';
      for (const s of items) {
        const li = document.createElement('li');
        if (current && current.id === s.id) li.classList.add('active');
        li.innerHTML = `<span>${escapeHtml(s.title || '(untitled)')}</span>
                        <span class="status">${s.status}</span>`;
        li.addEventListener('click', () => load(s.id));
        list.appendChild(li);
      }
    } catch (e) { ui.err('List failed: ' + e.message); }
  }

  async function load(id) {
    try {
      const s = await api.get('/api/atam/' + id);
      current = s;
      $('#atam-empty').classList.add('hidden');
      $('#atam-form').classList.remove('hidden');
      $('#atam-preview-box').classList.add('hidden');
      $('#atam-ai-output').classList.add('hidden');
      $('#atam-status').value = s.status || 'draft';
      $('#atam-date').value = s.date || '';
      $('#atam-title').value = s.title || '';
      $('#atam-tags').value = joinCsv(s.tags);
      $('#atam-system-context').value = s.systemContext || '';
      $('#atam-drivers').value = joinLines(s.businessDrivers);
      $('#atam-constraints').value = joinLines(s.constraints);
      $('#atam-assumptions').value = joinLines(s.assumptions);
      renderApproaches(s.approaches || []);
      renderScenarios(s.scenarios || []);
      renderFindings(s.findings || []);
      refreshList();
    } catch (e) { ui.err('Load failed: ' + e.message); }
  }

  function renderApproaches(items) {
    const el = $('#atam-approaches');
    el.innerHTML = '';
    items.forEach((a, i) => el.appendChild(approachCard(a, i)));
  }

  function approachCard(a = {}, i = 0) {
    const d = document.createElement('div');
    d.className = 'card';
    d.dataset.index = i;
    d.innerHTML = `
      <div class="row">
        <input data-f="name" placeholder="name" value="${escapeHtml(a.name || '')}" />
        <input data-f="qualityAttributes" placeholder="quality attributes (comma)" value="${escapeHtml((a.qualityAttributes || []).join(', '))}" />
        <input data-f="tactics" placeholder="tactics (comma)" value="${escapeHtml((a.tactics || []).join(', '))}" />
        <div class="actions"><button type="button" class="mini danger" data-act="del">Remove</button></div>
      </div>
      <textarea data-f="description" rows="2" placeholder="description">${escapeHtml(a.description || '')}</textarea>
    `;
    d.dataset.id = a.id || '';
    d.querySelector('[data-act="del"]').addEventListener('click', () => d.remove());
    return d;
  }

  function renderScenarios(items) {
    const el = $('#atam-scenarios');
    el.innerHTML = '';
    items.forEach((s, i) => el.appendChild(scenarioCard(s, i)));
  }

  function scenarioCard(s = {}, i = 0) {
    const d = document.createElement('div');
    d.className = 'card';
    d.dataset.index = i;
    d.dataset.id = s.id || '';
    d.innerHTML = `
      <div class="row">
        <input data-f="qualityAttribute" placeholder="QA (e.g. Performance)" value="${escapeHtml(s.qualityAttribute || '')}" />
        <input data-f="importance" placeholder="importance H/M/L" value="${escapeHtml(s.importance || '')}" />
        <input data-f="difficulty" placeholder="difficulty H/M/L" value="${escapeHtml(s.difficulty || '')}" />
        <div class="actions"><button type="button" class="mini danger" data-act="del">Remove</button></div>
      </div>
      <div class="row">
        <input data-f="stimulusSource" placeholder="stimulus source" value="${escapeHtml(s.stimulusSource || '')}" />
        <input data-f="stimulus" placeholder="stimulus" value="${escapeHtml(s.stimulus || '')}" />
        <input data-f="environment" placeholder="environment" value="${escapeHtml(s.environment || '')}" />
        <input data-f="artifact" placeholder="artifact" value="${escapeHtml(s.artifact || '')}" />
      </div>
      <div class="row">
        <input data-f="response" placeholder="response" value="${escapeHtml(s.response || '')}" />
        <input data-f="responseMeasure" placeholder="measure" value="${escapeHtml(s.responseMeasure || '')}" />
      </div>
    `;
    d.querySelector('[data-act="del"]').addEventListener('click', () => d.remove());
    return d;
  }

  function renderFindings(items) {
    const el = $('#atam-findings');
    el.innerHTML = '';
    items.forEach((f, i) => el.appendChild(findingCard(f, i)));
  }

  function findingCard(f = {}, i = 0) {
    const d = document.createElement('div');
    d.className = 'card';
    d.dataset.index = i;
    d.dataset.id = f.id || '';
    const kinds = ['risk', 'non-risk', 'sensitivity-point', 'tradeoff-point'];
    d.innerHTML = `
      <div class="row">
        <select data-f="kind">${kinds.map((k) => `<option ${k === (f.kind || 'risk') ? 'selected' : ''}>${k}</option>`).join('')}</select>
        <input data-f="severity" placeholder="severity H/M/L" value="${escapeHtml(f.severity || '')}" />
        <input data-f="linkedAdrId" placeholder="linked ADR id (optional)" value="${escapeHtml(f.linkedAdrId || '')}" />
        <div class="actions"><button type="button" class="mini danger" data-act="del">Remove</button></div>
      </div>
      <textarea data-f="description" rows="2" placeholder="description">${escapeHtml(f.description || '')}</textarea>
    `;
    d.querySelector('[data-act="del"]').addEventListener('click', () => d.remove());
    return d;
  }

  function cardToObj(card, fields) {
    const o = { id: card.dataset.id || '' };
    for (const f of fields) {
      const el = card.querySelector(`[data-f="${f.name}"]`);
      if (!el) continue;
      let v = el.value;
      if (f.type === 'csv') v = splitCsv(v);
      o[f.name] = v;
    }
    return o;
  }

  function collect() {
    if (!current) return null;
    const approaches = $$('#atam-approaches .card').map((c) =>
      cardToObj(c, [{ name: 'name' }, { name: 'description' },
                    { name: 'qualityAttributes', type: 'csv' },
                    { name: 'tactics', type: 'csv' }]));
    const scenarios = $$('#atam-scenarios .card').map((c) =>
      cardToObj(c, [{ name: 'qualityAttribute' }, { name: 'stimulusSource' }, { name: 'stimulus' },
                    { name: 'environment' }, { name: 'artifact' }, { name: 'response' },
                    { name: 'responseMeasure' }, { name: 'importance' }, { name: 'difficulty' }]));
    const findings = $$('#atam-findings .card').map((c) =>
      cardToObj(c, [{ name: 'kind' }, { name: 'description' }, { name: 'severity' }, { name: 'linkedAdrId' }]));
    return {
      id: current.id,
      status: $('#atam-status').value,
      date: $('#atam-date').value,
      title: $('#atam-title').value,
      tags: splitCsv($('#atam-tags').value),
      systemContext: $('#atam-system-context').value,
      businessDrivers: splitLines($('#atam-drivers').value),
      constraints: splitLines($('#atam-constraints').value),
      assumptions: splitLines($('#atam-assumptions').value),
      utilityTree: current.utilityTree || [],
      approaches, scenarios, findings,
    };
  }

  async function createNew() {
    try {
      const templates = await api.get('/api/atam/templates');
      const names = templates.map((t) => t.name);
      const title = prompt('Title for new ATAM session?', 'Untitled ATAM session') || 'Untitled ATAM session';
      let tpl = '';
      if (names.length) tpl = prompt(`Template? (${names.join(', ')}; blank for none)`, names[0] || '') || '';
      const s = await api.post('/api/atam', { title, template: tpl });
      await load(s.id);
      ui.ok('Created');
    } catch (e) { ui.err('Create failed: ' + e.message); }
  }

  async function save() {
    if (!current) return;
    const payload = collect();
    payload.author = 'architect';
    payload.note = 'edit';
    try {
      const saved = await api.put('/api/atam/' + current.id, payload);
      current = saved;
      ui.ok('Saved');
      refreshList();
    } catch (e) { ui.err('Save failed: ' + e.message); }
  }

  async function del() {
    if (!current) return;
    if (!confirm('Delete this ATAM session?')) return;
    try {
      await api.del('/api/atam/' + current.id);
      current = null;
      $('#atam-form').classList.add('hidden');
      $('#atam-empty').classList.remove('hidden');
      refreshList();
    } catch (e) { ui.err('Delete failed: ' + e.message); }
  }

  async function preview() {
    if (!current) return;
    try {
      const md = await api.getText('/api/atam/' + current.id + '/export?format=md');
      const box = $('#atam-preview-box');
      box.textContent = md;
      box.classList.remove('hidden');
    } catch (e) { ui.err('Preview failed: ' + e.message); }
  }

  async function exportFile(format) {
    if (!current) return;
    await save();
    try {
      const out = await api.getText('/api/atam/' + current.id + '/export?format=' + format + '&save=true');
      const fname = `atam-${current.id.slice(0, 8)}.${format}`;
      const mime = format === 'md' ? 'text/markdown' : 'application/json';
      ui.downloadBlob(fname, out, mime);
      ui.ok('Exported ' + fname);
    } catch (e) { ui.err('Export failed: ' + e.message); }
  }

  async function aiInferTradeoffs() {
    if (!current) return;
    await save();
    try {
      const r = await api.post('/api/ai/atam/' + current.id + '/infer-tradeoffs', {});
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      showAiOutput(r.text);
    } catch (e) { ui.err('AI call failed: ' + e.message); }
  }
  async function aiSummary() {
    if (!current) return;
    await save();
    try {
      const r = await api.post('/api/ai/atam/' + current.id + '/summary', {});
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      showAiOutput(r.text);
    } catch (e) { ui.err('AI call failed: ' + e.message); }
  }
  async function aiAdrCandidates() {
    if (!current) return;
    await save();
    try {
      const r = await api.post('/api/ai/atam/' + current.id + '/adr-candidates', {});
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      showAiOutput(r.text);
    } catch (e) { ui.err('AI call failed: ' + e.message); }
  }
  function showAiOutput(text) {
    const box = $('#atam-ai-output');
    box.textContent = text;
    box.classList.remove('hidden');
  }

  function showDraftOutput(text) {
    $('#ai-atam-output').textContent = text;
    showAiOutput(text);
  }

  function collectQualityAttributes() {
    const values = [];
    splitCsv($('#atam-tags').value).forEach((x) => values.push(x));
    $$('#atam-approaches .card [data-f="qualityAttributes"]').forEach((el) =>
      splitCsv(el.value).forEach((x) => values.push(x)));
    $$('#atam-scenarios .card [data-f="qualityAttribute"]').forEach((el) => {
      if (el.value.trim()) values.push(el.value.trim());
    });
    return [...new Set(values.filter(Boolean))];
  }

  async function aiDraftAtam() {
    try {
      const r = await api.post('/api/ai/draft-atam', {
        title: $('#atam-title').value || '',
        systemContext: $('#atam-system-context').value || '',
        notes: $('#ai-atam-notes').value,
        qualityAttributes: collectQualityAttributes(),
      });
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      lastAiDraft = r.text;
      showDraftOutput(r.text);
    } catch (e) { ui.err('AI draft failed: ' + e.message); }
  }

  function extractDraftJson(text) {
    let s = String(text || '').trim();
    const fenced = s.match(/```(?:json)?\s*([\s\S]*?)```/i);
    if (fenced) s = fenced[1].trim();
    try { return JSON.parse(s); } catch (_) {}
    const first = s.indexOf('{');
    const last = s.lastIndexOf('}');
    if (first !== -1 && last > first) {
      return JSON.parse(s.slice(first, last + 1));
    }
    throw new Error('AI draft did not contain parseable JSON.');
  }

  function asArray(x) {
    return Array.isArray(x) ? x.filter((v) => v !== null && v !== undefined) : [];
  }

  function aiApplyAtamDraft() {
    if (!lastAiDraft) return ui.err('No ATAM draft. Click "Draft fields" first.');
    let draft;
    try {
      draft = extractDraftJson(lastAiDraft);
    } catch (e) {
      return ui.err(e.message);
    }

    const nonEmpty =
      $('#atam-system-context').value.trim() ||
      $('#atam-drivers').value.trim() ||
      $('#atam-constraints').value.trim() ||
      $('#atam-assumptions').value.trim() ||
      $('#atam-approaches .card') ||
      $('#atam-scenarios .card') ||
      $('#atam-findings .card');
    if (nonEmpty && !confirm('Apply AI draft and replace current ATAM draft fields?')) return;

    const filled = [];
    if (draft.systemContext) {
      $('#atam-system-context').value = String(draft.systemContext);
      filled.push('system context');
    }
    const setLines = (id, key, label) => {
      const vals = asArray(draft[key]).map((x) => String(x).trim()).filter(Boolean);
      if (!vals.length) return;
      $(id).value = vals.join('\n');
      filled.push(label);
    };
    setLines('#atam-drivers', 'businessDrivers', 'business drivers');
    setLines('#atam-constraints', 'constraints', 'constraints');
    setLines('#atam-assumptions', 'assumptions', 'assumptions');

    const approaches = asArray(draft.approaches).map((a) => ({
      name: a.name || '',
      description: a.description || '',
      qualityAttributes: asArray(a.qualityAttributes).map(String),
      tactics: asArray(a.tactics).map(String),
    })).filter((a) => a.name || a.description);
    if (approaches.length) {
      renderApproaches(approaches);
      filled.push('approaches');
    }

    const scenarios = asArray(draft.scenarios).map((s) => ({
      qualityAttribute: s.qualityAttribute || '',
      stimulusSource: s.stimulusSource || '',
      stimulus: s.stimulus || '',
      environment: s.environment || '',
      artifact: s.artifact || '',
      response: s.response || '',
      responseMeasure: s.responseMeasure || '',
      importance: s.importance || '',
      difficulty: s.difficulty || '',
    })).filter((s) => s.qualityAttribute || s.stimulus || s.response);
    if (scenarios.length) {
      renderScenarios(scenarios);
      filled.push('scenarios');
    }

    const validKinds = new Set(['risk', 'non-risk', 'sensitivity-point', 'tradeoff-point']);
    const findings = asArray(draft.findings).map((f) => ({
      kind: validKinds.has(f.kind) ? f.kind : 'risk',
      description: f.description || '',
      severity: f.severity || '',
      linkedAdrId: '',
    })).filter((f) => f.description);
    if (findings.length) {
      renderFindings(findings);
      filled.push('findings');
    }

    if (!filled.length) return ui.err('AI draft JSON did not contain recognizable ATAM fields.');
    ui.ok('Applied ATAM draft to: ' + filled.join(', ') + '. Review and Save.');
  }

  async function aiSuggestScenarioForFirstQA() {
    const qa = ($('#atam-scenarios .card [data-f="qualityAttribute"]') || { value: '' }).value;
    const ctx = $('#atam-system-context').value;
    if (!qa) return ui.err('Add a scenario with a QA first.');
    try {
      const r = await api.post('/api/ai/suggest-scenarios', { qualityAttribute: qa, systemContext: ctx });
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      showAiOutput(r.text);
    } catch (e) { ui.err('AI call failed: ' + e.message); }
  }

  async function findSimilarScenarios() {
    const firstCard = $('#atam-scenarios .card');
    if (!firstCard) return ui.err('Add a scenario first.');
    const pick = (f) => (firstCard.querySelector(`[data-f="${f}"]`) || { value: '' }).value;
    try {
      const hits = await api.post('/api/reuse/scenarios', {
        qualityAttribute: pick('qualityAttribute'),
        stimulus: pick('stimulus'),
        response: pick('response'),
      });
      if (!hits.length) return ui.ok('No similar scenarios found.');
      showAiOutput(hits.map((h) =>
        `${h.score.toFixed(2)}  [${h.qualityAttribute}] ${h.stimulus} -> ${h.response}  (session: ${h.sessionTitle})`).join('\n'));
    } catch (e) { ui.err('Reuse failed: ' + e.message); }
  }

  function init() {
    $('#atam-new').addEventListener('click', createNew);
    $('#atam-save').addEventListener('click', save);
    $('#atam-delete').addEventListener('click', del);
    $('#atam-preview').addEventListener('click', preview);
    $('#atam-export-md').addEventListener('click', () => exportFile('md'));
    $('#atam-export-json').addEventListener('click', () => exportFile('json'));
    $('#atam-filter').addEventListener('input', refreshList);
    $('#atam-add-approach').addEventListener('click', () =>
      $('#atam-approaches').appendChild(approachCard()));
    $('#atam-add-scenario').addEventListener('click', () =>
      $('#atam-scenarios').appendChild(scenarioCard()));
    $('#atam-add-finding').addEventListener('click', () =>
      $('#atam-findings').appendChild(findingCard()));
    $('#atam-ai-scenario').addEventListener('click', aiSuggestScenarioForFirstQA);
    $('#atam-find-similar-scenarios').addEventListener('click', findSimilarScenarios);
    $('#atam-infer-tradeoffs').addEventListener('click', aiInferTradeoffs);
    $('#atam-summary').addEventListener('click', aiSummary);
    $('#atam-adr-candidates').addEventListener('click', aiAdrCandidates);
    $('#ai-draft-atam').addEventListener('click', aiDraftAtam);
    $('#ai-apply-atam-draft').addEventListener('click', aiApplyAtamDraft);
    window.addEventListener('view:activated', (e) => { if (e.detail === 'atam') refreshList(); });
    window.addEventListener('atam:select', (e) => load(e.detail));
  }

  return { init, createNew, refreshList };
})();
