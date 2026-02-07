// Helper function to format money with thousand separators
function formatMoney(value) {
    return '$' + value.toFixed(2).replace(/\B(?=(\d{3})+(?!\d))/g, ',');
}

// Helper function to format number with 2 decimals
function formatNumber(value) {
    return value.toFixed(2);
}

// Helper function to show results
function showResult(elementId, html, isError = false) {
    const element = document.getElementById(elementId);
    element.innerHTML = html;
    element.className = 'result ' + (isError ? 'error' : 'success');
}

// Helper function to make API calls
async function apiCall(endpoint, data) {
    try {
        const response = await fetch(`http://localhost:8080/api/${endpoint}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        });

        return await response.json();
    } catch (error) {
        return { error: 'Failed to connect to server. Make sure the server is running.' };
    }
}

// GenAI ValueScore Calculator
async function calculateValueScore() {
    const scale = parseFloat(document.getElementById('vs-scale').value);
    const frequency = parseFloat(document.getElementById('vs-frequency').value);
    const manualComplexity = parseFloat(document.getElementById('vs-complexity').value);

    if (!scale || !frequency || !manualComplexity) {
        showResult('vs-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('valueScore', {
        scale,
        frequency,
        manualComplexity
    });

    if (result.error) {
        showResult('vs-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>ValueScore:</strong>
                <span class="result-value">${formatNumber(result.valueScore)}</span>
            </div>
        `;
        showResult('vs-result', html);
    }
}

// PERT Calculator
async function calculatePERT() {
    const optimistic = parseFloat(document.getElementById('pert-optimistic').value);
    const mostLikely = parseFloat(document.getElementById('pert-mostlikely').value);
    const pessimistic = parseFloat(document.getElementById('pert-pessimistic').value);
    const unit = document.getElementById('pert-unit').value;

    if (isNaN(optimistic) || isNaN(mostLikely) || isNaN(pessimistic)) {
        showResult('pert-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('pert', {
        optimistic,
        mostLikely,
        pessimistic,
        unit
    });

    if (result.error) {
        showResult('pert-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Expected (E):</strong>
                <span class="result-value">${formatNumber(result.expected)} ${result.unit}</span>
            </div>
            <div class="result-line">
                <strong>Sigma (σ):</strong>
                <span class="result-value">${formatNumber(result.sigma)} ${result.unit}</span>
            </div>
            <div class="result-line">
                <strong>Upper 95%:</strong>
                <span class="result-value">${formatNumber(result.upper95)} ${result.unit}</span>
            </div>
        `;
        showResult('pert-result', html);
    }
}

// Risk Exposure Calculator
async function calculateRisk() {
    const probabilityPercent = parseFloat(document.getElementById('risk-probability').value);
    const impact = parseFloat(document.getElementById('risk-impact').value);
    const period = document.getElementById('risk-period').value || '/month';

    if (isNaN(probabilityPercent) || isNaN(impact)) {
        showResult('risk-result', 'Please fill in all required fields.', true);
        return;
    }

    const result = await apiCall('risk', {
        probabilityPercent,
        impact,
        period
    });

    if (result.error) {
        showResult('risk-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Risk Exposure:</strong>
                <span class="result-value">${formatMoney(result.exposure)}${result.period}</span>
            </div>
        `;
        showResult('risk-result', html);
    }
}

// TCO Calculator
async function calculateTCO() {
    const capEx = parseFloat(document.getElementById('tco-capex').value);
    const opExMonthly = parseFloat(document.getElementById('tco-opex').value);
    const months = parseInt(document.getElementById('tco-months').value);

    if (isNaN(capEx) || isNaN(opExMonthly) || isNaN(months)) {
        showResult('tco-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('tco', {
        capEx,
        opExMonthly,
        months
    });

    if (result.error) {
        showResult('tco-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Total Cost of Ownership:</strong>
                <span class="result-value">${formatMoney(result.tco)}</span>
            </div>
        `;
        showResult('tco-result', html);
    }
}

// Budget with Reserves Calculator
async function calculateBudget() {
    const baseBudget = parseFloat(document.getElementById('budget-base').value);
    const knownReserve = parseFloat(document.getElementById('budget-known').value);
    const unknownPercent = parseFloat(document.getElementById('budget-unknown').value);

    if (isNaN(baseBudget) || isNaN(knownReserve) || isNaN(unknownPercent)) {
        showResult('budget-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('budget', {
        baseBudget,
        knownReserve,
        unknownPercent
    });

    if (result.error) {
        showResult('budget-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Unknown Reserve:</strong>
                <span class="result-value">${formatMoney(result.unknownReserve)}</span>
            </div>
            <div class="result-line">
                <strong>Total Budget:</strong>
                <span class="result-value">${formatMoney(result.totalBudget)}</span>
            </div>
        `;
        showResult('budget-result', html);
    }
}

// Little's Law (Concurrency) Calculator
async function calculateLittlesLaw() {
    const rps = parseFloat(document.getElementById('ll-rps').value);
    const latency = parseFloat(document.getElementById('ll-latency').value);

    if (isNaN(rps) || isNaN(latency)) {
        showResult('ll-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('littlesLaw', { rps, latency });

    if (result.error) {
        showResult('ll-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Concurrency (L):</strong>
                <span class="result-value">${formatNumber(result.concurrency)} concurrent requests</span>
            </div>
        `;
        showResult('ll-result', html);
    }
}

// LLM Latency Calculator
async function calculateLLMLatency() {
    const timePerToken = parseFloat(document.getElementById('llm-tpt').value);
    const tokensCount = parseInt(document.getElementById('llm-tokens').value);
    const prefillTime = parseFloat(document.getElementById('llm-prefill').value) || 0;

    if (isNaN(timePerToken) || isNaN(tokensCount)) {
        showResult('llm-result', 'Please fill in all required fields.', true);
        return;
    }

    const result = await apiCall('llmLatency', { tokensCount, timePerToken, prefillTime });

    if (result.error) {
        showResult('llm-result', `Error: ${result.error}`, true);
    } else {
        let html = `
            <div class="result-line">
                <strong>Generation Time:</strong>
                <span class="result-value">${result.generationTimeSec} sec</span>
            </div>
        `;
        if (prefillTime > 0) {
            html += `
                <div class="result-line">
                    <strong>Total Time (with prefill):</strong>
                    <span class="result-value">${result.totalTimeSec} sec</span>
                </div>
            `;
        }
        showResult('llm-result', html);
    }
}

// Vector DB Storage Sizing Calculator
async function calculateVectorStorage() {
    const pages = parseInt(document.getElementById('vec-pages').value);
    const chunksPerPage = parseInt(document.getElementById('vec-chunks').value);
    const vectorDim = parseInt(document.getElementById('vec-dim').value);
    const bytesPerFloat = parseInt(document.getElementById('vec-bytes').value);

    if (isNaN(pages) || isNaN(chunksPerPage) || isNaN(vectorDim)) {
        showResult('vec-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('vectorStorage', { pages, chunksPerPage, vectorDim, bytesPerFloat });

    if (result.error) {
        showResult('vec-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Total Vectors:</strong>
                <span class="result-value">${result.totalVectors.toLocaleString()}</span>
            </div>
            <div class="result-line">
                <strong>Vector Size:</strong>
                <span class="result-value">${formatNumber(result.vectorSizeKB)} KB</span>
            </div>
            <div class="result-line">
                <strong>Total Storage:</strong>
                <span class="result-value">~${formatNumber(result.totalSizeMB)} MB</span>
            </div>
        `;
        showResult('vec-result', html);
    }
}

// Log Storage (Observability) Calculator
async function calculateLogStorage() {
    const rps = parseFloat(document.getElementById('log-rps').value);
    const logSizeKB = parseFloat(document.getElementById('log-size').value);
    const retentionDays = parseInt(document.getElementById('log-days').value);

    if (isNaN(rps) || isNaN(logSizeKB)) {
        showResult('log-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('logStorage', { rps, logSizeKB, retentionDays });

    if (result.error) {
        showResult('log-result', `Error: ${result.error}`, true);
    } else {
        const html = `
            <div class="result-line">
                <strong>Events per Day:</strong>
                <span class="result-value">~${Number(result.eventsPerDay).toLocaleString()}</span>
            </div>
            <div class="result-line">
                <strong>Daily Volume:</strong>
                <span class="result-value">~${formatNumber(result.dailySizeGB)} GB</span>
            </div>
            <div class="result-line">
                <strong>Total (${retentionDays} days):</strong>
                <span class="result-value">~${formatNumber(result.totalSizeGB)} GB</span>
            </div>
        `;
        showResult('log-result', html);
    }
}

// Scalability Check (RAM) Calculator
async function calculateScalability() {
    const rps = parseFloat(document.getElementById('sc-rps').value);
    const latency = parseFloat(document.getElementById('sc-latency').value);
    const ramPerWorkerMB = parseFloat(document.getElementById('sc-ram-worker').value);
    const availableRAM_GB = parseFloat(document.getElementById('sc-ram-available').value);

    if (isNaN(rps) || isNaN(latency) || isNaN(ramPerWorkerMB) || isNaN(availableRAM_GB)) {
        showResult('sc-result', 'Please fill in all fields.', true);
        return;
    }

    const result = await apiCall('scalability', { rps, latency, ramPerWorkerMB, availableRAM_GB });

    if (result.error) {
        showResult('sc-result', `Error: ${result.error}`, true);
    } else {
        const verdict = result.willCrash
            ? '<span style="color: #dc3545; font-weight: bold;">CRASH! Not enough RAM</span>'
            : '<span style="color: #28a745; font-weight: bold;">OK - fits in available RAM</span>';
        const html = `
            <div class="result-line">
                <strong>Concurrency (L):</strong>
                <span class="result-value">${formatNumber(result.concurrency)} workers</span>
            </div>
            <div class="result-line">
                <strong>Required RAM:</strong>
                <span class="result-value">${formatNumber(result.requiredRAM_GB)} GB</span>
            </div>
            <div class="result-line">
                <strong>Available RAM:</strong>
                <span class="result-value">${result.availableRAM_GB} GB</span>
            </div>
            <div class="result-line">
                <strong>Verdict:</strong> ${verdict}
            </div>
        `;
        showResult('sc-result', html);
    }
}

// Add Enter key support for all calculators
document.addEventListener('DOMContentLoaded', function() {
    // ValueScore
    ['vs-scale', 'vs-frequency', 'vs-complexity'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateValueScore();
        });
    });

    // PERT
    ['pert-optimistic', 'pert-mostlikely', 'pert-pessimistic'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculatePERT();
        });
    });

    // Risk
    ['risk-probability', 'risk-impact', 'risk-period'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateRisk();
        });
    });

    // TCO
    ['tco-capex', 'tco-opex', 'tco-months'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateTCO();
        });
    });

    // Budget
    ['budget-base', 'budget-known', 'budget-unknown'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateBudget();
        });
    });

    // Little's Law
    ['ll-rps', 'll-latency'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateLittlesLaw();
        });
    });

    // LLM Latency
    ['llm-tpt', 'llm-tokens', 'llm-prefill'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateLLMLatency();
        });
    });

    // Vector Storage
    ['vec-pages', 'vec-chunks', 'vec-dim'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateVectorStorage();
        });
    });

    // Log Storage
    ['log-rps', 'log-size', 'log-days'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateLogStorage();
        });
    });

    // Scalability Check
    ['sc-rps', 'sc-latency', 'sc-ram-worker', 'sc-ram-available'].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateScalability();
        });
    });
});
