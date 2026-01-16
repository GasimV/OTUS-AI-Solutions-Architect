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
});
