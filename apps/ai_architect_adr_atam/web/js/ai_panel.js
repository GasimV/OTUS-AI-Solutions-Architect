const AiPanel = (() => {
  const { $ } = ui;

  async function refresh() {
    try {
      const st = await api.get('/api/ai/status');
      const dot = $('#ai-dot'); const label = $('#ai-label');
      if (!st.enabled) { dot.className = 'dot warn'; label.textContent = 'AI: disabled'; return; }
      if (st.reachable && st.modelAvailable) {
        dot.className = 'dot ok'; label.textContent = 'AI: ready (' + st.activeModel + ')';
      } else if (st.reachable) {
        dot.className = 'dot warn'; label.textContent = 'AI: model missing';
      } else {
        dot.className = 'dot err'; label.textContent = 'AI: offline';
      }
      // pre-fill config dialog
      $('#cfg-host').value = st.host || '127.0.0.1';
      $('#cfg-port').value = st.port || 11434;
      $('#cfg-model').value = st.activeModel || 'gemma4:e2b';
      $('#cfg-enabled').checked = !!st.enabled;
    } catch (e) {
      $('#ai-dot').className = 'dot err';
      $('#ai-label').textContent = 'AI: unknown';
    }
  }

  function openConfig() {
    const dlg = $('#ai-config-dialog');
    if (dlg && typeof dlg.showModal === 'function') dlg.showModal();
  }

  async function save() {
    const cfg = {
      host: $('#cfg-host').value,
      port: parseInt($('#cfg-port').value || '11434', 10),
      model: $('#cfg-model').value,
      enabled: $('#cfg-enabled').checked,
    };
    try {
      await api.post('/api/ai/config', cfg);
      ui.ok('AI config saved');
      refresh();
    } catch (e) { ui.err('Save failed: ' + e.message); }
  }

  function init() {
    $('#ai-status').addEventListener('click', openConfig);
    $('#cfg-save').addEventListener('click', (ev) => { ev.preventDefault(); save(); $('#ai-config-dialog').close(); });
    refresh();
    setInterval(refresh, 15000);
  }

  return { init, refresh };
})();
