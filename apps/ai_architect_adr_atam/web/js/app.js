// Entry point — wires up tab switching and bootstraps each module.
(function () {
  document.querySelectorAll('.tab').forEach((t) =>
    t.addEventListener('click', () => ui.showView(t.dataset.view)));

  Dashboard.init();
  ADR.init();
  ATAM.init();
  AiPanel.init();

  // Initial view
  ui.showView('dashboard');
})();
