const Dashboard = (() => {
  const { $, $$ } = ui;

  async function refreshAdrs(filter) {
    try {
      const q = filter ? ('?q=' + encodeURIComponent(filter)) : '';
      const items = await api.get('/api/adr' + q);
      const list = $('#dash-adr-list');
      list.innerHTML = '';
      if (!items.length) {
        list.innerHTML = '<li class="muted">No ADRs yet.</li>';
        return;
      }
      for (const a of items.slice(0, 25)) {
        const li = document.createElement('li');
        li.innerHTML = `<span class="num">ADR-${a.number || 0}</span>
                        <span>${escapeHtml(a.title || '(untitled)')}</span>
                        <span class="status ${a.status}">${a.status}</span>`;
        li.addEventListener('click', () => {
          ui.showView('adr');
          window.dispatchEvent(new CustomEvent('adr:select', { detail: a.id }));
        });
        list.appendChild(li);
      }
    } catch (e) { ui.err('Failed to load ADRs: ' + e.message); }
  }

  async function refreshAtams(filter) {
    try {
      const q = filter ? ('?q=' + encodeURIComponent(filter)) : '';
      const items = await api.get('/api/atam' + q);
      const list = $('#dash-atam-list');
      list.innerHTML = '';
      if (!items.length) {
        list.innerHTML = '<li class="muted">No ATAM sessions yet.</li>';
        return;
      }
      for (const s of items.slice(0, 25)) {
        const li = document.createElement('li');
        li.innerHTML = `<span>${escapeHtml(s.title || '(untitled)')}</span>
                        <span class="status ${s.status}">${s.status}</span>`;
        li.addEventListener('click', () => {
          ui.showView('atam');
          window.dispatchEvent(new CustomEvent('atam:select', { detail: s.id }));
        });
        list.appendChild(li);
      }
    } catch (e) { ui.err('Failed to load ATAM sessions: ' + e.message); }
  }

  async function refreshAi() {
    try {
      const st = await api.get('/api/ai/status');
      const el = $('#ai-detail');
      const endpoint = st.provider === 'gemini'
        ? `${st.apiHost || 'generativelanguage.googleapis.com'}:${st.apiPort || 443}`
        : `${st.host || '127.0.0.1'}:${st.port || 11434}`;
      const lines = [
        `<div><span class="k">Provider</span> ${escapeHtml(st.provider || 'ollama')}</div>`,
        `<div><span class="k">Enabled</span> ${st.enabled}</div>`,
        `<div><span class="k">Reachable</span> ${st.reachable}</div>`,
        `<div><span class="k">Active model</span> ${escapeHtml(st.activeModel || '')}</div>`,
        `<div><span class="k">Model available</span> ${st.modelAvailable}</div>`,
        `<div><span class="k">Endpoint</span> ${escapeHtml(endpoint)}</div>`,
      ];
      if (st.provider === 'gemini') {
        lines.push(`<div><span class="k">API key</span> ${st.apiKeyConfigured ? 'configured' : 'missing'}</div>`);
      }
      if (st.availableModels && st.availableModels.length) {
        lines.push(`<div><span class="k">Models seen</span> ${st.availableModels.map(escapeHtml).join(', ')}</div>`);
      }
      if (st.error) lines.push(`<div><span class="k">Note</span> ${escapeHtml(st.error)}</div>`);
      el.innerHTML = lines.join('');
    } catch (e) {
      $('#ai-detail').textContent = 'AI status unavailable: ' + e.message;
    }
  }

  function escapeHtml(s) {
    return String(s || '').replace(/[&<>"']/g, (c) =>
      ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' })[c]);
  }

  function init() {
    $('#dash-adr-search').addEventListener('input', (e) => refreshAdrs(e.target.value));
    $('#dash-atam-search').addEventListener('input', (e) => refreshAtams(e.target.value));
    $('#btn-new-adr').addEventListener('click', () => { ui.showView('adr'); ADR.createNew(); });
    $('#btn-new-atam').addEventListener('click', () => { ui.showView('atam'); ATAM.createNew(); });
    window.addEventListener('view:activated', (e) => {
      if (e.detail === 'dashboard') {
        refreshAdrs(); refreshAtams(); refreshAi();
      }
    });
  }

  return { init, refreshAdrs, refreshAtams, refreshAi, escapeHtml };
})();
