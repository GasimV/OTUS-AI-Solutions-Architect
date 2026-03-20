// Helper function to format money with thousand separators
function formatMoney(value) {
    return '$' + value.toFixed(2).replace(/\B(?=(\d{3})+(?!\d))/g, ',');
}

// Helper function to format number with 2 decimals
function formatNumber(value) {
    return value.toFixed(2);
}

// Helper function to format whole numbers
function formatInteger(value) {
    return Number(value).toLocaleString();
}

function getStatusClass(status) {
    return String(status || '').toLowerCase();
}

function getPriorityClass(priority) {
    return String(priority || '').toLowerCase();
}

const inlineInputHelpText = {
    'vs-scale': 'How broad the business impact is if this use case succeeds. Example: 1 to 3 for niche internal tasks, 7 to 10 for org-wide or customer-facing impact.',
    'vs-frequency': 'How often the workflow happens in real life. Example: 1 to 3 for occasional use, 7 to 10 for daily or continuous operations.',
    'vs-complexity': 'How hard or costly the current manual process is. Example: 1 to 3 for simple work, 7 to 10 for slow, expert-heavy, or error-prone work.',

    'pert-optimistic': 'Best-case estimate if things go unusually smoothly. Example: use the shortest realistic duration, not an impossible ideal.',
    'pert-mostlikely': 'Your normal expected estimate under typical conditions. Example: the duration the team would most confidently commit to first.',
    'pert-pessimistic': 'Worst-case estimate when known risks and delays materialize. Example: use a realistic upper bound, not a disaster scenario.',
    'pert-unit': 'Time unit for the estimate results. Example: choose days for shorter tasks and weeks for larger work items.',

    'risk-probability': 'Chance the risk event happens, expressed as a percentage. Example: 10% for unlikely risks, 50% for uncertain risks, 80%+ for highly likely risks.',
    'risk-impact': 'Business or financial loss if the risk happens once. Example: $1,000 for minor incidents or $50,000+ for high-impact outages.',
    'risk-period': 'Label appended to the output so the exposure is easier to interpret. Example: use /month, /quarter, or /year.',

    'tco-capex': 'Upfront one-time spend for the standalone TCO estimate. Example: hardware purchase, migration cost, or initial platform setup.',
    'tco-opex': 'Recurring monthly operating cost for the standalone TCO estimate. Example: hosting, support, licenses, and operations labor.',
    'tco-months': 'How long the standalone TCO calculation should run. Example: 12, 24, or 36 months are common planning horizons.',

    'budget-base': 'Planned baseline budget before adding any reserves. Example: the committed delivery budget for the project itself.',
    'budget-known': 'Reserve already identified for known risks or scope items. Example: vendor overages, compliance work, or migration buffers.',
    'budget-unknown': 'Percentage reserve for unknown unknowns. Example: 10% to 30% is a common early-stage planning range.',

    'll-rps': 'Arrival rate used in Little’s Law, in requests per second. Example: 20 RPS for internal services or hundreds for public APIs.',
    'll-latency': 'Average time each request spends in the system, in seconds. Example: 0.05 to 1.0 seconds for many online systems.',

    'llm-tpt': 'Average generation time per output token in milliseconds. Example: 10 to 50 ms/token depending on model size and hardware.',
    'llm-tokens': 'Number of output tokens the model is expected to generate. Example: 50 for short answers, 200 to 1,000 for richer outputs.',
    'llm-prefill': 'Initial prompt processing time before token generation starts. Example: 0 for a rough estimate or 100 to 1,000 ms for long prompts.',

    'vec-pages': 'Number of source pages or documents being embedded. Example: 100 for a small corpus, thousands or more for knowledge bases.',
    'vec-chunks': 'Average chunks produced from each page or document. Example: 5 to 20 chunks per page depending on chunk size and overlap.',
    'vec-dim': 'Embedding dimension produced by your vector model. Example: 384, 768, 1024, or 1536 are common values.',
    'vec-bytes': 'Data type used for each vector element in the storage sizing estimate. Example: float16 uses less space than float32.',

    'log-rps': 'Average request or event rate feeding the log volume estimate. Example: 10 to 500 RPS for many application services.',
    'log-size': 'Typical size of one log record in kilobytes. Example: 0.5 to 5 KB for structured application logs.',
    'log-days': 'How long the logs should be retained. Example: 7, 30, or 90 days depending on compliance and debugging needs.',

    'sc-rps': 'Peak request rate used for the RAM stress check. Example: choose the highest realistic burst the server may see.',
    'sc-latency': 'Average request time in seconds used to estimate concurrency. Example: 0.05 to 2 seconds depending on workload.',
    'sc-ram-worker': 'Approximate memory used by one concurrent worker or request in MB. Example: 10 to 200 MB depending on the stack.',
    'sc-ram-available': 'Usable RAM available on the server for this workload. Example: 4, 8, 16, or 64 GB after OS reservation.',

    'sizing-component-type': 'Choose the infrastructure component you want to estimate. Example: use Stateless service for APIs, SQL for relational databases, and VRAM needs for LLM inference planning.',
    'sizing-safety-factor': 'Extra headroom added on top of calculated resources for uncertainty and growth. Typical values are 1.2 to 1.5; 1.3 is a practical default.',
    'sizing-peak-rps': 'Highest request rate the component must handle during peak load, not the daily average. Example: 200 to 2,000 RPS for many internal services; public APIs may be much higher.',
    'sizing-latency': 'Average end-to-end response time per request in milliseconds. Example: 20 to 80 ms for cache-backed services, 100 to 300 ms for heavier data workflows.',
    'sizing-req-kb': 'Typical incoming payload size for one request. Example: 1 to 8 KB for JSON APIs; file or embedding requests can be much larger.',
    'sizing-resp-kb': 'Typical outgoing payload size for one response. Example: 2 to 32 KB for common APIs; search or analytics results may be larger.',
    'sizing-replicas': 'Number of copies or nodes storing or serving the component data or traffic. Example: 2 to 3 for HA, more for large clusters.',
    'sizing-peak-factor': 'Multiplier for burstiness above your stated peak assumptions. Typical values are 1.2 to 2.0; 1.5 is a common conservative choice.',

    'stateless-cpu-seconds': 'CPU time consumed by one request across all cores. Example: 0.001 to 0.02 seconds per request for many API handlers.',
    'stateless-base-ram': 'Baseline RAM needed even with little traffic, such as runtime, libraries, and caches. Example: 1 to 8 GB for many app services.',
    'stateless-mem-request': 'Extra memory held while one request is in flight. Example: 0.5 to 10 MB depending on payloads, batching, and framework behavior.',
    'stateless-vcpu-node': 'vCPU capacity of one application node or instance. Example: 2, 4, 8, or 16 vCPU.',
    'stateless-ram-node': 'RAM capacity of one application node or instance. Example: 4, 8, 16, or 32 GB.',

    'sql-data-gb': 'Total primary data volume stored in the database before indexes and overhead. Example: 100 to 5,000 GB for many production systems.',
    'sql-hot-data-gb': 'Working set that must stay fast in memory or cache for good query performance. Example: 10% to 40% of total data is often treated as hot.',
    'sql-read-rps': 'Read queries or requests per second hitting the SQL tier. Example: 100 to 5,000 read RPS depending on workload.',
    'sql-write-rps': 'Write queries or transactions per second hitting the SQL tier. Example: 10 to 1,000 write RPS in many OLTP systems.',
    'sql-cpu-read': 'Average CPU seconds needed for one read request. Example: 0.0005 to 0.01 seconds depending on indexing and query complexity.',
    'sql-cpu-write': 'Average CPU seconds needed for one write request. Example: 0.001 to 0.02 seconds depending on validation, indexes, and transactions.',
    'sql-index-ratio': 'Index size relative to raw data size. Example: 0.3 to 1.0 is common; analytics-heavy schemas can exceed that.',
    'sql-wal-gb': 'Space reserved for WAL, redo, or journal logs. Example: 10 to 200 GB depending on write volume and recovery policy.',
    'sql-temp-gb': 'Temporary disk space used by sorts, joins, and query spill. Example: 10 to 100 GB for mixed workloads.',
    'sql-query-workspace-gb': 'RAM reserved for active query execution and internal buffers. Example: 4 to 64 GB depending on concurrency and query shape.',
    'sql-os-reserve-gb': 'Memory left for the OS and non-database processes on the host. Example: 2 to 16 GB.',
    'sql-cache-coverage': 'Fraction of hot data you expect memory cache to cover. Example: 0.5 to 0.9 for well-tuned production systems.',

    'nosql-data-gb': 'Total dataset stored across the NoSQL cluster before replication and overhead. Example: 500 to 20,000 GB.',
    'nosql-hot-data-gb': 'Frequently accessed subset that benefits from memory caching. Example: 10% to 50% of total data.',
    'nosql-read-rps': 'Read operations per second across the NoSQL cluster. Example: 500 to 20,000 read RPS.',
    'nosql-write-rps': 'Write operations per second across the NoSQL cluster. Example: 100 to 10,000 write RPS.',
    'nosql-rps-shard': 'Throughput one shard can safely handle before you add more shards. Example: 300 to 2,000 RPS per shard, depending on software and hardware.',
    'nosql-data-shard': 'Preferred data volume per shard for manageable balancing and repair time. Example: 100 to 500 GB per shard.',
    'nosql-cpu-read': 'Average CPU seconds consumed by one read operation. Example: 0.0003 to 0.005 seconds.',
    'nosql-cpu-write': 'Average CPU seconds consumed by one write operation. Example: 0.0005 to 0.01 seconds.',
    'nosql-index-ratio': 'Secondary-index overhead relative to raw data. Example: 0.1 to 0.5 for many key-value or wide-column setups.',
    'nosql-compaction-ratio': 'Extra disk overhead reserved for compaction, merge, or repair activity. Example: 0.2 to 0.5 is common.',
    'nosql-cache-ratio': 'Fraction of hot data targeted to stay in RAM. Example: 0.2 to 0.6 depending on cache policy.',
    'nosql-memtable-gb': 'Memory reserved for write buffers, memtables, or flush staging. Example: 2 to 32 GB per node or cluster estimate.',
    'nosql-os-reserve-gb': 'Memory kept for the OS and background services. Example: 2 to 16 GB.',

    'vector-num-vectors': 'Total number of embeddings or vectors stored. Example: 1 million to 1 billion vectors depending on product scale.',
    'vector-dimension': 'Embedding width for each vector. Example: 384, 768, 1024, or 1536 are common model output sizes.',
    'vector-precision': 'Numeric precision used to store vector values. Example: fp16 reduces memory versus fp32; int8 or int4 trade memory for recall and implementation constraints.',
    'vector-metadata-bytes': 'Average metadata stored per vector outside the embedding itself. Example: 64 to 512 bytes for IDs, tags, timestamps, and routing fields.',
    'vector-index-ratio': 'Approximate index size relative to raw vector data. Example: 0.5 to 2.0 depending on HNSW, IVF, and tuning choices.',
    'vector-resident-fraction': 'Fraction of raw vector data you want resident in RAM for performance. Example: 0.3 to 1.0 depending on workload and storage design.',
    'vector-index-resident-fraction': 'Fraction of the vector index you want resident in RAM. Example: 0.5 to 1.0 for latency-sensitive search.',
    'vector-peak-qps': 'Peak similarity-search queries per second. Example: 20 to 1,000 QPS for many retrieval systems.',
    'vector-candidates-scanned': 'Approximate number of candidate vectors examined per query. Example: 100 to 5,000 depending on recall target and index settings.',
    'vector-ops-core-sec': 'How many vector math operations one CPU core can process each second on your target hardware. Example: tens to hundreds of millions per core per second.',
    'vector-query-buffer-gb': 'Memory reserved for active query buffers and temporary search state. Example: 2 to 16 GB.',
    'vector-wal-gb': 'Disk space for write-ahead or durability logs. Example: 10 to 100 GB.',
    'vector-snapshot-gb': 'Extra disk space reserved for snapshots or backups. Example: 20 to 200 GB or more for large stores.',
    'vector-os-reserve-gb': 'Memory left for the host OS and side processes. Example: 2 to 16 GB.',

    'cache-hot-data-gb': 'Hot working set you expect the cache to hold in memory. Example: 10 to 500 GB for many application caches.',
    'cache-serialization-overhead': 'Multiplier for object wrappers, metadata, and serialization inefficiency. Example: 1.1 to 1.5 for many Redis-like caches.',
    'cache-eviction-headroom': 'Extra memory reserved so the cache does not run at its absolute limit. Example: 1.1 to 1.3 is typical.',
    'cache-rps-core': 'Requests per second one CPU core can process on the cache tier. Example: 2,000 to 20,000 depending on item size and network overhead.',

    'logging-events-sec': 'Log or telemetry events produced each second. Example: 100 to 10,000 events per second for medium systems.',
    'logging-avg-kb': 'Average payload size of a single log event. Example: 0.5 to 5 KB for structured logs; traces can be larger.',
    'logging-retention-days': 'How long logs should remain queryable or retained. Example: 7, 30, 90, or 365 days.',
    'logging-replication-factor': 'How many copies of log data are stored for durability and availability. Example: 2 or 3 replicas.',
    'logging-overhead-ratio': 'Extra storage overhead for indexing, parsing, and internal processing. Example: 0.2 to 1.0 depending on the platform.',

    'vram-model-parameters': 'Total parameter count of the model. Example: 7B, 13B, 32B, or 70B parameters.',
    'vram-precision-type': 'Precision used for model weights and cache estimates. Example: fp16 is common for inference; int8 or int4 reduce VRAM at the cost of quantization tradeoffs.',
    'vram-batch-size': 'Number of requests or sequences processed together on the GPU. Example: batch 1 for interactive chat; 4 to 32 for throughput-oriented workloads.',
    'vram-context-length': 'Maximum prompt or sequence length used during inference. Example: 2048, 4096, 8192, or higher for long-context models.',
    'vram-hidden-size': 'Hidden dimension of the transformer blocks. Example: around 4096 for many 7B-class models, larger for bigger models.',
    'vram-num-layers': 'Number of transformer layers in the model. Example: around 32 for many 7B models and 80+ for larger models.',
    'vram-architecture-type': 'High-level architecture used to choose the activation estimate method. Example: decoder_only for GPT/Llama-style models; choose fallback types if you lack exact metadata.',
    'vram-num-attention-heads': 'Total attention heads in each transformer layer. Example: 32 or 40 for many decoder-only models.',
    'vram-num-kv-heads': 'Number of key/value heads used for GQA or MQA. Example: 8 KV heads with 32 attention heads gives a 0.25 GQA ratio; leave blank to default to full attention heads.',

    'sizing-include-tco': 'Enable on-prem versus cloud cost comparison alongside the sizing estimate. Use it when you want rough financial tradeoffs in the same calculation.',
    'tco-hardware-capex': 'Upfront cost of compute servers or nodes for an on-prem deployment. Example: $20,000 to $500,000 depending on scale and GPU usage.',
    'tco-storage-capex': 'Upfront spend for storage hardware such as SSD, NVMe, or SAN capacity. Example: $5,000 to $200,000.',
    'tco-network-capex': 'Upfront spend for switches, NICs, firewalls, or related network gear. Example: $2,000 to $100,000.',
    'tco-annual-power': 'Yearly electricity cost for running the infrastructure. Example: a few thousand dollars for small fleets, much more for GPU-heavy systems.',
    'tco-annual-cooling': 'Yearly cooling or data-center environmental cost. Example: often similar order of magnitude to power for dense deployments.',
    'tco-annual-support': 'Yearly vendor support, maintenance, and hardware service contracts. Example: 5% to 15% of hardware spend per year.',
    'tco-annual-licensing': 'Yearly software, database, OS, or platform license cost. Example: $0 for open source to six figures for enterprise stacks.',
    'tco-annual-staff': 'Yearly staffing cost to operate the platform. Example: one platform engineer might be $50,000 to $200,000+ depending on region.',
    'tco-annual-downtime': 'Estimated yearly business cost from outages or degraded service. Example: $1,000 to $100,000+ depending on criticality.',
    'tco-years': 'Planning horizon for the on-prem total cost calculation. Example: 3 to 5 years is common.',
    'tco-monthly-compute': 'Monthly cloud spend for compute instances or containers. Example: $1,000 to $100,000+ depending on scale.',
    'tco-monthly-storage': 'Monthly cloud storage cost for primary data. Example: tens to thousands of dollars depending on capacity and tier.',
    'tco-monthly-backup': 'Monthly cloud backup and snapshot cost. Example: $50 to several thousand dollars.',
    'tco-monthly-egress': 'Monthly outbound network transfer cost. Example: near zero for internal systems, much higher for public data-heavy services.',
    'tco-monthly-managed': 'Monthly managed-service premium beyond raw compute and storage. Example: database, observability, or queueing service charges.',
    'tco-monthly-support': 'Monthly paid support plan from the cloud vendor. Example: basic plans may be small; enterprise plans can be substantial.',
    'tco-monthly-staff': 'Monthly labor cost to operate the cloud environment. Example: part of an engineer month to several full-time staff.',
    'tco-monthly-downtime': 'Estimated monthly business loss from incidents in the cloud model. Example: use recent incident history or SLA risk assumptions.',
    'tco-months-sizing': 'Planning horizon for the cloud TCO calculation in months. Example: 12, 24, or 36 months.',

    'load-expected-rps': 'Traffic level the service was planned or sized to handle. Example: 500 to 5,000 RPS for many backend services.',
    'load-actual-rps': 'Real observed request rate under current load. Example: if expected is 1,000 and actual is 1,300, the service is above plan.',
    'load-service-type': 'Whether the service is mostly stateless or keeps state tightly coupled to storage or sessions. Example: APIs are often stateless; databases are stateful.',
    'load-cpu-usage': 'Observed CPU utilization under current load. Example: 40% to 70% is often comfortable; 85%+ usually deserves attention.',
    'load-cpu-threshold': 'CPU limit you consider operationally safe before scaling or tuning. Example: many teams use 70% to 80%.',
    'load-ram-usage': 'Observed memory usage of the service or tier. Example: 8 to 24 GB for app nodes, much more for data platforms.',
    'load-ram-threshold': 'Memory level you treat as the safe ceiling before paging or OOM risk becomes uncomfortable. Example: keep below 70% to 85% of real capacity.',
    'load-disk-iops': 'Current read/write IOPS consumed by the workload. Example: 1,000 to 10,000 IOPS for many transactional systems.',
    'load-disk-threshold': 'IOPS limit you consider safe for the storage layer. Example: use measured storage capability, not theoretical vendor maximums.',
    'load-latency-ms': 'Observed average request latency in milliseconds. Example: 50 to 200 ms is common for user-facing APIs, depending on the SLA.',
    'load-latency-sla-ms': 'Latency objective or SLA target used as the limit. Example: 100 ms, 200 ms, or 500 ms depending on the service.',
    'load-error-rate': 'Observed percentage of failed or unhealthy requests. Example: 0.1% to 1.0% may be acceptable depending on the service and error budget.',
    'load-error-budget': 'Maximum error rate tolerated before the service is considered out of budget. Example: 0.1%, 0.5%, or 1.0%.'
};

function closeInlineInputHelp() {
    document.querySelectorAll('.input-help.is-open, .input-help.is-hover-preview').forEach(wrapper => {
        wrapper.classList.remove('is-open');
        wrapper.classList.remove('is-hover-preview');
        wrapper.style.removeProperty('--tooltip-offset');
        wrapper.style.removeProperty('--tooltip-width');
        const trigger = wrapper.querySelector('.input-help-trigger');
        if (trigger) {
            trigger.setAttribute('aria-expanded', 'false');
        }
    });
}

function positionInlineInputHelp(wrapper) {
    const tooltip = wrapper.querySelector('.input-help-tooltip');
    const card = wrapper.closest('.calculator-card');

    if (!tooltip || !card) {
        return;
    }

    const cardRect = card.getBoundingClientRect();
    const maxWidth = Math.max(180, Math.min(280, cardRect.width - 32));
    wrapper.style.setProperty('--tooltip-width', `${maxWidth}px`);

    const wasClosed = window.getComputedStyle(tooltip).display === 'none';
    if (wasClosed) {
        tooltip.style.display = 'block';
        tooltip.style.visibility = 'hidden';
    }

    tooltip.style.left = '0px';
    const tooltipRect = tooltip.getBoundingClientRect();
    const padding = 12;
    let offset = 0;

    if (tooltipRect.right > cardRect.right - padding) {
        offset -= tooltipRect.right - (cardRect.right - padding);
    }
    if (tooltipRect.left + offset < cardRect.left + padding) {
        offset += (cardRect.left + padding) - (tooltipRect.left + offset);
    }

    wrapper.style.setProperty('--tooltip-offset', `${Math.round(offset)}px`);

    if (wasClosed) {
        tooltip.style.display = '';
        tooltip.style.visibility = '';
    }
}

function toggleInlineInputHelp(wrapper) {
    const trigger = wrapper.querySelector('.input-help-trigger');
    const shouldOpen = !wrapper.classList.contains('is-open');

    closeInlineInputHelp();

    if (shouldOpen) {
        wrapper.classList.add('is-open');
        positionInlineInputHelp(wrapper);
        if (trigger) {
            trigger.setAttribute('aria-expanded', 'true');
        }
    }
}

function attachInlineInputHelp(helpMap) {
    Object.entries(helpMap).forEach(([inputId, helpText]) => {
        const label = document.querySelector(`label[for="${inputId}"]`);
        if (!label || label.dataset.helpEnhanced === 'true') {
            return;
        }

        const wrapper = document.createElement('span');
        wrapper.className = 'input-help';

        const trigger = document.createElement('button');
        trigger.type = 'button';
        trigger.className = 'input-help-trigger';
        trigger.textContent = 'i';
        trigger.setAttribute('aria-label', `More info about ${label.textContent.replace(/\s+/g, ' ').trim()}`);
        trigger.setAttribute('aria-expanded', 'false');

        const tooltip = document.createElement('span');
        tooltip.className = 'input-help-tooltip';
        tooltip.id = `help-${inputId}`;
        tooltip.setAttribute('role', 'tooltip');
        tooltip.textContent = helpText;

        trigger.setAttribute('aria-describedby', tooltip.id);

        const showHoverPreview = () => {
            if (!wrapper.classList.contains('is-open')) {
                wrapper.classList.add('is-hover-preview');
            }
            positionInlineInputHelp(wrapper);
        };

        const hideHoverPreview = () => {
            wrapper.classList.remove('is-hover-preview');
            if (!wrapper.classList.contains('is-open')) {
                wrapper.style.removeProperty('--tooltip-offset');
                wrapper.style.removeProperty('--tooltip-width');
            }
        };

        label.addEventListener('pointerenter', showHoverPreview);
        label.addEventListener('pointerleave', hideHoverPreview);
        wrapper.addEventListener('pointerenter', showHoverPreview);
        wrapper.addEventListener('pointerleave', hideHoverPreview);

        trigger.addEventListener('mouseenter', showHoverPreview);

        trigger.addEventListener('focus', () => {
            positionInlineInputHelp(wrapper);
        });

        trigger.addEventListener('click', event => {
            event.preventDefault();
            event.stopPropagation();
            toggleInlineInputHelp(wrapper);
        });

        trigger.addEventListener('keydown', event => {
            if (event.key === 'Escape') {
                event.preventDefault();
                closeInlineInputHelp();
                trigger.blur();
            }
        });

        wrapper.appendChild(trigger);
        wrapper.appendChild(tooltip);
        label.classList.add('with-help');
        label.appendChild(wrapper);
        label.dataset.helpEnhanced = 'true';
    });

    document.addEventListener('click', event => {
        if (!event.target.closest('.input-help')) {
            closeInlineInputHelp();
        }
    });

    document.addEventListener('keydown', event => {
        if (event.key === 'Escape') {
            closeInlineInputHelp();
        }
    });

    window.addEventListener('resize', () => {
        document.querySelectorAll('.input-help.is-open').forEach(positionInlineInputHelp);
    });
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

function getNumericValue(id) {
    const value = document.getElementById(id).value;
    if (value === '') {
        return null;
    }

    const parsed = parseFloat(value);
    return Number.isFinite(parsed) ? parsed : null;
}

function addNumericField(data, key, id) {
    const value = getNumericValue(id);
    if (value !== null) {
        data[key] = value;
    }
}

function toggleSizingFields() {
    const componentType = document.getElementById('sizing-component-type').value;
    const trafficCommonFields = document.getElementById('sizing-traffic-common-fields');

    document.querySelectorAll('.sizing-fields').forEach(section => {
        section.classList.add('hidden');
    });

    if (componentType === 'llm_vram') {
        trafficCommonFields.classList.add('hidden');
    } else {
        trafficCommonFields.classList.remove('hidden');
    }

    const activeSection = document.getElementById(`sizing-fields-${componentType}`);
    if (activeSection) {
        activeSection.classList.remove('hidden');
    }
}

function toggleAttentionModeFields() {
    const mode = document.getElementById('vram-attention-mode').value;
    const kvGroup = document.getElementById('vram-kv-heads-group');
    if (kvGroup) {
        // Show KV heads field only for GQA or auto/legacy mode
        if (mode === 'MHA' || mode === 'MQA') {
            kvGroup.style.display = 'none';
        } else {
            kvGroup.style.display = '';
        }
    }
}

function toggleTcoFields() {
    const includeTco = document.getElementById('sizing-include-tco').checked;
    const tcoSection = document.getElementById('sizing-tco-fields');

    if (includeTco) {
        tcoSection.classList.remove('hidden');
    } else {
        tcoSection.classList.add('hidden');
    }
}

function renderSizingResult(data) {
    const componentLabels = {
        stateless: 'Stateless service',
        sql: 'SQL database',
        nosql: 'NoSQL database',
        vector_db: 'Vector DB',
        cache: 'Cache',
        logging: 'Logging / Monitoring',
        llm_vram: 'VRAM needs'
    };

    const metricLine = (label, value) => `
        <div class="result-line">
            <strong>${label}:</strong>
            <span class="result-value">${value}</span>
        </div>
    `;

    const summaryLines = [
        metricLine('Component Type', componentLabels[data.componentType] || data.componentType)
    ];

    const detailLines = [];

    if (data.componentType !== 'llm_vram') {
        detailLines.push(metricLine('Concurrency', formatNumber(data.concurrency)));
        detailLines.push(metricLine('Network Throughput (MB/s)', formatNumber(data.networkMBps)));
        detailLines.push(metricLine('Network Throughput (Gbps)', formatNumber(data.networkGbps)));
    }

    if (data.componentType === 'stateless') {
        summaryLines.push(metricLine('Required vCPU', formatNumber(data.requiredVcpu)));
        summaryLines.push(metricLine('Required RAM (GB)', formatNumber(data.requiredRamGB)));
        summaryLines.push(metricLine('Required Nodes', formatInteger(data.requiredNodes)));
    } else if (data.componentType === 'sql') {
        summaryLines.push(metricLine('Required vCPU', formatNumber(data.sqlVcpu)));
        summaryLines.push(metricLine('Required RAM (GB)', formatNumber(data.sqlRamGB)));
        summaryLines.push(metricLine('Required Disk (GB)', formatNumber(data.sqlDiskGB)));
    } else if (data.componentType === 'nosql') {
        summaryLines.push(metricLine('Shards', formatInteger(data.shards)));
        summaryLines.push(metricLine('Required Nodes', formatInteger(data.nosqlNodes)));
        summaryLines.push(metricLine('Required Disk (GB)', formatNumber(data.nosqlDiskGB)));
        detailLines.push(metricLine('Required vCPU', formatNumber(data.nosqlVcpu)));
        detailLines.push(metricLine('Required RAM (GB)', formatNumber(data.nosqlRamGB)));
    } else if (data.componentType === 'vector_db') {
        summaryLines.push(metricLine('Required vCPU', formatNumber(data.vectorVcpu)));
        summaryLines.push(metricLine('Required RAM (GB)', formatNumber(data.vectorRamGB)));
        summaryLines.push(metricLine('Required Disk (GB)', formatNumber(data.vectorDiskGB)));
        detailLines.push(metricLine('Raw Vector Data (GB)', formatNumber(data.rawVectorGB)));
        detailLines.push(metricLine('Metadata (GB)', formatNumber(data.metadataGB)));
        detailLines.push(metricLine('Index Size (GB)', formatNumber(data.indexGB)));
    } else if (data.componentType === 'cache') {
        summaryLines.push(metricLine('Required vCPU', formatNumber(data.cacheVcpu)));
        summaryLines.push(metricLine('Required RAM (GB)', formatNumber(data.cacheRamGB)));
    } else if (data.componentType === 'logging') {
        summaryLines.push(metricLine('Daily Log Volume (GB/day)', formatNumber(data.dailyLogGB)));
        summaryLines.push(metricLine('Required Disk (GB)', formatNumber(data.loggingDiskGB)));
    } else if (data.componentType === 'llm_vram') {
        summaryLines.push(metricLine('Weight Memory (GB)', formatNumber(data.weightMemoryGB)));
        summaryLines.push(metricLine('Activation / Workspace (GB)', formatNumber(data.activationGB)));
        summaryLines.push(metricLine('Total VRAM (GB)', formatNumber(data.totalVramGB)));
        detailLines.push(metricLine('KV Cache (GB)', formatNumber(data.kvCacheGB)));
        detailLines.push(metricLine('Architecture Type', data.architectureType));
        if (data.attentionMode) {
            detailLines.push(metricLine('Attention Mode', data.attentionMode));
        }
        detailLines.push(metricLine('Layers', formatInteger(data.numLayers)));

        if (data.usedFallbackHeuristic) {
            detailLines.push(metricLine('Activation Estimate Mode', 'Fallback heuristic'));
            detailLines.push(metricLine('Fallback Multiplier', formatNumber(data.fallbackActivationMultiplier)));
        } else {
            detailLines.push(metricLine('Attention Heads Used', formatInteger(data.numAttentionHeadsUsed)));
            detailLines.push(metricLine('KV Heads Used', formatInteger(data.numKeyValueHeadsUsed)));
            detailLines.push(metricLine('GQA Ratio', formatNumber(data.gqaRatio)));
            detailLines.push(metricLine('Prefill Working Memory (GB)', formatNumber(data.prefillActivationGB)));
            detailLines.push(metricLine('Decode Workspace (GB)', formatNumber(data.decodeWorkspaceGB)));
        }
    }

    if (data.componentType === 'sql') {
        detailLines.push(metricLine('Required vCPU', formatNumber(data.sqlVcpu)));
        detailLines.push(metricLine('Required RAM (GB)', formatNumber(data.sqlRamGB)));
        detailLines.push(metricLine('Required Disk (GB)', formatNumber(data.sqlDiskGB)));
    }

    if (data.componentType === 'stateless') {
        detailLines.push(metricLine('Required vCPU', formatNumber(data.requiredVcpu)));
        detailLines.push(metricLine('Required RAM (GB)', formatNumber(data.requiredRamGB)));
        detailLines.push(metricLine('Required Nodes', formatInteger(data.requiredNodes)));
    }

    if (data.componentType === 'nosql') {
        detailLines.push(metricLine('Shards', formatInteger(data.shards)));
        detailLines.push(metricLine('Required Nodes', formatInteger(data.nosqlNodes)));
        detailLines.push(metricLine('Required Disk (GB)', formatNumber(data.nosqlDiskGB)));
    }

    if (data.componentType === 'cache') {
        detailLines.push(metricLine('Required vCPU', formatNumber(data.cacheVcpu)));
        detailLines.push(metricLine('Required RAM (GB)', formatNumber(data.cacheRamGB)));
    }

    if (data.componentType === 'logging') {
        detailLines.push(metricLine('Daily Log Volume (GB/day)', formatNumber(data.dailyLogGB)));
        detailLines.push(metricLine('Required Disk (GB)', formatNumber(data.loggingDiskGB)));
    }

    const tcoLines = [];
    if (typeof data.tcoOnPrem === 'number') {
        tcoLines.push(metricLine('On-Prem TCO', formatMoney(data.tcoOnPrem)));
    }
    if (typeof data.tcoCloud === 'number') {
        tcoLines.push(metricLine('Cloud TCO', formatMoney(data.tcoCloud)));
    }
    if (typeof data.breakEvenMonths === 'number') {
        tcoLines.push(metricLine('Break-even (months)', formatNumber(data.breakEvenMonths)));
    }

    let html = `
        <div class="result-section-title">Summary</div>
        ${summaryLines.join('')}
        <div class="result-section-title">Detailed Metrics</div>
        ${detailLines.join('')}
    `;

    if (tcoLines.length > 0) {
        html += `
            <div class="result-section-title">TCO Comparison</div>
            ${tcoLines.join('')}
        `;
    }

    showResult('sizing-result', html);
}

async function calculateResourceSizing() {
    const componentType = document.getElementById('sizing-component-type').value;
    const includeTco = document.getElementById('sizing-include-tco').checked;

    const commonRequiredIds = componentType === 'llm_vram'
        ? ['sizing-safety-factor']
        : [
            'sizing-peak-rps',
            'sizing-latency',
            'sizing-req-kb',
            'sizing-resp-kb',
            'sizing-replicas',
            'sizing-safety-factor',
            'sizing-peak-factor'
        ];

    const componentRequiredIds = {
        stateless: [
            'stateless-cpu-seconds',
            'stateless-base-ram',
            'stateless-mem-request',
            'stateless-vcpu-node',
            'stateless-ram-node'
        ],
        sql: [
            'sql-data-gb',
            'sql-hot-data-gb',
            'sql-read-rps',
            'sql-write-rps',
            'sql-cpu-read',
            'sql-cpu-write',
            'sql-index-ratio',
            'sql-wal-gb',
            'sql-temp-gb',
            'sql-query-workspace-gb',
            'sql-os-reserve-gb',
            'sql-cache-coverage'
        ],
        nosql: [
            'nosql-data-gb',
            'nosql-hot-data-gb',
            'nosql-read-rps',
            'nosql-write-rps',
            'nosql-rps-shard',
            'nosql-data-shard',
            'nosql-cpu-read',
            'nosql-cpu-write',
            'nosql-index-ratio',
            'nosql-compaction-ratio',
            'nosql-cache-ratio',
            'nosql-memtable-gb',
            'nosql-os-reserve-gb'
        ],
        vector_db: [
            'vector-num-vectors',
            'vector-dimension',
            'vector-metadata-bytes',
            'vector-index-ratio',
            'vector-resident-fraction',
            'vector-index-resident-fraction',
            'vector-peak-qps',
            'vector-candidates-scanned',
            'vector-ops-core-sec',
            'vector-query-buffer-gb',
            'vector-wal-gb',
            'vector-snapshot-gb',
            'vector-os-reserve-gb'
        ],
        cache: [
            'cache-hot-data-gb',
            'cache-serialization-overhead',
            'cache-eviction-headroom',
            'cache-rps-core'
        ],
        logging: [
            'logging-events-sec',
            'logging-avg-kb',
            'logging-retention-days',
            'logging-replication-factor',
            'logging-overhead-ratio'
        ],
        llm_vram: [
            'vram-model-parameters',
            'vram-batch-size',
            'vram-context-length',
            'vram-hidden-size',
            'vram-num-layers'
        ]
    };

    const tcoRequiredIds = [
        'tco-hardware-capex',
        'tco-storage-capex',
        'tco-network-capex',
        'tco-annual-power',
        'tco-annual-cooling',
        'tco-annual-support',
        'tco-annual-licensing',
        'tco-annual-staff',
        'tco-annual-downtime',
        'tco-years',
        'tco-monthly-compute',
        'tco-monthly-storage',
        'tco-monthly-backup',
        'tco-monthly-egress',
        'tco-monthly-managed',
        'tco-monthly-support',
        'tco-monthly-staff',
        'tco-monthly-downtime',
        'tco-months-sizing'
    ];

    const requiredIds = [
        ...commonRequiredIds,
        ...(componentRequiredIds[componentType] || []),
        ...(includeTco ? tcoRequiredIds : [])
    ];

    const hasMissingFields = requiredIds.some(id => getNumericValue(id) === null);
    if (hasMissingFields) {
        showResult('sizing-result', 'Please fill in all required fields.', true);
        return;
    }

    const data = {
        componentType,
        includeTco
    };

    addNumericField(data, 'safetyFactor', 'sizing-safety-factor');

    if (componentType !== 'llm_vram') {
        addNumericField(data, 'peakRps', 'sizing-peak-rps');
        addNumericField(data, 'avgLatencyMs', 'sizing-latency');
        addNumericField(data, 'avgRequestSizeKB', 'sizing-req-kb');
        addNumericField(data, 'avgResponseSizeKB', 'sizing-resp-kb');
        addNumericField(data, 'replicas', 'sizing-replicas');
        addNumericField(data, 'peakFactor', 'sizing-peak-factor');
    }

    if (componentType === 'stateless') {
        addNumericField(data, 'cpuSecondsPerRequest', 'stateless-cpu-seconds');
        addNumericField(data, 'baseRamGB', 'stateless-base-ram');
        addNumericField(data, 'memPerRequestMB', 'stateless-mem-request');
        addNumericField(data, 'vcpuPerNode', 'stateless-vcpu-node');
        addNumericField(data, 'ramPerNodeGB', 'stateless-ram-node');
    } else if (componentType === 'sql') {
        addNumericField(data, 'dataGB', 'sql-data-gb');
        addNumericField(data, 'hotDataGB', 'sql-hot-data-gb');
        addNumericField(data, 'readRps', 'sql-read-rps');
        addNumericField(data, 'writeRps', 'sql-write-rps');
        addNumericField(data, 'cpuReadSec', 'sql-cpu-read');
        addNumericField(data, 'cpuWriteSec', 'sql-cpu-write');
        addNumericField(data, 'indexRatio', 'sql-index-ratio');
        addNumericField(data, 'walGB', 'sql-wal-gb');
        addNumericField(data, 'tempGB', 'sql-temp-gb');
        addNumericField(data, 'queryWorkspaceGB', 'sql-query-workspace-gb');
        addNumericField(data, 'osReserveGB', 'sql-os-reserve-gb');
        addNumericField(data, 'cacheCoverageRatio', 'sql-cache-coverage');
    } else if (componentType === 'nosql') {
        addNumericField(data, 'dataGB', 'nosql-data-gb');
        addNumericField(data, 'hotDataGB', 'nosql-hot-data-gb');
        addNumericField(data, 'readRps', 'nosql-read-rps');
        addNumericField(data, 'writeRps', 'nosql-write-rps');
        addNumericField(data, 'rpsPerShard', 'nosql-rps-shard');
        addNumericField(data, 'dataGBPerShard', 'nosql-data-shard');
        addNumericField(data, 'cpuReadSec', 'nosql-cpu-read');
        addNumericField(data, 'cpuWriteSec', 'nosql-cpu-write');
        addNumericField(data, 'indexRatio', 'nosql-index-ratio');
        addNumericField(data, 'compactionRatio', 'nosql-compaction-ratio');
        addNumericField(data, 'cacheRatio', 'nosql-cache-ratio');
        addNumericField(data, 'memtableGB', 'nosql-memtable-gb');
        addNumericField(data, 'osReserveGB', 'nosql-os-reserve-gb');
    } else if (componentType === 'vector_db') {
        addNumericField(data, 'numVectors', 'vector-num-vectors');
        addNumericField(data, 'dimension', 'vector-dimension');
        data.precision = document.getElementById('vector-precision').value;
        addNumericField(data, 'metadataBytesPerVector', 'vector-metadata-bytes');
        addNumericField(data, 'indexRatio', 'vector-index-ratio');
        addNumericField(data, 'residentFraction', 'vector-resident-fraction');
        addNumericField(data, 'indexResidentFraction', 'vector-index-resident-fraction');
        addNumericField(data, 'peakQueryQps', 'vector-peak-qps');
        addNumericField(data, 'candidatesScanned', 'vector-candidates-scanned');
        addNumericField(data, 'vectorOpsPerCorePerSecond', 'vector-ops-core-sec');
        addNumericField(data, 'queryBufferGB', 'vector-query-buffer-gb');
        addNumericField(data, 'walGB', 'vector-wal-gb');
        addNumericField(data, 'snapshotGB', 'vector-snapshot-gb');
        addNumericField(data, 'osReserveGB', 'vector-os-reserve-gb');
    } else if (componentType === 'cache') {
        addNumericField(data, 'hotCacheDataGB', 'cache-hot-data-gb');
        addNumericField(data, 'serializationOverhead', 'cache-serialization-overhead');
        addNumericField(data, 'evictionHeadroom', 'cache-eviction-headroom');
        addNumericField(data, 'rpsPerCore', 'cache-rps-core');
    } else if (componentType === 'logging') {
        addNumericField(data, 'eventsPerSec', 'logging-events-sec');
        addNumericField(data, 'avgLogKB', 'logging-avg-kb');
        addNumericField(data, 'retentionDays', 'logging-retention-days');
        addNumericField(data, 'replicationFactor', 'logging-replication-factor');
        addNumericField(data, 'overheadRatio', 'logging-overhead-ratio');
    } else if (componentType === 'llm_vram') {
        addNumericField(data, 'modelParameters', 'vram-model-parameters');
        data.precisionType = document.getElementById('vram-precision-type').value;
        addNumericField(data, 'batchSize', 'vram-batch-size');
        addNumericField(data, 'contextLength', 'vram-context-length');
        addNumericField(data, 'hiddenSize', 'vram-hidden-size');
        addNumericField(data, 'numLayers', 'vram-num-layers');
        data.architectureType = document.getElementById('vram-architecture-type').value;
        const attentionMode = document.getElementById('vram-attention-mode').value;
        if (attentionMode) {
            data.attentionMode = attentionMode;
        }
        addNumericField(data, 'numAttentionHeads', 'vram-num-attention-heads');
        addNumericField(data, 'numKeyValueHeads', 'vram-num-kv-heads');
    }

    if (includeTco) {
        addNumericField(data, 'hardwareCapex', 'tco-hardware-capex');
        addNumericField(data, 'storageCapex', 'tco-storage-capex');
        addNumericField(data, 'networkCapex', 'tco-network-capex');
        addNumericField(data, 'annualPowerCost', 'tco-annual-power');
        addNumericField(data, 'annualCoolingCost', 'tco-annual-cooling');
        addNumericField(data, 'annualSupportCost', 'tco-annual-support');
        addNumericField(data, 'annualLicensingCost', 'tco-annual-licensing');
        addNumericField(data, 'annualStaffCost', 'tco-annual-staff');
        addNumericField(data, 'annualDowntimeRiskCost', 'tco-annual-downtime');
        addNumericField(data, 'years', 'tco-years');
        addNumericField(data, 'monthlyComputeCost', 'tco-monthly-compute');
        addNumericField(data, 'monthlyStorageCost', 'tco-monthly-storage');
        addNumericField(data, 'monthlyBackupCost', 'tco-monthly-backup');
        addNumericField(data, 'monthlyEgressCost', 'tco-monthly-egress');
        addNumericField(data, 'monthlyManagedServicesCost', 'tco-monthly-managed');
        addNumericField(data, 'monthlySupportPlanCost', 'tco-monthly-support');
        addNumericField(data, 'monthlyStaffCost', 'tco-monthly-staff');
        addNumericField(data, 'monthlyDowntimeRiskCost', 'tco-monthly-downtime');
        addNumericField(data, 'months', 'tco-months-sizing');
    }

    const result = await apiCall('resource-sizing', data);

    if (result.error) {
        showResult('sizing-result', `Error: ${result.error}`, true);
    } else {
        renderSizingResult(result);
    }
}

function renderLoadAnalysisResult(data) {
    const metricLine = (label, value, status) => `
        <div class="result-line status-line">
            <strong>${label}:</strong>
            <span class="result-value">${value}</span>
            <span class="status-badge status-${getStatusClass(status)}">${status}</span>
        </div>
    `;

    const recommendationsHtml = (data.recommendations || []).map(item => `
        <div class="recommendation-item priority-${getPriorityClass(item.priority)}">
            <strong>${item.priority} - ${item.area}:</strong>
            <span>${item.message}</span>
        </div>
    `).join('');

    const html = `
        <div class="result-section-title">Summary</div>
        <div class="result-line">
            <strong>Service Type:</strong>
            <span class="result-value">${data.serviceType}</span>
        </div>
        <div class="result-line status-line">
            <strong>Overall Status:</strong>
            <span class="result-value">${data.overallStatus}</span>
            <span class="status-badge status-${getStatusClass(data.overallStatus)}">${data.overallStatus}</span>
        </div>

        <div class="result-section-title">Pressure Metrics</div>
        ${metricLine('Traffic Pressure', formatNumber(data.trafficPressure), data.trafficStatus)}
        ${metricLine('CPU Pressure', formatNumber(data.cpuPressure), data.cpuStatus)}
        ${metricLine('RAM Pressure', formatNumber(data.ramPressure), data.ramStatus)}
        ${metricLine('Disk Pressure', formatNumber(data.diskPressure), data.diskStatus)}
        ${metricLine('Latency Pressure', formatNumber(data.latencyPressure), data.latencyStatus)}
        ${metricLine('Error Pressure', formatNumber(data.errorPressure), data.errorStatus)}

        <div class="result-section-title">Recommendations</div>
        <div class="recommendations-list">${recommendationsHtml}</div>
    `;

    showResult('load-analysis-result', html);
}

async function calculateLoadAnalysis() {
    const requiredIds = [
        'load-expected-rps',
        'load-actual-rps',
        'load-cpu-usage',
        'load-cpu-threshold',
        'load-ram-usage',
        'load-ram-threshold',
        'load-disk-iops',
        'load-disk-threshold',
        'load-latency-ms',
        'load-latency-sla-ms',
        'load-error-rate',
        'load-error-budget'
    ];

    const hasMissingFields = requiredIds.some(id => getNumericValue(id) === null);
    if (hasMissingFields) {
        showResult('load-analysis-result', 'Please fill in all required fields.', true);
        return;
    }

    const data = {
        serviceType: document.getElementById('load-service-type').value
    };

    addNumericField(data, 'expectedRps', 'load-expected-rps');
    addNumericField(data, 'actualRps', 'load-actual-rps');
    addNumericField(data, 'cpuUsagePercent', 'load-cpu-usage');
    addNumericField(data, 'cpuThresholdPercent', 'load-cpu-threshold');
    addNumericField(data, 'ramUsageGB', 'load-ram-usage');
    addNumericField(data, 'ramThresholdGB', 'load-ram-threshold');
    addNumericField(data, 'diskIops', 'load-disk-iops');
    addNumericField(data, 'diskIopsThreshold', 'load-disk-threshold');
    addNumericField(data, 'latencyMs', 'load-latency-ms');
    addNumericField(data, 'latencySlaMs', 'load-latency-sla-ms');
    addNumericField(data, 'errorRatePercent', 'load-error-rate');
    addNumericField(data, 'errorBudgetPercent', 'load-error-budget');

    const result = await apiCall('load-analysis', data);

    if (result.error) {
        showResult('load-analysis-result', `Error: ${result.error}`, true);
    } else {
        renderLoadAnalysisResult(result);
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

    // Resource Sizing
    [
        'sizing-peak-rps',
        'sizing-latency',
        'sizing-req-kb',
        'sizing-resp-kb',
        'sizing-replicas',
        'sizing-safety-factor',
        'sizing-peak-factor',
        'stateless-cpu-seconds',
        'stateless-base-ram',
        'stateless-mem-request',
        'stateless-vcpu-node',
        'stateless-ram-node',
        'sql-data-gb',
        'sql-hot-data-gb',
        'sql-read-rps',
        'sql-write-rps',
        'sql-cpu-read',
        'sql-cpu-write',
        'sql-index-ratio',
        'sql-wal-gb',
        'sql-temp-gb',
        'sql-query-workspace-gb',
        'sql-os-reserve-gb',
        'sql-cache-coverage',
        'nosql-data-gb',
        'nosql-hot-data-gb',
        'nosql-read-rps',
        'nosql-write-rps',
        'nosql-rps-shard',
        'nosql-data-shard',
        'nosql-cpu-read',
        'nosql-cpu-write',
        'nosql-index-ratio',
        'nosql-compaction-ratio',
        'nosql-cache-ratio',
        'nosql-memtable-gb',
        'nosql-os-reserve-gb',
        'vector-num-vectors',
        'vector-dimension',
        'vector-metadata-bytes',
        'vector-index-ratio',
        'vector-resident-fraction',
        'vector-index-resident-fraction',
        'vector-peak-qps',
        'vector-candidates-scanned',
        'vector-ops-core-sec',
        'vector-query-buffer-gb',
        'vector-wal-gb',
        'vector-snapshot-gb',
        'vector-os-reserve-gb',
        'cache-hot-data-gb',
        'cache-serialization-overhead',
        'cache-eviction-headroom',
        'cache-rps-core',
        'logging-events-sec',
        'logging-avg-kb',
        'logging-retention-days',
        'logging-replication-factor',
        'logging-overhead-ratio',
        'vram-model-parameters',
        'vram-batch-size',
        'vram-context-length',
        'vram-hidden-size',
        'vram-num-layers',
        'vram-num-attention-heads',
        'vram-num-kv-heads',
        'tco-hardware-capex',
        'tco-storage-capex',
        'tco-network-capex',
        'tco-annual-power',
        'tco-annual-cooling',
        'tco-annual-support',
        'tco-annual-licensing',
        'tco-annual-staff',
        'tco-annual-downtime',
        'tco-years',
        'tco-monthly-compute',
        'tco-monthly-storage',
        'tco-monthly-backup',
        'tco-monthly-egress',
        'tco-monthly-managed',
        'tco-monthly-support',
        'tco-monthly-staff',
        'tco-monthly-downtime',
        'tco-months-sizing'
    ].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateResourceSizing();
        });
    });

    // Load Analysis
    [
        'load-expected-rps',
        'load-actual-rps',
        'load-cpu-usage',
        'load-cpu-threshold',
        'load-ram-usage',
        'load-ram-threshold',
        'load-disk-iops',
        'load-disk-threshold',
        'load-latency-ms',
        'load-latency-sla-ms',
        'load-error-rate',
        'load-error-budget'
    ].forEach(id => {
        document.getElementById(id).addEventListener('keypress', function(e) {
            if (e.key === 'Enter') calculateLoadAnalysis();
        });
    });

    attachInlineInputHelp(inlineInputHelpText);
    toggleSizingFields();
    toggleTcoFields();
});
