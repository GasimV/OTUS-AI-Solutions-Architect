# Federated Learning and Privacy-Preserving Architecture

<a id="contents"></a>

## Contents

- [1. Privacy-Preserving AI Ecosystem](#privacy-preserving-ai-ecosystem)
  - [1.1 PPA Ecosystem Overview](#ppa-ecosystem-overview)
  - [1.2 Statistical Methods](#statistical-methods)
  - [1.3 Algorithmic Approaches](#algorithmic-approaches)
  - [1.4 Cryptographic Methods](#cryptographic-methods)
  - [1.5 Hybridization Principle](#hybridization-principle)
  - [1.6 Practical Architecture Guidelines](#practical-architecture-guidelines)
- [2. Differential Privacy Architecture and Threat Model](#differential-privacy-architecture-and-threat-model)
  - [2.1 Essence and Mathematical Basis](#dp-essence-and-mathematical-basis)
  - [2.2 DP Math Visual: Neighboring Datasets](#dp-math-visual-neighboring-datasets)
  - [2.3 Local vs Global Differential Privacy](#local-vs-global-differential-privacy)
  - [2.4 Differential Privacy Frameworks and Libraries](#differential-privacy-frameworks-and-libraries)
  - [2.5 White-Box Threats: Model Inversion and Data Extraction](#white-box-threats-model-inversion-and-data-extraction)
  - [2.6 Architect Guidance](#dp-architect-guidance)
- [3. Federated Learning Architecture Patterns](#federated-learning-architecture-patterns)
  - [3.1 Cross-Silo Federated Learning](#cross-silo-federated-learning)
  - [3.2 Cross-Device Federated Learning](#cross-device-federated-learning)
  - [3.3 Centralized (Hub-and-Spoke) FL](#centralized-fl)
  - [3.4 Decentralized (Peer-to-Peer / Gossip) FL](#decentralized-fl)
  - [3.5 Split Learning](#split-learning)
  - [3.6 FL Frameworks Comparison](#fl-frameworks-comparison)
  - [3.7 Federated Learning Decision Matrix](#federated-learning-decision-matrix)
  - [3.8 Federated Learning Practical Design Checklist](#federated-learning-practical-design-checklist)
- [4. Advanced PPA Stack for AI Architects](#advanced-ppa-stack-for-ai-architects)
  - [4.1 Gradient Compression and Vector Quantization](#gradient-compression-and-vector-quantization)
  - [4.2 Fully Homomorphic Encryption](#fully-homomorphic-encryption)
  - [4.3 FHE Tooling Landscape](#fhe-tooling-landscape)
  - [4.4 Secure Aggregation with FHE](#secure-aggregation-with-fhe)
  - [4.5 Secure Multi-Party Computation](#secure-multi-party-computation)
  - [4.6 SMPC Frameworks](#smpc-frameworks)
  - [4.7 Design Trade-off Summary](#design-trade-off-summary)
  - [4.8 Architect Playbook](#architect-playbook)
- [5. AI Architect Design and Operations](#ai-architect-design-and-operations)
  - [5.1 Architect Decision Matrix](#architect-decision-matrix)
  - [5.2 Reference Privacy-Preserving AI Stack](#reference-privacy-preserving-ai-stack)
  - [5.3 Common Threats and Mitigations](#common-threats-and-mitigations)
  - [5.4 Active Recall Prompts](#active-recall-prompts)
  - [5.5 Final Mental Model](#final-mental-model)

---

<a id="privacy-preserving-ai-ecosystem"></a>

## 1. Privacy-Preserving AI Ecosystem

This section covers the core privacy-preserving AI method families: statistical methods, algorithmic approaches, cryptographic methods, and hybrid combinations.

[Back to Contents](#contents)

---

<a id="ppa-ecosystem-overview"></a>

### 1.1 PPA Ecosystem Overview

**PPA (Privacy-Preserving AI)** is the architecture discipline of building AI systems that can learn from, infer on, or analyze sensitive data while reducing exposure of raw data, individual records, model updates, and confidential computation.

The main families are:

| Family | Core idea | Main protection target |
| --- | --- | --- |
| Statistical methods | Add mathematically calibrated privacy protection | Individual records and re-identification risk |
| Algorithmic approaches | Keep data local and distribute training | Raw data centralization risk |
| Cryptographic methods | Compute on protected data or inside protected environments | Inputs, intermediate values, and computation boundaries |

For an AI Architect, the important point is that these methods solve different parts of the privacy problem. They are often combined rather than used alone.

[Back to Contents](#contents)

---

<a id="statistical-methods"></a>

### 1.2 Statistical Methods

**Core concept:** Differential Privacy (DP)

**Definition**

Differential Privacy is a mathematical framework that limits how much the presence or absence of a single record can influence the output of a query, statistic, or model.

Simple meaning:

> The system should not reveal whether one specific person was included in the dataset.

**Key Function**

- Provides formal privacy guarantees through calibrated noise injection.
- Reduces re-identification risk.
- Makes aggregate outputs less sensitive to any single data subject.

**Important Concepts**

| Concept | Meaning |
| --- | --- |
| Privacy budget | Controls how much privacy is spent across queries or training steps |
| Epsilon | Smaller epsilon usually means stronger privacy but lower utility |
| Sensitivity | How much one record can change the output |
| Noise calibration | Amount of noise needed to protect the result |
| Utility | Accuracy or usefulness of the output after privacy protection |

**Design Considerations**

- The privacy budget is a trade-off between privacy and accuracy.
- Noise calibration depends on query sensitivity.
- Strong DP can reduce model quality or slow convergence.
- DP must be tracked across repeated queries or training rounds.
- DP protects against record-level inference, but it does not automatically secure infrastructure, code, keys, or access control.

**Typical Use Cases**

- Analytics dashboards over sensitive data.
- Model training with regulated or personal datasets.
- Data sharing scenarios where aggregate results are allowed but raw data is not.
- Public statistics where individual membership must remain hidden.

**Architectural Use**

| Placement | Example |
| --- | --- |
| Data/query layer | Add noise to aggregate analytics |
| Training layer | Apply DP-SGD during model training |
| Client layer | Apply local DP before sending updates |
| Reporting layer | Release only DP-protected metrics |

[Back to Contents](#contents)

---

<a id="algorithmic-approaches"></a>

### 1.3 Algorithmic Approaches

**Core concepts:** Federated Learning (FL) and Split Learning

**Definition**

Federated Learning is a distributed learning paradigm where data remains local and clients share model updates, gradients, or weights instead of raw data.

Simple meaning:

> Move the model to the data, not the data to the model.

**Key Function**

- Enables training without centralizing raw data.
- Preserves data locality and ownership boundaries.
- Supports collaboration across devices, hospitals, banks, regions, or organizations.

**Variants**

| Approach | How it works | Best fit |
| --- | --- | --- |
| Federated Learning | Clients train locally and send model updates to an aggregator | Multi-client training where data cannot be centralized |
| Split Learning | Model is split between client and server; client sends activations, not raw data | Data-sensitive settings where client compute is limited or model parts must be separated |
| Cross-device FL | Many unreliable edge/mobile clients participate intermittently | Mobile keyboards, edge AI, personalization |
| Cross-silo FL | A smaller number of stable organizations collaborate | Hospitals, banks, enterprises, research networks |

**Design Considerations**

- Communication overhead can be high because updates are frequent and large.
- Clients may have different data distributions, also called non-IID data.
- Clients may have different compute, bandwidth, uptime, and hardware.
- Gradient leakage or reconstruction attacks can expose sensitive patterns.
- Aggregation logic becomes a critical trust and security boundary.
- Model evaluation is harder because data is distributed.

**Typical Use Cases**

- Healthcare training across multiple hospitals.
- Mobile or edge ML where raw data stays on device.
- Cross-organization collaboration without raw data pooling.
- Financial fraud detection across institutions.
- Industrial AI where plant or factory data cannot leave local sites.

**Common FL Architecture**

```text
Client A data -> local training -> model update \
Client B data -> local training -> model update  -> secure aggregation -> global model
Client C data -> local training -> model update /
```

Important architecture components:

- Client selection.
- Local training runtime.
- Secure update transport.
- Aggregation server.
- Model registry.
- Privacy accounting.
- Monitoring and anomaly detection.
- Rollback and versioning.

[Back to Contents](#contents)

---

<a id="cryptographic-methods"></a>

### 1.4 Cryptographic Methods

**Core concepts:** SMPC, FHE, and TEE

**Definitions**

| Method | Full name | Simple meaning |
| --- | --- | --- |
| SMPC | Secure Multi-Party Computation | Multiple parties compute a result without revealing private inputs |
| FHE | Fully Homomorphic Encryption | Computation is performed directly on encrypted data |
| TEE | Trusted Execution Environment | Computation runs inside a protected hardware-backed enclave |

**Key Function**

- Enables computation over protected, encrypted, or isolated data.
- Provides stronger protection at computation boundaries.
- Helps reduce trust in the central processing environment.

**Design Considerations**

| Method | Strength | Main trade-off |
| --- | --- | --- |
| SMPC | Strong multi-party privacy for joint computation | Network and computation overhead |
| FHE | Data can remain encrypted during computation | Very high compute cost and limited practical workloads |
| TEE | Practical confidential computation with lower overhead than FHE | Hardware trust, side-channel risk, enclave limitations |

**Typical Use Cases**

- Secure model inference.
- Cross-party analytics.
- Confidential ML pipelines.
- Secure aggregation in federated learning.
- Protected feature computation.
- Sensitive scoring where model, input, or both must be protected.

**Architectural Use**

| Layer | Example |
| --- | --- |
| Input protection | Encrypt or secret-share sensitive inputs |
| Training protection | Secure aggregation with SMPC |
| Inference protection | Run inference inside a TEE or over encrypted inputs |
| Output protection | Combine cryptographic protection with access control and DP |

[Back to Contents](#contents)

---

<a id="hybridization-principle"></a>

### 1.5 Hybridization Principle

**Critical architect insight:** no single approach provides complete protection.

Each method has a different weakness:

| Method | Weakness |
| --- | --- |
| Federated Learning | Model updates can leak information through gradient reconstruction or membership inference |
| Differential Privacy | Stronger privacy can reduce model accuracy and utility |
| SMPC | Stronger protection but higher communication and computation cost |
| FHE | Very strong encrypted computation but often expensive and operationally complex |
| TEE | Practical but depends on hardware trust, patching, attestation, and side-channel risk management |

**Recommended Hybrid Patterns**

| Pattern | Benefit | Trade-off |
| --- | --- | --- |
| FL + Local DP | Protects client updates before sharing | Lower model quality if noise is too high |
| FL + SMPC | Aggregator cannot inspect individual client updates | More network and computation overhead |
| FL + TEE | Aggregation or training coordination runs in a protected enclave | Hardware dependency and enclave trust assumptions |
| DP + TEE | Protects both computation boundary and released outputs | Operational complexity |
| SMPC + DP | Protects inputs during computation and outputs after computation | Higher engineering and performance cost |

**Architect's Core Task**

Design a hybrid stack that balances:

- Privacy guarantees.
- Model performance.
- System scalability.
- Latency.
- Infrastructure and hardware budget.
- Regulatory requirements.
- Operational complexity.

The right architecture starts from the threat model, not from the technology name.

[Back to Contents](#contents)

---

<a id="practical-architecture-guidelines"></a>

### 1.6 Practical Architecture Guidelines

Combine privacy layers strategically:

| Architecture layer | Typical privacy technique |
| --- | --- |
| Data layer | Differential Privacy, minimization, masking, access control |
| Training layer | Federated Learning, secure aggregation, DP-SGD |
| Computation layer | SMPC, FHE, TEE |
| Transport layer | TLS, mTLS, signed updates |
| Governance layer | Consent, audit logs, retention policy, privacy budget tracking |
| Output layer | DP-protected metrics, redaction, policy checks |

Optimize for:

- Threat model: who are you defending against?
- Data sensitivity: personal, financial, health, biometric, enterprise-confidential.
- Latency constraints: real-time inference vs. batch analytics.
- Regulatory requirements: GDPR, HIPAA, financial regulations, internal policy.
- Deployment environment: cloud, edge, mobile, on-premises, multi-party.
- Model performance requirements.
- Cost and operational feasibility.

Always evaluate:

- Privacy vs. utility.
- Cost vs. security gain.
- Performance vs. cryptographic strength.
- Centralized simplicity vs. federated complexity.
- Compliance claims vs. actual technical guarantees.

[Back to Contents](#contents)

---

<a id="differential-privacy-architecture-and-threat-model"></a>

## 2. Differential Privacy Architecture and Threat Model

This section expands Differential Privacy (DP) from a short definition into an **AI Architect-level design tool**: the mathematical guarantee, Local vs. Global DP patterns, implementation frameworks, and the white-box attacks DP is meant to reduce.

> <mark>Architect focus</mark>: DP is a **statistical privacy guarantee**, not an encryption scheme. It limits what can be inferred from outputs, gradients, or aggregates by adding calibrated noise.

[Back to Contents](#contents)

---

<a id="dp-essence-and-mathematical-basis"></a>

### 2.1 Essence and Mathematical Basis

**Differential Privacy (DP)** provides a strict mathematical guarantee that the output of a data-processing algorithm should not reveal whether one specific record was present in the training sample.

**Simple illustrative example**

Suppose a hospital wants to publish the average age of patients in a sensitive cohort.

- Without DP, the exact average may shift slightly when one patient is included or removed.
- An attacker who knows enough background information might use that shift to infer whether a specific person was in the cohort.
- With DP, the system adds **calibrated noise** to the published average so the result looks almost the same whether that person was included or not.

> <mark>Intuition</mark>: DP makes an individual's presence statistically hard to detect from the released result.

**Core guarantee**

- The algorithm's output should be nearly indistinguishable whether the dataset contains an individual's record or not.
- This protects against **membership inference** and reduces the signal available for reconstruction-style attacks.
- The guarantee is expressed over *neighboring datasets*: two datasets that differ by one record.

**Mathematical form**

```text
Pr[M(D) in R] <= e^epsilon Pr[M(D') in R] + delta
```

Where:

| Symbol | Meaning | Architect interpretation |
| --- | --- | --- |
| **M** | Randomized mechanism or algorithm | Training, query, aggregation, or reporting process |
| **D** | Dataset with one individual's record | Original dataset |
| **D'** | Neighboring dataset without that record | Comparison dataset differing by one record |
| **R** | Possible output region | Model update, statistic, prediction range, or report output |
| **epsilon** | Privacy budget | Allowed information leakage |
| **delta** | Failure probability | Probability the privacy guarantee may not hold |

**Protection mechanism**

DP usually adds **calibrated statistical noise** during:

- Query answering.
- Aggregate analytics.
- Gradient computation.
- DP-SGD training.
- Final reporting or model release.

> <mark>Key point</mark>: noise must be calibrated to **sensitivity**, not added arbitrarily. Random noise without privacy accounting is not DP.

[Back to Contents](#contents)

---

<a id="dp-math-visual-neighboring-datasets"></a>

### 2.2 DP Math Visual: Neighboring Datasets

The DP mental model compares two output distributions:

- **D**: dataset **with** an individual's record.
- **D'**: dataset **without** that record.

If the output distributions are very close, an attacker cannot confidently infer whether the individual participated in the dataset.

**Privacy budget effect**

| Parameter | Effect |
| --- | --- |
| **Smaller epsilon** | Stronger privacy, more noise, lower utility |
| **Larger epsilon** | Weaker privacy, less noise, higher utility |
| **Smaller delta** | Lower probability of privacy failure |
| **Larger delta** | Higher probability that the guarantee can fail |

**Architect interpretation**

- **epsilon** controls the allowed distinguishability between outputs.
- **delta** captures a small probability of privacy failure.
- DP does not promise zero leakage; it bounds leakage mathematically.
- Privacy budget must be tracked across repeated queries, epochs, training rounds, and releases.

> <mark>Design warning</mark>: privacy budget is cumulative. Repeated access to DP-protected outputs can spend privacy even if each individual query looks safe.

[Back to Contents](#contents)

---

<a id="local-vs-global-differential-privacy"></a>

### 2.3 Local vs Global Differential Privacy

Differential Privacy can be applied before data leaves the client or centrally after collection/aggregation.

| Pattern | Trust model | Where noise is added | Accuracy impact | Best fit |
| --- | --- | --- | --- | --- |
| **Local DP (LDP)** | **Trust nobody** | Client side before sending data | High accuracy loss | Untrusted server, telemetry, client-side privacy |
| **Global DP / Central DP** | **Trusted aggregator** | Server side during aggregation or release | Lower accuracy loss | Trusted platform, data warehouse, ML training pipeline |

#### 2.3.1 Local Differential Privacy

**Local Differential Privacy (LDP)** adds noise on the client side before data is transmitted.

**Flow**

```text
Raw client data -> local noise -> noisy data/update -> server
```

**Properties**

- Server receives already-noisy data.
- Provides maximum protection against an untrusted server.
- Does not require trusting the aggregator with clean client values.

**Trade-off**

- <mark>Strong privacy, weaker utility</mark>.
- Model convergence can degrade sharply because every client update is noisy before aggregation.

**Architect fit**

- Mobile telemetry.
- Browser or device analytics.
- Cross-device FL where the server should not see clean client updates.
- Systems where client trust boundary is stronger than server trust.

#### 2.3.2 Global Differential Privacy

**Global DP**, also called **Central DP**, adds noise after the trusted aggregator receives clean data, gradients, or updates.

**Flow**

```text
Clean client data/updates -> trusted aggregator -> calibrated noise -> DP output/model
```

**Properties**

- Requires trust in the aggregator.
- Preserves higher model utility.
- Easier to tune for analytics and DP-SGD pipelines.

**Trade-off**

- <mark>Better utility, stronger trust assumption</mark>.
- The aggregator becomes a sensitive security boundary.

**Architect fit**

- Enterprise ML platforms.
- Internal analytics.
- Data warehouse reporting.
- Cross-silo FL with strong governance and a trusted coordinator.

[Back to Contents](#contents)

---

<a id="differential-privacy-frameworks-and-libraries"></a>

### 2.4 Differential Privacy Frameworks and Libraries

DP tooling depends on whether the workload is **deep learning**, **Google ecosystem ML**, or **analytics over warehouses**.

| Library | Primary use case | Strength | Architect caveat |
| --- | --- | --- | --- |
| **Opacus (PyTorch)** | Deep learning with **DP-SGD** | Efficient per-sample gradient computation through `einsum`; high performance in the PyTorch ecosystem | Best for PyTorch training, not general SQL analytics |
| **TensorFlow Privacy** | ML in the Google / TensorFlow ecosystem | XLA-driven JIT compilation and advanced vectorization | Best when the team is already standardized on TensorFlow |
| **Google DP Core (C++ / Java / Go)** | Data warehouse analytics | DP analytics for SQL-style queries and aggregations such as `COUNT` and `SUM` | Not oriented toward neural network gradient training |

#### 2.4.1 Opacus

**Best for:**

- PyTorch deep learning.
- DP-SGD training.
- Per-sample gradient clipping and noise injection.

**Why it matters**

- DP-SGD requires computing and clipping gradients per sample.
- Opacus is optimized for this workflow in PyTorch.

#### 2.4.2 TensorFlow Privacy

**Best for:**

- TensorFlow-based ML training.
- Google ecosystem teams.
- DP-SGD with TensorFlow-native execution.

**Why it matters**

- Can benefit from XLA JIT compilation and vectorization.
- Fits teams already invested in TensorFlow production pipelines.

#### 2.4.3 Google DP Core

**Best for:**

- Analytics pipelines.
- Data warehouses.
- SQL-style aggregate releases.

**Why it matters**

- Strong fit for DP-protected `COUNT`, `SUM`, and similar aggregate queries.
- Not the default choice for gradient-based neural network training.

> <mark>Selection rule</mark>: choose the DP library by workload first. **PyTorch training -> Opacus**, **TensorFlow training -> TensorFlow Privacy**, **analytics -> Google DP Core**.

[Back to Contents](#contents)

---

<a id="white-box-threats-model-inversion-and-data-extraction"></a>

### 2.5 White-Box Threats: Model Inversion and Data Extraction

White-box threats assume the attacker has access to sensitive internals such as gradients, model updates, or training signals.

> <mark>Threat model</mark>: access to raw gradients can be close to access to the original training data.

**Core vulnerability**

Gradients are derivatives of the loss function. They can preserve mathematical traces of the training samples that produced them.

In plain terms:

- Raw client gradients can contain sensitive properties of the local training batch.
- Plain federated learning does **not** automatically prevent gradient leakage.
- Attackers can reconstruct examples or infer membership from exposed updates.

**Model Inversion / DLG**

Deep Leakage from Gradients (DLG) is an iterative reconstruction attack.

Simplified objective:

```text
argmin_x || gradient_L(x) - gradient_L(x*) || + regularization
```

Meaning:

- The attacker guesses a candidate input.
- Computes gradients for the guess.
- Compares guessed gradients to observed real gradients.
- Iteratively improves the guess until reconstructed data matches the gradient signal.

**Simple illustrative example with math**

Assume a tiny one-parameter linear model:

$$
\hat{y} = w x
$$

$$
L = \frac{1}{2}(\hat{y} - y)^2
$$

The gradient with respect to the model weight is:

$$
\nabla_w L = (w x - y)x
$$

Suppose the attacker sees:

$$
w = 2,\quad y = 1,\quad \nabla_w L_{\text{observed}} = 3
$$

The attacker searches for an input `x` whose gradient matches `3`:

$$
(2x - 1)x = 3
$$

$$
2x^2 - x - 3 = 0
$$

Solving:

$$
x = 1.5 \quad \text{or} \quad x = -1
$$

If the valid data range is known to be:

$$
x \ge 0
$$

then the likely reconstructed input is:

$$
x = 1.5
$$

> <mark>Intuition</mark>: the gradient acts like an equation containing information about the original input. DLG generalizes this idea by optimizing candidate inputs until their gradients match the observed gradients.

**Attack vectors**

| Attack | What it tries to learn | Typical signal |
| --- | --- | --- |
| **Model inversion / DLG** | Reconstruct input samples or sensitive attributes | Raw gradients or model updates |
| **Data extraction** | Recover memorized training data | Model behavior, gradients, or outputs |
| **Membership inference** | Infer whether a record was in training data | Black-box outputs or white-box internals |

**Architectural mitigation**

DP-SGD reduces leakage by:

- **Clipping gradients** to bound sensitivity.
- Adding **calibrated noise** to gradients.
- Tracking the privacy budget over training.

Other controls should be layered with DP:

- Secure aggregation.
- Update access control.
- Gradient logging restrictions.
- Model output limiting.
- Robust aggregation and poisoning detection.

> <mark>Important distinction</mark>: DP reduces information leakage from gradients, but it does not replace secure aggregation, access control, or model integrity defenses.

[Back to Contents](#contents)

---

<a id="dp-architect-guidance"></a>

### 2.6 Architect Guidance

**When to use DP**

- Releasing aggregate statistics over sensitive data.
- Training models on personal, health, financial, or enterprise-confidential datasets.
- Reducing membership inference and reconstruction risk.
- Protecting model updates in FL with DP-SGD.
- Publishing reports or dashboards where repeated queries could leak individual records.

**Key design decisions**

| Decision | Architect question |
| --- | --- |
| **Local vs Global DP** | Do you trust the aggregator with clean data or updates? |
| **epsilon / delta** | What privacy-utility trade-off is acceptable to compliance and product owners? |
| **Noise location** | Should noise be added on client, server, gradient, aggregate, or output layer? |
| **Budget accounting** | How will budget be tracked across epochs, rounds, queries, and releases? |
| **Utility validation** | How will the team prove the model or analytics remain useful? |

**Common architecture patterns**

| Pattern | Privacy benefit | Trade-off |
| --- | --- | --- |
| **Local DP + FL** | Server never receives clean client updates | Lower convergence and model utility |
| **Central DP-SGD** | Stronger utility with formal training privacy | Requires trusted training pipeline |
| **Secure Aggregation + DP** | Aggregator cannot inspect individual updates and final updates are noise-protected | Higher complexity |
| **DP Analytics Layer** | Safer aggregate reporting | Privacy budget must be enforced across queries |

**Golden rule**

> <mark>DP is not a checkbox</mark>: the privacy claim is only meaningful if the system has calibrated noise, sensitivity control, privacy accounting, and governance over repeated access.

[Back to Contents](#contents)

---

<a id="federated-learning-architecture-patterns"></a>

## 3. Federated Learning Architecture Patterns

This section focuses on the major federated learning deployment patterns and frameworks an AI Architect should understand when designing distributed privacy-preserving training systems.

[Back to Contents](#contents)

---

<a id="cross-silo-federated-learning"></a>

### 3.1 Cross-Silo Federated Learning

**Pattern:** enterprise / B2B federated learning.

**Core Pattern**

Cross-silo FL is collaboration between a small number of organizations, usually 2-100 stable nodes. Each participant owns large, high-quality datasets and usually has enterprise-grade compute, GPU infrastructure, and governance processes.

**Architecture Characteristics**

| Area | Characteristics |
| --- | --- |
| Scale | 2-100 nodes |
| Data | Large, high-quality organizational datasets |
| Infrastructure | On-premises, private cloud, or controlled enterprise clusters |
| Network | High bandwidth, controlled connectivity |
| Security controls | Firewalls, reverse proxies, mTLS, IAM integration |
| Identity | Keycloak, Active Directory, enterprise IdP, service accounts |
| Reliability | Node dropout is critical because each node may represent a major data owner |

Reliability usually requires:

- Stateful training sessions.
- Strong checkpointing.
- Round restart or resume strategy.
- Model artifact versioning.
- Clear participation agreements.

**Core Components**

| Component | Role |
| --- | --- |
| Central aggregator | Performs aggregation and distributes updated global model |
| Client training node | Trains on local private data |
| Identity provider | Authenticates organizations, users, and services |
| Secure transport | Protects communication between participants |
| Model registry | Stores model versions and lineage |
| Governance layer | Tracks consent, data use, audit events, and compliance approvals |

**Key Challenge: Data Alignment and Entity Resolution**

Cross-silo FL often requires aligning schemas across organizations and sometimes matching shared entities, such as the same users across banks, hospitals, or insurers.

This must be done without exposing raw data.

Architectural concerns:

- Schema heterogeneity.
- Feature naming differences.
- Different coding systems or units.
- Missing fields.
- Different data quality levels.
- Entity matching without raw identifier exposure.

**Architect Guidance**

- Plan for schema heterogeneity from the beginning.
- Introduce a feature standardization layer.
- Use privacy-preserving record linkage (PPRL) when entities must be matched.
- Define shared feature contracts and validation rules.
- Expect heavy governance, legal review, compliance, and audit requirements.
- Treat each participant as a separate security and trust domain.

[Back to Contents](#contents)

---

<a id="cross-device-federated-learning"></a>

### 3.2 Cross-Device Federated Learning

**Pattern:** B2C / edge federated learning.

**Core Pattern**

Cross-device FL trains across very large numbers of unreliable edge devices, such as phones, browsers, IoT devices, laptops, and embedded systems.

**Architecture Characteristics**

| Area | Characteristics |
| --- | --- |
| Scale | Potentially millions of devices |
| Reliability | Highly unstable infrastructure |
| Participation | Partial, intermittent, and opportunistic |
| Training trigger | Device idle, charging, and connected to Wi-Fi |
| Aggregation | Often asynchronous or semi-synchronous |
| Dropout | High dropout is normal; around 90% dropout may be expected |

**Training Triggers**

Typical client eligibility conditions:

- Device is idle.
- Device is charging.
- Device is connected to Wi-Fi.
- Battery level is sufficient.
- Thermal limits are safe.
- User privacy settings allow participation.

**Aggregation**

Cross-device FL often uses asynchronous or dropout-tolerant aggregation patterns, such as FedAsync-style designs. The system must assume that many selected devices will never complete a round.

**Optimization Constraints**

| Constraint | Typical technique |
| --- | --- |
| RAM/VRAM | Quantization, small local batches, memory-aware training |
| Bandwidth | Compressed updates, sparse gradients, delta updates |
| Compute | Distillation, adapters, short local epochs |
| Model size | LoRA adapters only, INT8 / INT4 quantization |
| Battery | Strict scheduling and training windows |

**Security**

Secure Aggregation (SecAgg) is usually mandatory.

SecAgg goal:

> The server sees only aggregated gradients or updates, not individual device updates.

Typical aggregation group:

- Around 1000 devices per aggregation group, depending on system design.
- Enough participants are needed so individual updates are hidden in the aggregate.

**Architect Guidance**

- Design for extreme unreliability.
- Expect partial participation as normal behavior.
- Optimize communication efficiency aggressively.
- Send tiny updates whenever possible.
- Avoid assuming stable client identity or availability.
- Build privacy, consent, and opt-out behavior into the client lifecycle.

[Back to Contents](#contents)

---

<a id="centralized-fl"></a>

### 3.3 Centralized (Hub-and-Spoke) FL

**Pattern:** hub-and-spoke federated learning.

**Core Pattern**

A single central aggregator, or hub, coordinates training across multiple clients, or spokes.

```text
Client A -> \
Client B ->  Central Aggregator -> Global Model
Client C -> /
```

**Control Plane Design**

Hub responsibilities:

- Orchestrates training rounds.
- Handles service discovery.
- Distributes training triggers.
- Coordinates client participation.
- Performs key management or integrates with KMS.
- Publishes updated global model versions.

Spoke responsibilities:

- Train locally on private data.
- Initiate outbound connections when possible.
- Send updates to the hub.
- Pull updated global models.

Outbound spoke connections are NAT-friendly and easier to deploy in enterprise networks.

**Risks**

| Risk | Why it matters |
| --- | --- |
| Single point of failure | Hub outage stops training |
| Single point of trust | Hub becomes a sensitive coordination boundary |
| DDoS target | Central endpoint is exposed to all participants |
| Exfiltration target | Aggregated gradients and model artifacts may be valuable |

**Mitigations**

- Keep the hub as stateless as possible.
- Externalize session state to an in-memory DB or distributed state store.
- Store model artifacts in object storage such as S3 or MinIO.
- Add multi-hub redundancy.
- Use rate limiting and DDoS protection.
- Use mTLS and signed updates.
- Apply secure aggregation when individual updates are sensitive.

**Architect Guidance**

Use centralized FL when simplicity is more important than maximum resilience or decentralization.

Good fit:

- Cross-silo projects with a trusted coordinator.
- Internal enterprise deployments.
- Regulated collaborations where one party is legally responsible for orchestration.

Add redundancy and strong perimeter controls before production deployment.

[Back to Contents](#contents)

---

<a id="decentralized-fl"></a>

### 3.4 Decentralized (Peer-to-Peer / Gossip) FL

**Pattern:** peer-to-peer / gossip federated learning.

**Core Pattern**

Decentralized FL has no central coordinator. Participants exchange updates with peers and aggregate in a distributed manner.

```text
Node A <-> Node B
  ^         |
  |         v
Node D <-> Node C
```

**Control Plane**

Common techniques:

- Gossip protocols.
- Push-sum.
- Peer averaging.
- Mesh networking.

Network options:

- WireGuard.
- Nebula.
- Private mesh VPN.
- Overlay networks.

**Advantages**

- High fault tolerance.
- Can continue even if a significant fraction of nodes fail, for example around 30%.
- No single point of trust (ideal for competing banks).
- Stronger fit for low-trust collaborations.

**Risks**

- Massive network overhead.
- Slower convergence.
- Complex distributed monitoring.
- Harder debugging and incident response.
- Harder governance and compliance evidence.
- Often over-engineered for ordinary real-world use.

**Architect Guidance**

Use decentralized FL when trust is low and a central coordinator is unacceptable, such as competing organizations that cannot agree on a neutral hub.

Avoid unless there is a strong requirement for decentralization. Most production systems are simpler and easier to operate with a centralized or semi-centralized design.

[Back to Contents](#contents)

---

<a id="split-learning"></a>

### 3.5 Split Learning

**Pattern:** SplitNN / split model training.

**Core Pattern**

The model is split between:

- Client-side front layers.
- Server-side back layers.

**Workflow**

1. Client computes the forward pass through the front layers.
2. Client sends activations, often called smashed data, to the server.
3. Server completes forward pass and backward pass.
4. Server sends gradients back to the client.
5. Client updates its local front layers.

```text
Raw data -> client layers -> activations -> server layers -> loss
                                  <- gradients <-
```

**Advantages**

- Raw data never leaves the client.
- Lower client compute requirements than full local model training.
- Enables training larger models on weaker devices.
- Useful when the client cannot host the full model.

**Trade-offs and Risks**

| Risk | Explanation |
| --- | --- |
| Synchronous bottleneck | Client and server must communicate during forward/backward passes |
| Network latency | Training speed is sensitive to round-trip latency |
| Activation leakage | Activations can reveal information about original inputs |
| Model inversion | Attackers may reconstruct inputs from activations |
| Server dependency | Server-side layers become a central compute dependency |

**Architect Guidance**

Use split learning when clients are resource-constrained but raw data cannot leave the client boundary.

Combine with:

- Differential Privacy on activations.
- Encryption for activations in transit.
- TEE-based server-side computation.
- Access control and audit logging.
- Activation compression to reduce network cost.

[Back to Contents](#contents)

---

<a id="fl-frameworks-comparison"></a>

### 3.6 FL Frameworks Comparison

| Framework | Positioning | Strengths | Weaknesses |
| --- | --- | --- | --- |
| Flower | Industry-standard general-purpose FL | Framework-agnostic, supports PyTorch/TensorFlow, scalable, flexible | SecAgg must be added manually, MLOps integration is DIY |
| OpenFL | Enterprise / healthcare FL | Built-in PKI authentication, DICOM (Digital Imaging and Communications in Medicine; ```.dcm```) support, native TEE integration | Tied to Envoy, less flexible topology |
| FATE | Heavy enterprise, banks, vertical FL | All-in-one HE/MPC support, strong for vertical FL | Complex monolithic architecture, hard Kubernetes deployment |
| PySyft | Research / data governance | Data-owner-controlled execution model, simple Python API | Low production performance |

**Flower**

- Strong fit for general FL experiments and flexible production prototypes.
- Good when the team wants framework freedom.
- Requires explicit architecture work for security, SecAgg, governance, registry, and MLOps.

**OpenFL**

- Strong fit for enterprise and healthcare settings.
- Useful when PKI authentication, DICOM support, and TEE integration matter.
- Less flexible if the target topology does not match its assumptions.

**FATE**

- Strong fit for banking, vertical FL, and enterprise privacy-preserving machine learning.
- Includes homomorphic encryption and MPC capabilities.
- Operationally heavy and can be difficult to run cleanly on Kubernetes.

**PySyft**

- Strong fit for research, privacy governance exploration, and data-owner-controlled execution.
- Simple developer experience.
- Not usually the first choice for high-performance production FL.

[Back to Contents](#contents)

---

<a id="federated-learning-decision-matrix"></a>

### 3.7 Federated Learning Decision Matrix

Choose the pattern based on:

| Decision factor | Best-fit pattern |
| --- | --- |
| Few organizations with large datasets | Cross-silo FL |
| Millions of devices | Cross-device FL |
| Trusted coordinator exists | Centralized FL |
| Low trust and no acceptable coordinator | Decentralized FL |
| Weak clients | Split Learning |
| Strong clients | Federated Learning |
| High privacy needs | Add Secure Aggregation, DP, SMPC, or TEE |
| High dropout expected | Cross-device FL with asynchronous/dropout-tolerant aggregation |
| Strong enterprise governance | Cross-silo FL with identity, audit, and legal controls |

**Data Distribution**

- Few organizations, large data -> Cross-silo.
- Many devices, small local data -> Cross-device.

**Trust Model**

- Trusted coordinator -> Centralized.
- Low trust -> Decentralized or secure aggregation with strong governance.

**Compute Constraints**

- Weak clients -> Split Learning or adapters only.
- Strong clients -> Federated Learning with local training.
  * **Example**: *each hospital has its own GPU server, trains the full model locally on local patient data for a few epochs, then sends only model updates to the aggregator.*

**Security Needs**

High-security deployments commonly add:

- Secure Aggregation.
- Differential Privacy.
- SMPC.
- TEE.
- Strong identity, attestation, and audit.

[Back to Contents](#contents)

---

<a id="federated-learning-practical-design-checklist"></a>

### 3.8 Federated Learning Practical Design Checklist

**Define**

- Threat model.
- Regulatory constraints.
- Data ownership boundaries.
- Trust assumptions.
- Participant eligibility.
- Model ownership and IP rules.

**Decide**

- Centralized vs. decentralized control plane.
- Synchronous vs. asynchronous training.
- Cross-silo vs. cross-device deployment.
- Full FL vs. Split Learning.
- Secure aggregation and encryption strategy.
- Model registry and rollout strategy.

**Optimize**

- Communication cost.
- Model size.
- Fault tolerance.
- Client compute and memory.
- Round duration.
- Aggregation frequency.
- Checkpoint and recovery behavior.

**Always Plan For**

- Node dropout.
- Privacy leakage through gradients or activations.
- Poisoned or malicious updates.
- Distributed observability.
- Auditability.
- Participant onboarding/offboarding.
- Failed rounds and rollback.

[Back to Contents](#contents)

---

<a id="advanced-ppa-stack-for-ai-architects"></a>

## 4. Advanced PPA Stack for AI Architects

This section covers advanced privacy-preserving architecture concepts that commonly appear in **production federated learning** and **regulated AI systems**: **gradient compression**, **vector quantization**, **Fully Homomorphic Encryption (FHE)**, **Secure Multi-Party Computation (SMPC)**, **secure aggregation**, and tooling choices.

> <mark>Architect focus</mark>: these techniques are not interchangeable. **Compression solves scale**, **FHE solves encrypted computation**, and **SMPC solves multi-party privacy under network constraints**.

[Back to Contents](#contents)

---

<a id="gradient-compression-and-vector-quantization"></a>

### 4.1 Gradient Compression and Vector Quantization

**Gradient compression is critical for federated learning** because raw model updates can be extremely large.

**Scale example**

- **10B parameters in FP32** is approximately **40 GB per training round**.

This creates:

- **WAN congestion**.
- **Training slowdown**.
- **Infeasible cross-device FL** for large models.
- Higher cost for cross-silo FL when participants communicate over constrained links.

**Core Problem**

Federated learning repeatedly exchanges **model updates, gradients, weights, or adapter parameters**. Without compression, **network transfer becomes the bottleneck before local training compute does**.

#### 4.1.1 Sparsification: Top-K Gradients

**Sparsification sends only the largest gradient values.**

*Typical pattern:*

- Select the **Top-K percent largest gradients**.
- Send only those selected values and their positions.
- Store unsent gradients locally through **error accumulation** or **error feedback**.
- Reintroduce accumulated error in later rounds.

*Trade-offs:*

| Benefit | Risk |
| --- | --- |
| **Major bandwidth reduction** | Slower convergence because information is discarded or delayed |
| Works well with large sparse updates | Requires careful tuning of K and error feedback |
| Useful for WAN-constrained FL | Can destabilize training under non-IID data |

#### 4.1.2 Vector Quantization

**Vector quantization reduces numerical precision before transmitting updates.**

*Examples:*

- **FP32 -> INT8**.
- **FP32 -> INT4**.
- **FP32 -> 1-bit quantization**.

*Impact:*

- **Drastically reduces payload size**.
- Makes large-scale FL more practical.
- Can be combined with sparsification and delta updates.

*Trade-offs:*

| Benefit | Risk |
| --- | --- |
| **Efficient communication** | Numerical instability |
| **Lower transfer cost** | Accuracy drop if quantization is too aggressive |
| Better fit for edge devices | Extra quantize/dequantize compute on clients |

#### 4.1.3 FedLoRA: Parameter-Efficient FL

**FedLoRA applies parameter-efficient fine-tuning to federated learning.**

*Core idea:*

- Freeze the base model.
- Train only **low-rank adapter matrices**.
- Share only **adapter updates** instead of full model weights.

*Impact:*

- Payloads can drop from **GBs** to approximately **10-50 MB**.
- Training becomes more feasible on constrained clients.
- Global aggregation operates over adapter parameters rather than the full model.

*Hidden cost:*

- Quantize/dequantize work shifts compute to **client CPU**.
- Adapter-only training may not capture all domain shifts.
- Adapter versioning and compatibility become important architecture concerns.

**Architect Guidance**

<mark>Recommended stack</mark>:

- **Quantization**.
- **Sparsification**.
- **Adapter-based training**.
- Delta updates.
- Error feedback.

*Always tune:*

- **Compression ratio vs. convergence speed**.
- **Payload size vs. model quality**.
- Client CPU cost vs. network savings.
- Synchronous round duration vs. dropout risk.

<mark>Mandatory fit</mark>:

- **Cross-device FL**.
- **WAN-constrained environments**.
- Edge personalization.
- Large model FL where full update exchange is impossible.

[Back to Contents](#contents)

---

<a id="fully-homomorphic-encryption"></a>

### 4.2 Fully Homomorphic Encryption

**Fully Homomorphic Encryption (FHE)** allows computation directly on **encrypted data**.

**Core property**

```text
f(E(x)) = E(f(x))
```

*Meaning:*

- Data is **encrypted before processing**.
- The server computes over **ciphertext**.
- Decryption is **not required during processing**.
- The server does **not** see plaintext inputs, gradients, or intermediate values.

**Value Proposition**

FHE enables **zero-trust ML** patterns where the compute provider is not trusted with plaintext data.

It can protect:

- **Model inputs**.
- Sensitive features.
- Gradients or updates.
- Intermediate computation.
- Inference requests in regulated environments.

> <mark>Key value</mark>: FHE lets the platform compute without becoming a plaintext trust boundary.

**Key Constraints**

| Constraint | Practical impact |
| --- | --- |
| **Ciphertext expansion** | Data often becomes **10x-50x larger** |
| **Computation overhead** | Workloads can be **100x-10000x slower** |
| **Limited operations** | Best fit for polynomial arithmetic |
| **Model changes** | ReLU and Sigmoid often need polynomial approximations |
| Operational complexity | Parameter selection, key handling, and performance tuning are specialized |

**Cryptographic Foundation**

FHE is commonly based on **lattice cryptography**.

*Important foundations:*

- **Learning With Errors (LWE)**.
- **Ring Learning With Errors (Ring-LWE)**.
- **Post-quantum security assumptions**.

*Common schemes:*

The acronyms are named after the scheme authors:

| Scheme | Full name | Main data type |
| --- | --- | --- |
| **CKKS** | **Cheon-Kim-Kim-Song** | Approximate real/complex numbers |
| **BFV** | **Brakerski-Fan-Vercauteren** | Exact integers |
| **BGV** | **Brakerski-Gentry-Vaikuntanathan** | Exact integers |

Short distinction:

- **CKKS**: best for ML because it supports *approximate arithmetic*, which fits floating-point-style workloads.
- **BFV**: best when you need *exact integer arithmetic*.
- **BGV**: also exact integer arithmetic, often used in more cryptography-heavy systems with batching and leveled FHE designs.

| Scheme | Best fit |
| --- | --- |
| **CKKS** | Approximate arithmetic and floating-point-like ML workloads |
| **BFV** | Exact integer arithmetic |
| **BGV** | Exact integer arithmetic and batching use cases |

**Architect Guidance**

<mark>Use FHE for</mark>:

- **High-sensitivity inference**.
- **Regulated environments** such as finance, healthcare, and government.
- Scenarios where the server must not access plaintext.
- Batch workloads where latency is not strict.

<mark>Avoid FHE for</mark>:

- **Real-time large-scale training**.
- Latency-sensitive online inference unless the model is very small.
- Workloads requiring unsupported nonlinear operations without approximation.
- Systems where operational teams cannot support cryptographic parameter management.

[Back to Contents](#contents)

---

<a id="fhe-tooling-landscape"></a>

### 4.3 FHE Tooling Landscape

FHE tooling differs sharply by audience. Some tools target **ML engineers**, while others target **cryptographers and systems engineers**.

| Tool | Positioning | Strength | Weakness |
| --- | --- | --- | --- |
| **Zama Concrete ML** | Python-first ML-to-FHE workflow | **PyTorch/ONNX to FHE compilation** | High RAM usage and workload constraints |
| **TenSEAL** | CKKS tensor operations | Convenient encrypted tensor abstractions | Semi-maintained and narrower ecosystem |
| **HElib** | IBM cryptographic library | Deep cryptographic primitives | Steep learning curve |
| **Lattigo** | Go-based FHE library | High concurrency performance | Low ML abstraction level |

#### 4.3.1 Zama Concrete ML

*Best for:*

- ML engineers.
- Python-first teams.
- Prototyping encrypted inference.
- Compiling PyTorch or ONNX-style models into FHE-compatible execution.

*Trade-off:*

- **High RAM usage**.
- Model and operation constraints must be designed around early.

#### 4.3.2 TenSEAL

*Best for:*

- CKKS tensor operations.
- Encrypted vector and matrix experimentation.
- Smaller prototypes that need tensor-style APIs.

*Trade-off:*

- **Semi-maintained**.
- Less suitable as the default choice for a large production ML platform.

#### 4.3.3 HElib

*Best for:*

- Deep cryptographic work.
- Custom encrypted computation protocols.
- Teams with cryptography expertise.

*Trade-off:*

- **Very steep learning curve**.
- Low-level primitives require significant engineering.

#### 4.3.4 Lattigo

*Best for:*

- Go-based services.
- Concurrent cryptographic systems.
- Custom privacy-preserving infrastructure.

*Trade-off:*

- Lower ML abstraction level.
- Requires more custom system integration.

**Architect Takeaway**

<mark>Choose</mark>:

- **Zama** when ML pipeline integration is the main goal.
- **Lattigo / HElib** when building custom cryptographic systems.
- **TenSEAL** when prototyping CKKS tensor operations.

[Back to Contents](#contents)

---

<a id="secure-aggregation-with-fhe"></a>

### 4.4 Secure Aggregation with FHE

**Secure aggregation protects individual client updates** during federated learning aggregation.

*Goal:*

- Prevent **gradient leakage**.
- Reduce model inversion risk from individual updates.
- Prevent an honest-but-curious server from inspecting client gradients.

**Protocol Flow**

1. **Clients encrypt gradients** with a public key.
2. The **server aggregates encrypted gradients** through homomorphic summation.
3. Participants **jointly decrypt only the aggregate** through threshold cryptography.
4. The server receives the **aggregate update**, not individual plaintext updates.

```text
Client updates -> encrypted gradients -> homomorphic sum -> threshold decrypt aggregate
```

**Key Mechanisms**

**Threshold cryptography:**

- The private key is split across participants.
- **Shamir Secret Sharing** is a common building block.
- The server **cannot decrypt alone**.
- A threshold number of participants must cooperate to decrypt the aggregate.

**Trade-offs**

| Benefit | Cost |
| --- | --- |
| **Strong protection against individual gradient inspection** | Huge network overhead from ciphertext inflation |
| **Server can aggregate without plaintext** | Expensive cryptographic compute |
| Good fit for higher-trust B2B collaboration | Often overkill for cross-device FL |

**Best Fit**

<mark>Use FHE-based secure aggregation for</mark>:

- **B2B or cross-silo environments**.
- Consortium ML across banks, hospitals, insurers, or regulated enterprises.
- Cases where participants can tolerate higher latency and cost.
- Smaller participant groups with strong privacy requirements.

<mark>Avoid as the default for</mark>:

- **Massive cross-device FL**.
- Mobile networks.
- Systems with strict round-time limits.

[Back to Contents](#contents)

---

<a id="secure-multi-party-computation"></a>

### 4.5 Secure Multi-Party Computation

**Secure Multi-Party Computation (SMPC)** allows parties to jointly compute a result without revealing their private inputs.

**Core idea**

- Data is split into **shares**.
- Each party computes on **shares**.
- No one sees the **full private data**.
- The final result is reconstructed only when allowed by the protocol.

**Mechanisms**

Common SMPC mechanisms include:

- **Secret sharing**.
- **Garbled circuits**.
- **Oblivious transfer**.

**Advantages over FHE**

| Advantage | Meaning |
| --- | --- |
| **Lower compute cost** | Usually much cheaper than FHE for many workloads |
| Efficient simple operations | Works well with XOR, AND, additions, and multiplications depending on the protocol |
| **Natural fit for aggregation** | Strong fit for secure gradient aggregation |

**Critical Limitation: Network Latency Bottleneck**

SMPC requires **communication between parties during computation**.

<mark>Practical requirements</mark>:

- **Low latency**, often less than **20 ms** for interactive protocols.
- **LAN, private network, or dedicated WAN**.
- Stable participants.
- Careful handling of synchronization and dropout.

> <mark>Architect warning</mark>: high-latency global networks can make SMPC impractical because the protocol may require many communication rounds.

**Typical Use Cases**

- **Secure gradient aggregation**.
- Cross-party analytics.
- Classical ML such as logistic regression.
- Tree-based models such as XGBoost.
- Private set intersection and privacy-preserving record linkage.

**Architect Guidance**

<mark>Use SMPC when</mark>:

- **Strong privacy** is needed.
- Participants are stable.
- Low-latency networking is available.
- The workload can tolerate synchronization.

<mark>Avoid SMPC when</mark>:

- Participants are globally distributed over high-latency networks.
- Dropout is common.
- The system requires real-time performance across many parties.
- Operational complexity cannot be justified.

[Back to Contents](#contents)

---

<a id="smpc-frameworks"></a>

### 4.6 SMPC Frameworks

| Framework | Positioning | Strength | Weakness |
| --- | --- | --- | --- |
| **SecretFlow** | Enterprise privacy-preserving ML platform | **Combines SMPC, FHE, TEE, and FL** | Complex architecture |
| **MP-SPDZ** | Academic high-performance SMPC | **Supports 40+ protocols** | Custom language and limited ML convenience |
| **CrypTen** | PyTorch-native SMPC | **Easy ML integration and GPU acceleration** | Slower community development |

#### 4.6.1 SecretFlow

*Best for:*

- Enterprise privacy-preserving ML.
- Architectures that combine SMPC, FHE, TEE, and FL.
- Production-oriented multi-party collaboration.

*Strength:*

- **Broad production-ready privacy stack**.

*Weakness:*

- **Complex architecture**.
- Higher operational learning curve.

#### 4.6.2 MP-SPDZ

*Best for:*

- Research.
- High-performance SMPC experiments.
- Protocol benchmarking.
- Custom secure computation systems.

*Strength:*

- Supports **more than 40 protocols**.

*Weakness:*

- Custom language.
- No easy ML integration for ordinary ML teams.

#### 4.6.3 CrypTen

*Best for:*

- ML-native prototyping.
- PyTorch-style secure computation experiments.
- Teams that want familiar tensor workflows.

*Strength:*

- **Easy ML integration**.
- **GPU acceleration**.

*Weakness:*

- Slowed community development.
- Requires validation before committing to production use.

**Architect Takeaway**

<mark>Choose</mark>:

- **SecretFlow** for enterprise privacy-preserving ML platforms.
- **MP-SPDZ** for research and high-performance protocol work.
- **CrypTen** for ML-native prototyping.

[Back to Contents](#contents)

---

<a id="design-trade-off-summary"></a>

### 4.7 Design Trade-off Summary

| Dimension | FHE | SMPC | Compression |
| --- | --- | --- | --- |
| **Privacy** | **Very high** | **High** | Low |
| **Compute cost** | **Extremely high** | Moderate | Low |
| **Network cost** | **Very high** | High and synchronization-heavy | **Very low** |
| **Scalability** | Low | Medium and LAN-bound | **High** |
| **Main use case** | **Secure inference** | **Secure training and secure aggregation for smaller groups** | **FL scaling** |

**Architect interpretation**

- **FHE** is strongest when plaintext must never be exposed to the compute server.
- **SMPC** is strongest when several parties can jointly compute with low-latency communication.
- **Compression does not provide privacy by itself**, but it is often mandatory to make FL feasible.
- **Secure aggregation protects individual updates**, but still needs governance, authentication, and poisoning defenses.

> <mark>Design rule</mark>: do not pick the strongest privacy primitive by default. Pick the strongest primitive the workload, latency budget, and operations team can actually sustain.

[Back to Contents](#contents)

---

<a id="architect-playbook"></a>

### 4.8 Architect Playbook

**For Cross-Device FL**

<mark>Must include</mark>:

- **Quantization**.
- **Sparsification**.
- Small local epochs.
- Dropout-tolerant aggregation.
- **Secure aggregation, usually not FHE**.

*Architect rule:*

- **Optimize payload size and dropout tolerance first**.
- Do not assume mobile or edge clients can handle heavy cryptography.

**For Cross-Silo FL: Enterprise**

<mark>Common stack</mark>:

- **FL**.
- **SMPC or FHE** for secure aggregation or protected computation.
- Optional **Differential Privacy**.
- Strong identity, audit, and governance.
- Model registry and participant-specific lineage.

*Architect rule:*

- **Cross-silo FL is as much a governance and operations problem as an ML problem**.

**For Highly Regulated Systems**

<mark>Use</mark>:

- **FHE** for high-sensitivity inference.
- **SMPC** for secure training or aggregation.
- **TEE** when practical confidential execution is needed with lower overhead.
- **DP** for released outputs or model update privacy.
- **Hybrid architecture** rather than a single technique.

**Golden Rule**

Always balance:

- **Privacy**.
- **Performance**.
- **Network cost**.
- **System complexity**.
- Operational maturity.
- Regulatory evidence.

> <mark>Golden rule</mark>: no single technique wins. Design a **hybrid stack** from the **threat model**, **workload**, **latency constraints**, and **trust boundaries**.

[Back to Contents](#contents)

---

<a id="ai-architect-design-and-operations"></a>

## 5. AI Architect Design and Operations

This section groups architecture-level decision tools, reference stack patterns, threats, active recall prompts, and final mental models.

[Back to Contents](#contents)

---

<a id="architect-decision-matrix"></a>

### 5.1 Architect Decision Matrix

| Need | Strong candidate | Why |
| --- | --- | --- |
| Prevent centralizing raw data | Federated Learning | Data stays with owners or devices |
| Protect individual records in aggregate outputs | Differential Privacy | Formal record-level privacy guarantee |
| Hide individual client updates from aggregator | FL + SMPC secure aggregation | Aggregator sees only combined result |
| Run sensitive inference in cloud with lower overhead | TEE | Practical confidential execution |
| Compute on encrypted data with minimal trust in processor | FHE | Data stays encrypted during computation |
| Multi-organization analytics without exposing inputs | SMPC | Parties jointly compute without sharing raw inputs |
| Public release of statistics | DP | Limits inference about any single record |
| Mobile/edge personalization | Cross-device FL | Local data stays on device |
| Multi-hospital or multi-bank model training | Cross-silo FL + secure aggregation | Collaboration without data pooling |

Decision questions:

1. Is raw data allowed to leave the owner boundary?
2. Who is trusted: clients, server, aggregator, cloud provider, hardware vendor?
3. Are model updates considered sensitive?
4. Is the output itself privacy-sensitive?
5. Is the workload training, inference, analytics, or reporting?
6. What are the latency, cost, and accuracy constraints?

[Back to Contents](#contents)

---

<a id="reference-privacy-preserving-ai-stack"></a>

### 5.2 Reference Privacy-Preserving AI Stack

A practical privacy-preserving AI architecture can be layered like this:

```text
Data owners / clients
  -> local preprocessing and minimization
  -> optional local Differential Privacy
  -> local training or feature computation
  -> encrypted transport
  -> secure aggregation with SMPC or TEE
  -> global model update
  -> model validation and privacy accounting
  -> model registry
  -> privacy-aware inference or reporting
```

Example cross-silo FL architecture:

```text
Hospital A data -> local model update \
Hospital B data -> local model update  -> secure aggregation -> global model
Hospital C data -> local model update /

Optional protections:
- Local DP on updates
- SMPC secure aggregation
- TEE-based coordinator
- Audit logging and privacy budget tracking
```

Architectural controls to include:

- Data minimization before training.
- Authentication and authorization for clients.
- Signed model updates.
- Encrypted communication.
- Secure aggregation.
- Privacy budget tracking.
- Model update anomaly detection.
- Poisoning and backdoor detection.
- Model registry with lineage.
- Audit logs for training rounds and releases.

[Back to Contents](#contents)

---

<a id="common-threats-and-mitigations"></a>

### 5.3 Common Threats and Mitigations

| Threat | Example | Mitigation |
| --- | --- | --- |
| Membership inference | Attacker infers whether a person was in training data | Differential Privacy, regularization, output limiting |
| Gradient reconstruction | Model updates reveal training examples | Secure aggregation, DP, update clipping |
| Model inversion | Attacker reconstructs sensitive attributes from model behavior | DP, access control, output restriction |
| Activation inversion | Split Learning activations reveal client inputs | DP on activations, encryption, TEE, activation minimization |
| Malicious client update | Client poisons global model | Robust aggregation, anomaly detection, client attestation |
| Honest-but-curious aggregator | Aggregator inspects individual updates | SMPC secure aggregation, TEE |
| Cloud operator exposure | Cloud infrastructure can observe sensitive processing | TEE, encryption, strict access control |
| Output leakage | Released analytics reveal individuals | DP, k-anonymity-style checks, suppression policies |
| Key compromise | Encrypted data becomes readable | Key management, rotation, HSM/KMS, separation of duties |

Important distinction:

> Privacy-preserving AI is not only about protecting raw data. It must also protect updates, activations, intermediate values, models, outputs, logs, and metadata.

[Back to Contents](#contents)

---

<a id="active-recall-prompts"></a>

### 5.4 Active Recall Prompts

Use these to test understanding:

1. What privacy problem does Differential Privacy solve?
2. Why does Federated Learning not automatically guarantee privacy?
3. What can gradient reconstruction attacks reveal?
4. Why does FL often need secure aggregation?
5. What is the difference between SMPC, FHE, and TEE?
6. Why is FHE powerful but expensive?
7. What does a privacy budget control?
8. Why can adding DP noise reduce model quality?
9. What is the difference between cross-device FL and cross-silo FL?
10. Why should architects start with a threat model before choosing a privacy technology?
11. When is centralized FL a reasonable design?
12. Why is decentralized FL often operationally expensive?
13. Why can Split Learning still leak private information?
14. What does PPRL solve in cross-silo FL?
15. Which framework would you consider first for enterprise healthcare FL, and why?
16. Why is gradient compression often mandatory for cross-device FL?
17. What is the trade-off between compression ratio and convergence speed?
18. Why does FedLoRA reduce FL payload size?
19. Why is FHE powerful but usually unsuitable for real-time large-scale training?
20. What is ciphertext expansion, and why does it matter architecturally?
21. Why does SMPC become difficult across high-latency networks?
22. When would SecretFlow be a better fit than MP-SPDZ?
23. Why does secure aggregation protect updates but not solve poisoning risk?
24. What do epsilon and delta control in Differential Privacy?
25. Why does Local DP usually reduce model utility more than Global DP?
26. Which DP framework is the best fit for PyTorch DP-SGD training?
27. Why can access to raw gradients enable model inversion or data extraction?
28. Why is privacy budget accounting mandatory across repeated queries or training rounds?

[Back to Contents](#contents)

---

<a id="final-mental-model"></a>

### 5.5 Final Mental Model

Privacy-preserving AI architecture is a layered design problem:

```text
Differential Privacy protects records and outputs.
Local DP protects data before it leaves the client, but reduces utility.
Global DP preserves more utility, but requires a trusted aggregator.
DP-SGD clips gradients and adds calibrated noise to reduce leakage.
Federated Learning protects data locality.
Compression makes federated update exchange feasible at scale.
Secure Aggregation protects individual client updates.
Split Learning separates client and server model computation.
SMPC protects joint computation between parties.
FHE protects computation over encrypted data.
TEE protects computation inside a hardware-isolated environment.
```

Federated learning architecture choices:

```text
Cross-silo = few stable organizations.
Cross-device = many unreliable devices.
Centralized FL = simpler hub-and-spoke coordination.
Decentralized FL = no central trust point, but higher complexity.
Split Learning = weaker clients can train with server-side model layers.
```

Advanced PPA architecture choices:

```text
epsilon/delta = privacy budget and failure probability.
Opacus = PyTorch DP-SGD.
TensorFlow Privacy = TensorFlow DP-SGD.
Google DP Core = DP analytics and aggregate queries.
Compression = scalability tool, not a privacy guarantee.
FHE = strongest plaintext protection, but high compute and network cost.
SMPC = strong multi-party privacy, but latency-sensitive.
FHE secure aggregation = strong fit for smaller cross-silo groups.
Ordinary secure aggregation = better default for massive cross-device FL.
```

No single technique solves everything.

The strongest architectural pattern is:

> Define the threat model first, then combine statistical, algorithmic, cryptographic, operational, and governance controls to balance privacy, utility, latency, cost, scalability, and deployment feasibility.

[Back to Contents](#contents)
