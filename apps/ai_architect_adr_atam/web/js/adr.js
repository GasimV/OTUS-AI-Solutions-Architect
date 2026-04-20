const ADR = (() => {
  const { $, $$, splitLines, joinLines, splitCsv, joinCsv, parseLinks, stringifyLinks } = ui;

  let current = null;    // loaded ADR in editor
  let lastList = [];
  let lastAiDraft = '';

  function escapeHtml(s) {
    return String(s || '').replace(/[&<>"']/g, (c) =>
      ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' })[c]);
  }

  async function refreshList() {
    try {
      const f = $('#adr-filter').value;
      const s = $('#adr-status-filter').value;
      const qs = [];
      if (f) qs.push('q=' + encodeURIComponent(f));
      if (s) qs.push('status=' + encodeURIComponent(s));
      const items = await api.get('/api/adr' + (qs.length ? '?' + qs.join('&') : ''));
      lastList = items;
      const list = $('#adr-list');
      list.innerHTML = '';
      for (const a of items) {
        const li = document.createElement('li');
        if (current && current.id === a.id) li.classList.add('active');
        li.innerHTML = `<span class="num">ADR-${a.number}</span>
                        <span>${escapeHtml(a.title || '(untitled)')}</span>
                        <span class="status ${a.status}">${a.status}</span>`;
        li.addEventListener('click', () => load(a.id));
        list.appendChild(li);
      }
    } catch (e) { ui.err('List failed: ' + e.message); }
  }

  async function load(id) {
    try {
      const a = await api.get('/api/adr/' + id);
      current = a;
      $('#adr-empty').classList.add('hidden');
      $('#adr-form').classList.remove('hidden');
      $('#adr-reuse-box').classList.add('hidden');
      $('#adr-preview-box').classList.add('hidden');
      $('#adr-number').value = a.number || 0;
      $('#adr-status').value = a.status || 'proposed';
      $('#adr-date').value = a.date || '';
      $('#adr-title').value = a.title || '';
      $('#adr-tags').value = joinCsv(a.tags);
      $('#adr-qas').value = joinCsv(a.qualityAttributes);
      $('#adr-stakeholders').value = joinCsv(a.stakeholders);
      $('#adr-requirements').value = joinLines(a.requirements);
      $('#adr-context').value = a.context || '';
      $('#adr-decision').value = a.decision || '';
      $('#adr-consequences').value = a.consequences || '';
      $('#adr-alternatives').value = joinLines(a.alternatives);
      $('#adr-risks').value = joinLines(a.risks);
      $('#adr-assumptions').value = joinLines(a.assumptions);
      $('#adr-links').value = stringifyLinks(a.links);
      refreshList();
    } catch (e) { ui.err('Load failed: ' + e.message); }
  }

  async function createNew() {
    try {
      const templates = await api.get('/api/adr/templates');
      const names = templates.map((t) => t.name);
      const title = prompt('Title for new ADR?', 'Untitled Decision') || 'Untitled Decision';
      let tpl = '';
      if (names.length) {
        tpl = prompt(`Template? (${names.join(', ')}; blank for none)`, names[0] || '') || '';
      }
      const a = await api.post('/api/adr', { title, template: tpl });
      await load(a.id);
      ui.ok('Created ADR-' + a.number);
    } catch (e) { ui.err('Create failed: ' + e.message); }
  }

  function collect() {
    if (!current) return null;
    return {
      id: current.id,
      number: parseInt($('#adr-number').value || '0', 10),
      status: $('#adr-status').value,
      date: $('#adr-date').value,
      title: $('#adr-title').value,
      tags: splitCsv($('#adr-tags').value),
      qualityAttributes: splitCsv($('#adr-qas').value),
      stakeholders: splitCsv($('#adr-stakeholders').value),
      requirements: splitLines($('#adr-requirements').value),
      context: $('#adr-context').value,
      decision: $('#adr-decision').value,
      consequences: $('#adr-consequences').value,
      alternatives: splitLines($('#adr-alternatives').value),
      risks: splitLines($('#adr-risks').value),
      assumptions: splitLines($('#adr-assumptions').value),
      links: parseLinks($('#adr-links').value),
    };
  }

  async function save() {
    if (!current) return;
    const payload = collect();
    payload.author = 'architect';
    payload.note = 'edit';
    try {
      const saved = await api.put('/api/adr/' + current.id, payload);
      current = saved;
      ui.ok('Saved ADR-' + saved.number);
      refreshList();
    } catch (e) { ui.err('Save failed: ' + e.message); }
  }

  async function del() {
    if (!current) return;
    if (!confirm('Delete this ADR?')) return;
    try {
      await api.del('/api/adr/' + current.id);
      current = null;
      $('#adr-form').classList.add('hidden');
      $('#adr-empty').classList.remove('hidden');
      refreshList();
      ui.ok('Deleted');
    } catch (e) { ui.err('Delete failed: ' + e.message); }
  }

  async function findSimilar() {
    const payload = {
      title: $('#adr-title').value,
      context: $('#adr-context').value,
      tags: splitCsv($('#adr-tags').value),
      qualityAttributes: splitCsv($('#adr-qas').value),
    };
    try {
      const hits = await api.post('/api/reuse/adrs', payload);
      const box = $('#adr-reuse-box');
      box.classList.remove('hidden');
      if (!hits.length) { box.innerHTML = '<em>No similar ADRs found.</em>'; return; }
      box.innerHTML = '<h4>Similar past ADRs</h4><ul>' + hits.map((h) =>
        `<li><span class="score">${h.score.toFixed(2)}</span>
             <span><a href="#" data-open="${h.id}">ADR-${h.number}: ${escapeHtml(h.title)}</a>
             <span class="muted">(${h.status})</span></span></li>`).join('') + '</ul>';
      box.querySelectorAll('a[data-open]').forEach((el) =>
        el.addEventListener('click', (ev) => { ev.preventDefault(); load(el.dataset.open); }));
    } catch (e) { ui.err('Similarity failed: ' + e.message); }
  }

  async function preview() {
    if (!current) return;
    try {
      const md = await api.getText('/api/adr/' + current.id + '/export?format=md');
      const box = $('#adr-preview-box');
      box.textContent = md;
      box.classList.remove('hidden');
    } catch (e) { ui.err('Preview failed: ' + e.message); }
  }

  async function exportFile(format) {
    if (!current) return;
    try {
      await save();
      const md = await api.getText('/api/adr/' + current.id + '/export?format=' + format + '&save=true');
      const fname = `adr-${String(current.number).padStart(4, '0')}-${current.slug || 'untitled'}.${format}`;
      const mime = format === 'md' ? 'text/markdown' : 'application/json';
      ui.downloadBlob(fname, md, mime);
      ui.ok('Exported ' + fname);
    } catch (e) { ui.err('Export failed: ' + e.message); }
  }

  async function callAiInline(kind) {
    try {
      if (kind.startsWith('improve:')) {
        const section = kind.split(':')[1];
        const elId = 'adr-' + section;
        const r = await api.post('/api/ai/improve-section',
          { section, text: $('#' + elId).value });
        handleAiResult(r, (t) => { $('#' + elId).value = t; });
        return;
      }
      if (kind === 'summarize-context') {
        const r = await api.post('/api/ai/summarize-context', { context: $('#adr-context').value });
        handleAiResult(r, (t) => ui.toast('Summary ready — shown in AI panel', ''));
        if (r.ok) $('#ai-adr-output').textContent = r.text;
        return;
      }
      if (kind === 'suggest-alternatives') {
        const r = await api.post('/api/ai/suggest-alternatives', {
          title: $('#adr-title').value,
          context: $('#adr-context').value,
          decision: $('#adr-decision').value,
        });
        if (r.ok) $('#adr-alternatives').value += (($('#adr-alternatives').value ? '\n' : '') + r.text);
        handleAiResult(r);
        return;
      }
      if (kind === 'suggest-consequences') {
        const r = await api.post('/api/ai/suggest-consequences', {
          title: $('#adr-title').value, decision: $('#adr-decision').value,
        });
        if (r.ok) $('#adr-consequences').value += (($('#adr-consequences').value ? '\n\n' : '') + r.text);
        handleAiResult(r);
        return;
      }
      if (kind === 'suggest-risks') {
        const r = await api.post('/api/ai/suggest-risks', {
          title: $('#adr-title').value, decision: $('#adr-decision').value,
        });
        if (r.ok) $('#adr-risks').value += (($('#adr-risks').value ? '\n' : '') + r.text);
        handleAiResult(r);
        return;
      }
    } catch (e) { ui.err('AI call failed: ' + e.message); }
  }

  function handleAiResult(r, onOk) {
    if (!r.ok) { ui.err(r.error || 'AI unavailable'); return; }
    if (onOk) onOk(r.text);
    else ui.ok('AI response applied');
  }

  async function aiDraft() {
    try {
      const r = await api.post('/api/ai/draft-adr', {
        title: $('#adr-title').value || '',
        notes: $('#ai-adr-notes').value,
        qualityAttributes: splitCsv($('#adr-qas').value),
      });
      if (!r.ok) return ui.err(r.error || 'AI unavailable');
      lastAiDraft = r.text;
      $('#ai-adr-output').textContent = r.text;
    } catch (e) { ui.err('AI draft failed: ' + e.message); }
  }

  function aiApply() {
    if (!lastAiDraft) return ui.err('No draft. Click "Draft" first.');
    // Naive mapping: put the whole draft in Context so the architect can re-split.
    $('#adr-context').value = lastAiDraft;
    ui.ok('Applied draft into Context — split into sections as needed, then Save.');
  }

  function init() {
    $('#adr-new').addEventListener('click', createNew);
    $('#adr-save').addEventListener('click', save);
    $('#adr-delete').addEventListener('click', del);
    $('#adr-reuse').addEventListener('click', findSimilar);
    $('#adr-preview').addEventListener('click', preview);
    $('#adr-export-md').addEventListener('click', () => exportFile('md'));
    $('#adr-export-json').addEventListener('click', () => exportFile('json'));
    $('#adr-filter').addEventListener('input', refreshList);
    $('#adr-status-filter').addEventListener('change', refreshList);
    $$('#adr-form [data-ai]').forEach((b) => b.addEventListener('click', () => callAiInline(b.dataset.ai)));
    $('#ai-draft-adr').addEventListener('click', aiDraft);
    $('#ai-apply-draft').addEventListener('click', aiApply);
    window.addEventListener('view:activated', (e) => { if (e.detail === 'adr') refreshList(); });
    window.addEventListener('adr:select', (e) => load(e.detail));
  }

  return { init, createNew, refreshList };
})();
