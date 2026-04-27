# Federated Learning and Privacy-Preserving Architecture

<a id="contents"></a>

## Contents

- [PPA Ecosystem Overview](#ppa-ecosystem-overview)
- [1. Statistical Methods](#statistical-methods)
- [2. Algorithmic Approaches](#algorithmic-approaches)
- [3. Cryptographic Methods](#cryptographic-methods)
- [4. Hybridization Principle](#hybridization-principle)
- [5. Practical Architecture Guidelines](#practical-architecture-guidelines)
- [6. Architect Decision Matrix](#architect-decision-matrix)
- [7. Reference Privacy-Preserving AI Stack](#reference-privacy-preserving-ai-stack)
- [8. Common Threats and Mitigations](#common-threats-and-mitigations)
- [9. Active Recall Prompts](#active-recall-prompts)
- [10. Final Mental Model](#final-mental-model)

---

<a id="ppa-ecosystem-overview"></a>

## PPA Ecosystem Overview

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

## 1. Statistical Methods

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

DP is usually placed at one of these points:

| Placement | Example |
| --- | --- |
| Data/query layer | Add noise to aggregate analytics |
| Training layer | Apply DP-SGD during model training |
| Client layer | Apply local DP before sending updates |
| Reporting layer | Release only DP-protected metrics |

[Back to Contents](#contents)

---

<a id="algorithmic-approaches"></a>

## 2. Algorithmic Approaches

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

**Architectural Use**

Common FL architecture:

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

## 3. Cryptographic Methods

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

## 4. Hybridization Principle

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

## 5. Practical Architecture Guidelines

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

<a id="architect-decision-matrix"></a>

## 6. Architect Decision Matrix

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

## 7. Reference Privacy-Preserving AI Stack

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

## 8. Common Threats and Mitigations

| Threat | Example | Mitigation |
| --- | --- | --- |
| Membership inference | Attacker infers whether a person was in training data | Differential Privacy, regularization, output limiting |
| Gradient reconstruction | Model updates reveal training examples | Secure aggregation, DP, update clipping |
| Model inversion | Attacker reconstructs sensitive attributes from model behavior | DP, access control, output restriction |
| Malicious client update | Client poisons global model | Robust aggregation, anomaly detection, client attestation |
| Honest-but-curious aggregator | Aggregator inspects individual updates | SMPC secure aggregation, TEE |
| Cloud operator exposure | Cloud infrastructure can observe sensitive processing | TEE, encryption, strict access control |
| Output leakage | Released analytics reveal individuals | DP, k-anonymity-style checks, suppression policies |
| Key compromise | Encrypted data becomes readable | Key management, rotation, HSM/KMS, separation of duties |

Important distinction:

> Privacy-preserving AI is not only about protecting raw data. It must also protect updates, intermediate values, models, outputs, logs, and metadata.

[Back to Contents](#contents)

---

<a id="active-recall-prompts"></a>

## 9. Active Recall Prompts

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

[Back to Contents](#contents)

---

<a id="final-mental-model"></a>

## 10. Final Mental Model

Privacy-preserving AI architecture is a layered design problem:

```text
Differential Privacy protects records and outputs.
Federated Learning protects data locality.
SMPC protects joint computation between parties.
FHE protects computation over encrypted data.
TEE protects computation inside a hardware-isolated environment.
```

No single technique solves everything.

The strongest architectural pattern is:

> Define the threat model first, then combine statistical, algorithmic, and cryptographic protections to balance privacy, utility, latency, cost, and operational feasibility.

[Back to Contents](#contents)
