const AiPanel = (() => {
  const { $ } = ui;
  const defaultModels = {
    gemini: 'gemma-4-26b-a4b-it',
    ollama: 'gemma4:e2b',
  };
  let lastProvider = 'gemini';

  function syncProviderFields() {
    const provider = $('#cfg-provider').value || 'gemini';
    const isGemini = provider === 'gemini';
    $('#cfg-host').disabled = isGemini;
    $('#cfg-port').disabled = isGemini;
    $('#cfg-api-key-note').textContent = isGemini
      ? 'Gemini reads GEMINI_API_KEY from .env.'
      : 'Ollama runs locally and does not use an API key.';
    const model = $('#cfg-model');
    if (!model.value || model.value === defaultModels[lastProvider]) {
      model.value = defaultModels[provider];
    }
    lastProvider = provider;
  }

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
      const provider = st.provider || 'gemini';
      $('#cfg-provider').value = provider;
      $('#cfg-host').value = st.host || '127.0.0.1';
      $('#cfg-port').value = st.port || 11434;
      $('#cfg-model').value = st.activeModel || defaultModels[provider] || defaultModels.gemini;
      $('#cfg-enabled').checked = !!st.enabled;
      lastProvider = provider;
      syncProviderFields();
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
      provider: $('#cfg-provider').value || 'gemini',
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
    $('#cfg-provider').addEventListener('change', syncProviderFields);
    $('#cfg-save').addEventListener('click', (ev) => { ev.preventDefault(); save(); $('#ai-config-dialog').close(); });
    refresh();
    setInterval(refresh, 15000);
  }

  return { init, refresh };
})();
