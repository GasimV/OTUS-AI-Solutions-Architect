// Small UI helpers shared across views.
const ui = (() => {
  const $ = (sel, root = document) => root.querySelector(sel);
  const $$ = (sel, root = document) => Array.from(root.querySelectorAll(sel));

  let toastTimer = null;
  function toast(message, kind = '') {
    const el = $('#toast');
    if (!el) return;
    el.className = 'toast show ' + kind;
    el.textContent = message;
    clearTimeout(toastTimer);
    toastTimer = setTimeout(() => (el.className = 'toast'), 2800);
  }
  const ok = (m) => toast(m, 'ok');
  const err = (m) => toast(m, 'err');

  function splitLines(v) {
    return (v || '')
      .split(/\r?\n/)
      .map((s) => s.trim())
      .filter(Boolean);
  }
  function joinLines(arr) {
    return (arr || []).join('\n');
  }
  function splitCsv(v) {
    return (v || '')
      .split(',')
      .map((s) => s.trim())
      .filter(Boolean);
  }
  function joinCsv(arr) {
    return (arr || []).join(', ');
  }
  function parseLinks(v) {
    return splitLines(v)
      .map((line) => {
        const idx = line.indexOf(':');
        if (idx < 0) return { relation: 'related', targetId: line.trim() };
        return {
          relation: line.slice(0, idx).trim() || 'related',
          targetId: line.slice(idx + 1).trim(),
        };
      })
      .filter((l) => l.targetId);
  }
  function stringifyLinks(links) {
    return (links || []).map((l) => `${l.relation || 'related'}:${l.targetId}`).join('\n');
  }
  function showView(name) {
    $$('.view').forEach((v) => v.classList.add('hidden'));
    const el = $('#view-' + name);
    if (el) el.classList.remove('hidden');
    $$('.tab').forEach((t) => t.classList.toggle('active', t.dataset.view === name));
    window.dispatchEvent(new CustomEvent('view:activated', { detail: name }));
  }
  function downloadBlob(filename, content, mime) {
    const blob = new Blob([content], { type: mime || 'application/octet-stream' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url; a.download = filename; a.click();
    setTimeout(() => URL.revokeObjectURL(url), 0);
  }

  return { $, $$, toast, ok, err, splitLines, joinLines, splitCsv, joinCsv,
           parseLinks, stringifyLinks, showView, downloadBlob };
})();
