// Thin JSON fetch wrapper around the local server.
const api = (() => {
  async function request(method, path, body, opts = {}) {
    const init = { method, headers: { 'Content-Type': 'application/json' } };
    if (body !== undefined) init.body = JSON.stringify(body);
    if (opts.signal) init.signal = opts.signal;
    const res = await fetch(path, init);
    const isJson = (res.headers.get('content-type') || '').includes('application/json');
    const payload = isJson ? await res.json() : await res.text();
    if (!res.ok) {
      const msg = (isJson && payload && payload.error) ? payload.error : `HTTP ${res.status}`;
      const err = new Error(msg);
      err.status = res.status;
      err.payload = payload;
      throw err;
    }
    return payload;
  }
  return {
    get: (p) => request('GET', p),
    post: (p, b) => request('POST', p, b || {}),
    put: (p, b) => request('PUT', p, b || {}),
    del: (p) => request('DELETE', p),
    // Raw GET returning text (for markdown export preview)
    getText: async (p) => {
      const res = await fetch(p);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return res.text();
    },
  };
})();
