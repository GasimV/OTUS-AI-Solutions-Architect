# Infrastructure & Fault Tolerance - Active Recall Notes

## Contents

1. [Storage Fundamentals](#1-storage-fundamentals)
   - [Logical Disks](#logical-disks)
   - [RAID (Redundant Array of Independent Disks)](#raid-redundant-array-of-independent-disks)
2. [Fault Tolerance Concepts](#2-fault-tolerance-concepts)
   - [Cluster Example](#cluster-example-server1--server2--witness)
3. [RTO & RPO](#3-rto--rpo-critical)
4. [Virtualization](#4-virtualization)
5. [Containerization](#5-containerization)
6. [Container vs VM Security](#6-container-vs-vm-security)
7. [HDFS (Hadoop Distributed File System)](#7-hdfs-hadoop-distributed-file-system)
8. [Hadoop Ecosystem](#8-hadoop-ecosystem)
9. [HA vs DR Regimes](#9-ha-vs-dr-regimes)
10. [FC SAN (Fiber Channel Storage)](#10-fc-san-fiber-channel-storage)
11. [Data Center Connectivity](#11-data-center-connectivity)
12. [Disaster Recovery Plan (DRP)](#12-disaster-recovery-plan-drp)
13. [SCOM (System Center Operations Manager)](#13-scom-system-center-operations-manager)
14. [Startup & Shutdown Procedures](#14-startup--shutdown-procedures)
15. [AI System Architecture](#15-ai-system-architecture)
16. [Fault-Tolerant Design Patterns](#16-fault-tolerant-design-patterns)
17. [Final Recall Checklist](#final-recall-checklist)

---

## 1. Storage Fundamentals

### Logical Disks

- Abstraction over physical disks
- Managed by OS / storage controller
- Can span multiple physical drives (via RAID, LVM)

**Key idea:** Logical != Physical -> improves flexibility, scaling, redundancy

---

### LVM (Logical Volume Management)

A method of allocating storage space that is **more flexible than conventional partitioning**. LVM sits as a layer in the OS device-driver stack (a form of storage virtualization).

#### Core Concepts

```
Physical Disks / Partitions
        |
  Physical Volumes (PVs)        ← each PV is divided into Physical Extents (PEs)
        |
  Volume Group (VG)              ← pool of PEs drawn from one or more PVs
        |
  Logical Volumes (LVs)         ← virtual partitions carved from the VG
        |
  File Systems / Swap            ← mounted and used like regular partitions
```

| Term | What It Is |
|------|------------|
| **Physical Volume (PV)** | A disk, partition, or LUN that LVM manages |
| **Physical Extent (PE)** | Fixed-size chunk of a PV (the smallest unit LVM works with) |
| **Logical Extent (LE)** | Maps 1:1 to a PE (or multiple PEs if mirrored) |
| **Volume Group (VG)** | A pool of PEs from one or more PVs — the storage "bucket" |
| **Logical Volume (LV)** | A virtual partition carved from a VG — what you actually mount and use |

#### Key Properties

- **Resizable online** — LVs can be grown (concatenate more LEs) or shrunk (return LEs to pool) without unmounting, if the file system supports it
- **Non-contiguous** — LEs in an LV do not need to be physically adjacent, so LVs can grow without moving existing data
- **Striped LVs** — each successive LE is allocated from a different PV, improving sequential read throughput (similar concept to RAID 0)
- **VG boundaries** — PVs and LVs cannot span across different VGs, but VGs can absorb new PVs or release existing ones
- **Hot-pluggable** — if hardware supports it, engineers can add/remove PVs from a VG without downtime

#### Why It Matters

- Traditional partitions are **fixed at creation** — resizing requires downtime and risk
- LVM decouples logical layout from physical disks — you can move, resize, and snapshot volumes on the fly
- Enables administrators to bring entire VGs online/offline or move them between hosts as a single unit

---

### RAID (Redundant Array of Independent Disks)

Originally "Redundant Array of **Inexpensive** Disks" — later redefined to "Independent" by the RAID Advisory Board (1992). RAID coordinates two or more storage devices into a **disk array** using techniques developed in the 1970s-80s: **data striping** (for performance) and **disk mirroring or parity** (for recovery).

**Core concept:** With the exception of RAID 1, all standard RAID levels use **storage virtualization** to abstract multiple physical devices into one logical volume.

#### Standard RAID Levels

| Level | Technique | Min Drives | Fault Tolerance | Key Characteristics |
|-------|-----------|------------|-----------------|---------------------|
| **RAID 0** | Block-level striping | 2 | None — any drive failure = total data loss | Read/write throughput multiplied by number of drives; capacity = sum of all drives |
| **RAID 1** | Mirroring | 2 | Survives all but last drive failure | Data written identically to all drives; read requests served by any drive; write speed limited by slowest drive |
| **RAID 2** | Bit-level striping + Hamming parity | 3+ | Historical only | Synchronized spindles; not used commercially since ~2014 |
| **RAID 3** | Byte-level striping + dedicated parity | 3+ | 1 drive | Synchronized spindles; not commonly used in practice |
| **RAID 4** | Block-level striping + dedicated parity | 3+ | 1 drive | Better I/O parallelism than RAID 2/3 (reads don't span all drives); replaced by RAID 5 in most cases |
| **RAID 5** | Block-level striping + distributed parity | 3+ | 1 drive | Parity spread across all drives (no bottleneck); vulnerable during rebuild — reading all data opens window for second failure |
| **RAID 6** | Block-level striping + double distributed parity | 4+ | 2 drives | ~3,800x lower failure chance than RAID 5; essential for large-capacity drives with long rebuild times |

#### Parity Explained

- Most RAID levels use **XOR** for parity calculation — simple, fast, recovers any single missing block
- RAID 6 uses **two separate parities** (based on Galois field arithmetic / Reed-Solomon codes) to handle two simultaneous failures

#### Nested (Hybrid) RAID

| Level | Structure | Key Property |
|-------|-----------|-------------|
| **RAID 0+1** | Stripe first, then mirror | If one drive fails, entire mirror is lost — runs as RAID 0 until rebuilt; higher rebuild risk |
| **RAID 1+0 (RAID 10)** | Mirror first, then stripe | Can survive multiple failures as long as no single mirror loses all drives; faster rebuild (only one drive to copy) |
| **JBOD + RAID N** | Concatenate smaller RAID sets | Reduces rebuild time for large arrays; allows mixing drive sizes over time; partial array failure does not destroy the whole set |

**RAID 10 vs RAID 0+1:** RAID 10 is preferred because it has a faster, safer rebuild — only one mirrored drive needs copying, not an entire stripe set.

#### RAID Implementations

| Type | How It Works | Examples |
|------|-------------|---------|
| **Hardware RAID** | Dedicated controller with own processor and cache; configured via BIOS/Option ROM; OS-independent | Adaptec, LSI MegaRAID, HP SmartArray |
| **Software RAID** | Managed by the OS kernel; no special hardware needed; flexible but uses host CPU | Linux md, OpenBSD softraid, Windows Storage Spaces |
| **Firmware/Hybrid ("Fake RAID")** | Standard controller chip + proprietary firmware; BIOS handles RAID during boot, OS drivers take over after | Intel Rapid Storage Technology (RST) |
| **File-system integrated** | RAID logic built into the file system itself | ZFS (RAID-Z1/Z2/Z3), Btrfs, Spectrum Scale |

**ZFS RAID-Z variants:**
- RAID-Z1 = equivalent to RAID 5 (single parity)
- RAID-Z2 = equivalent to RAID 6 (double parity)
- RAID-Z3 = triple parity (no standard RAID equivalent)
- ZFS uses **dynamic stripe size** and copy-on-write, which eliminates the write hole problem

#### RAID Weaknesses & Risks

**1. Correlated failures:**
- Drives in an array are often the same age, brand, and batch — failures are **statistically correlated**, not independent
- A study of ~100,000 drives found the probability of two drives in the same cluster failing within 1 hour was **4x higher** than predicted by independent failure models
- Recommendation: **mix drives from different batches and manufacturers**

**2. Unrecoverable Read Errors (URE) during rebuild:**
- Enterprise drives: URE rate ~1 bit in 10^15
- Desktop drives: URE rate ~1 bit in 10^14
- During RAID 5 rebuild, **all blocks on all remaining drives must be read** — with large drives, a URE on any remaining drive means rebuild failure and total data loss
- RAID 6 mitigates this (tolerates one URE during rebuild); RAID 10 has lowest risk (only one drive needs copying)

**3. Growing rebuild times:**
- Drive capacity grows faster than transfer speed — rebuild of a large drive can take **hours to days**
- During rebuild, the array runs with reduced redundancy — a second failure during this window is catastrophic
- RAID 5 is increasingly risky for large drives; RAID 6 or RAID 10 recommended

**4. Write hole:**
- A crash during a write can leave parity inconsistent with data — on next failure, parity cannot be used for recovery
- Solutions: write-ahead logging (hardware RAID uses onboard cache), write-intent bitmaps (Linux md), or copy-on-write (ZFS)

**5. Data integrity:**
- **Data scrubbing (patrol read)** — periodic background read of all blocks to detect and fix bad sectors before they matter
- RAID protects against drive failure but **not** against operator error, software bugs, viruses, or controller failure
- RAID is **not a backup** — always maintain separate backups

#### Recall

- Fault tolerance = ability to survive disk failure
- Tradeoff = **performance vs capacity vs safety**
- LVM = flexible volume management on top of physical disks
- RAID 5 is risky for large drives — prefer RAID 6 or RAID 10
- RAID ≠ backup — protects against hardware failure, not data corruption or human error
- Write hole = parity corruption after crash — solved by ZFS (COW) or write-ahead logging

[Back to Contents](#contents)

---

## 2. Fault Tolerance Concepts

### Fault Tolerance

- System continues working despite failures

### Techniques

- **Replication** - data copies
- **Failover** - switch to backup
- **Load balancing**
- **Health checks**

---

### Cluster Example: Server1 / Server2 + Witness

- **Server1 (Primary)**
- **Server2 (Secondary)**
- **Witness** (tie-breaker) - Prevents **split-brain problem**

#### Failover Logic

- If Server1 fails -> Server2 becomes active
- Witness decides which node is authoritative

[Back to Contents](#contents)

---

## 3. RTO & RPO (CRITICAL)

### Definitions

- **RTO (Recovery Time Objective)** -> Max acceptable downtime
- **RPO (Recovery Point Objective)** -> Max acceptable data loss (time window)

### Quick Formula Thinking

- **RTO ~ Time to restore system**
- **RPO ~ Backup frequency**

### Example

- Backups every 10 min -> RPO = 10 min
- System restored in 1 hour -> RTO = 1h

### Recall Trick

- **RTO** = "How fast back?"
- **RPO** = "How much lost?"

[Back to Contents](#contents)

---

## 4. Virtualization

### Concept

- Multiple VMs run on one physical machine via **hypervisor**

### Types

- **Type 1 (bare metal):** VMware ESXi, Hyper-V, KVM
- **Type 2:** VirtualBox, VMware Workstation

### Major Providers

| Provider | Use Case |
|----------|----------|
| VMware | Enterprise DC |
| Microsoft Hyper-V | Windows ecosystems |
| KVM | Open-source / cloud |
| AWS EC2 | Cloud VMs |
| GCP Compute Engine | Scalable workloads |
| Citrix XenServer | Server virtualization |

### Pros

- Isolation
- Strong security boundary
- OS flexibility

### Cons

- Heavy (each VM has OS)
- Higher cost
- Slower startup

[Back to Contents](#contents)

---

## 5. Containerization

### Concept

- Apps share host OS kernel
- Lightweight compared to VMs

### Tools

- Docker
- Kubernetes
- Docker Swarm

### Pros

- Fast startup
- Efficient resource usage
- Ideal for microservices

### Cons

- Weaker isolation vs VMs
- Kernel dependency

[Back to Contents](#contents)

---

## 6. Container vs VM Security

### Containers

**Risk:**
- Shared kernel -> escape vulnerability possible
- If attacker breaks container -> may access host

### VMs

**Strong isolation:**
- Separate OS
- Hypervisor boundary

### Conclusion

- Containers = efficiency
- VMs = stronger security
- **Best practice: containers inside VMs**

[Back to Contents](#contents)

---

## 7. HDFS (Hadoop Distributed File System)

### Components

- **NameNode** -> metadata (brain)
- **DataNode** -> stores data blocks
- **Client** -> reads/writes

### Key Features

- Data split into blocks
- Replicated (usually 3x)
- Fault tolerant

[Back to Contents](#contents)

---

## 8. Hadoop Ecosystem

### Core

- **HDFS** -> storage
- **MapReduce** -> processing

### Tools

- **Apache Hive** -> SQL on Hadoop
- **Hue** -> web UI for Hadoop
- **Spark** -> faster processing

### MapReduce

- **Map** -> process chunks
- **Reduce** -> aggregate results

[Back to Contents](#contents)

---

## 9. HA vs DR Regimes

### High Availability (HA)

- Goal: no downtime
- Local redundancy

### Disaster Recovery (DR)

- Goal: survive major disaster
- Cross-site recovery

### Modes

| Mode | Description |
|------|-------------|
| **Active/Active** | Both sites live |
| **Active/Passive** | Standby backup |
| **Warm standby** | Partially ready |
| **Cold standby** | Slow recovery |

[Back to Contents](#contents)

---

## 10. FC SAN (Fiber Channel Storage)

### Components

- HBA (host bus adapter)
- Switches
- Storage arrays

### Flow

```
Server -> HBA -> FC Switch -> Storage
```

[Back to Contents](#contents)

---

## 11. Data Center Connectivity

### Key Concepts

- Primary site + Backup site
- Dual fiber links
- Synchronous / Asynchronous replication

[Back to Contents](#contents)

---

## 12. Disaster Recovery Plan (DRP)

### What is it?

- Documented process for restoring systems

### Includes

- Roles & responsibilities
- Recovery steps
- Communication plan

### Types

| Type | Description |
|------|-------------|
| **Backup & Restore** | Simplest, slowest |
| **Pilot Light** | Minimal core running |
| **Warm Standby** | Scaled-down copy ready |
| **Active-Active** | Full redundancy |

### Backup Methods

| Method | Description | Use Case | Pros | Cons | Example / Tools |
|--------|-------------|----------|------|------|-----------------|
| **Full Backup** | Complete copy of all data and models | Rare, but required for full recovery | Simplicity of restoration | Large volume, long duration | — |
| **Incremental Backup** | Saves only changes since last backup | To save time and disk space | Saves disk space and time | Complex recovery | Backing up only new model weights or new data |
| **Differential Backup** | Saves all changes since last full backup | Simplifies recovery compared to incremental | Easier than incremental for recovery | More data than incremental | — |

### Standby & Failover Solutions

| Solution Type | Description | Use Case | Example |
|---------------|-------------|----------|---------|
| **Hot Standby** | Backup system actively syncs with primary | Minimizing RTO | Model containers running in another region but not serving traffic |
| **Warm Standby** | Backup system periodically updated | Balance between cost and recovery speed | — |
| **Cold Standby** | Backup system not running, only data stored | Rare scenarios with low RTO requirements | — |
| **Multi-Region / Multi-Cloud DR** | Load and backups distributed across multiple regions or clouds | Protection from failures in a single region | AI service in AWS EU and Azure US |
| **Failover & Failback** | Failover — switch to backup system; Failback — return to primary after recovery | For highly available services | — |

### Backup & DR for AI Systems

Backup and Disaster Recovery (DR) strategies for AI systems have unique considerations, as these systems often include:

- Machine learning models (ML/LLM)
- Large volumes of data (training data, inference data)
- High availability and integrity requirements
- Integration with other services (APIs, message brokers, databases)

| Component | What to Back Up | Recommendations |
|-----------|-----------------|-----------------|
| **ML Models** | Weights, architecture, metadata | Store encrypted with versioning |
| **Training Data** | Training sets, annotations, metadata | Use replication and multi-region storage |
| **Infrastructure** | Configurations, logs, settings | Use load balancing and response caching |
| **Services** | APIs, containers, brokers, databases | Store in dedicated systems (MLflow, DVC, W&B) |
| **Metrics & Logs** | Training history, performance metrics, errors | Run DR tests simulating failures and data loss |

[Back to Contents](#contents)

---

## 13. SCOM (System Center Operations Manager)

### What is SCOM?

A **cross-platform data center monitoring system** for operating systems and hypervisors. It provides a single interface showing **state, health, and performance** information across computer systems. It generates alerts based on availability, performance, configuration, or security conditions. Works with both **Windows Server** and **Unix-based** hosts.

### How It Works — The Agent Model

```
[Monitored Server]          [SCOM Server]            [Admin / Operator]
      |                          |                          |
  Agent installed           Central DB                 Views alerts
      |                     (alert history)                 |
  Watches sources:              |                    Gets notified via:
  - Windows Event Log      Applies filtering          - Email
  - App-specific logs      rules on arrival           - Pager
      |                         |                     - Support ticket
  Detects alert  ------>  Receives alert  ------>  Triggers workflow
                          from agent                (auto-remediation
                                                    or notification)
```

1. An **agent** is installed on each monitored computer
2. The agent watches sources (Windows Event Log, application-specific logs) for specific events
3. On alert detection, the agent **forwards the alert** to the central SCOM server
4. The SCOM server stores alerts in a **database** (history of all alerts)
5. **Filtering rules** are applied to incoming alerts — a rule can:
   - Send a notification (email, pager)
   - Generate a network support ticket
   - Trigger an automated workflow to fix the issue

### Core Components

| Component | Role |
|-----------|------|
| **Agent** | Installed on monitored machines; watches event logs and forwards alerts |
| **Management Server** | Central server that receives alerts, applies rules, stores history |
| **Database** | Stores alert history, configuration, and operational data |
| **Console** | UI for administrators and operators to view alerts and system health |
| **Gateway Server** | Enables monitoring across trust boundaries (e.g., DMZ networks) |

### Management Packs (MPs)

A **management pack** is a set of filtering rules specific to a monitored application. Think of it as a "plugin" that teaches SCOM how to monitor a particular product.

**What they define:**
- Which events to watch for
- What thresholds trigger alerts
- How to respond to specific conditions

**Sources of management packs:**
- **Microsoft** provides MPs for their products (SQL Server, SharePoint, etc.)
- **Third-party vendors** write MPs for their own products
- **Custom MPs** can be authored by end-users for anything not already covered

**Authoring tools:**
- System Center Operations Manager Resource Kit
- Visual Studio with Authoring Extensions
- Visio MP Designer

**Examples of what MPs can monitor:**

| MP Category | Examples |
|-------------|----------|
| Microsoft | SQL Server, SharePoint, Active Directory |
| Web servers | Apache, Tomcat, IIS |
| Virtualization | VMware, Hyper-V |
| Linux | SUSE Linux, Red Hat |

**Default behavior:** Without additional MPs, SCOM only monitors basic OS-related services. MPs extend its reach to application-level monitoring.

### Roles & Permissions

- **Administrator** — can install agents, configure monitored computers, create management packs
- **Operator / Viewer** — any valid user account can be granted read-only rights to view recent alerts

This separation allows broad visibility into system health without giving everyone admin-level access.

### Multi-Network Architecture

Multiple SCOM servers can be **aggregated together** to monitor across:
- Logical Windows domain boundaries
- Physical network boundaries

**Connection method (since SCOM 2007):**
- Direct **TCP connection** on **port 5723** (replaced the older web service approach)
- Allows connecting separately-managed groups to a central location

### The Command Shell

Since Operations Manager 2007, SCOM includes an extensible CLI called **The Command Shell**:
- A customized instance of **Windows PowerShell**
- Provides interactive and script-based access to SCOM data and operations
- Enables automation of monitoring tasks and alert management

### Azure Integration

- **Log Analytics Data Sources** can be enabled on Azure-hosted servers to collect and send monitoring data to on-premises SCOM Management Servers
- In November 2020, Microsoft announced plans to make SCOM a **fully cloud-managed instance** in Azure (codename **"Aquila"**)

### Recall Summary

- **Agent-based model** — lightweight software on each monitored machine reports to a central server
- **Management Packs** — modular rule sets that extend SCOM to monitor specific applications
- **Alert pipeline:** Event detected -> Agent forwards -> Server filters -> Action triggered
- **Cross-platform** — not just Windows; supports Unix/Linux hosts
- **Scalable** — multiple SCOM servers can aggregate across domain and network boundaries via TCP port 5723

[Back to Contents](#contents)

---

## 14. Startup & Shutdown Procedures

### Startup (general logic)

1. DB (critical data)
2. Core services
3. App servers
4. Gateways

### Startup Procedure (Detailed Example)

The system must be started in the following order:

1. **Backup operational database server (SQL2)**
   - msk-dbscomopdb3 (10.10.64.101)
2. **Operational database servers (SQL1)** — start first node first
   - msk-dbscomopdb2 (10.10.64.86)
   - msk-dbscomopdb1 (10.10.64.85)
3. **Reporting database server (DW-DB)**
   - msk-dbscomdw (10.10.64.89)
4. **Root management servers (RMS)** — start first node first
   - msk-isscomrms2 (10.10.64.81)
   - msk-isscomrms1 (10.10.64.80)
5. **Management servers (MS1, MS2, MS3)**
   - msk-isscomms1 (10.10.64.83)
   - msk-isscomms2 (10.10.64.84)
   - msk-isscomms3 (10.10.64.102)
6. **Gateway servers (DMZ-GW1, DMZ-GW2)**
   - mskz-isscomgw1 (10.10.96.9)
   - mskz-isscomgw2 (10.10.104.9)
7. **Reporting server (SRS1)**
   - msk-wsscomrs (10.10.64.90)

### Shutdown (reverse order)

1. Gateways
2. App servers
3. Core services
4. DB last

### Shutdown Procedure (Detailed Example)

The system must be shut down in the following order:

1. **Gateway servers (DMZ-GW1, DMZ-GW2)**
   - mskz-isscomgw1 (10.10.96.9)
   - mskz-isscomgw2 (10.10.104.9)
2. **Management servers (MS1, MS2, MS3)**
   - msk-isscomms1 (10.10.64.83)
   - msk-isscomms2 (10.10.64.84)
   - msk-isscomms3 (10.10.64.102)
3. **Reporting server (SRS1)**
   - msk-wsscomrs (10.10.64.90)
4. **Root management server (RMS)** — shut down first node first
   - msk-isscomrms1 (10.10.64.80)
   - msk-isscomrms2 (10.10.64.81)
5. **Reporting database server (DW-DB)**
   - msk-dbscomdw (10.10.64.89)
6. **Operational database server (SQL1)** — shut down first node first
   - msk-dbscomopdb1 (10.10.64.85)
   - msk-dbscomopdb2 (10.10.64.86)
7. **Backup operational database server (SQL2)**
   - msk-dbscomopdb3 (10.10.64.101)

**Key idea:** Startup goes from databases up to gateways; shutdown is the exact reverse. Prevents dependency failures.

### Why This Order Matters

The system follows a **dependency chain** — each layer depends on the one below it:

```
Gateways  -->  Management Servers  -->  Root Management  -->  Databases
 (top)            (middle)               (core)              (foundation)
```

**On startup**, you build from the foundation up:
- Databases come first because every other service reads/writes data from them. Starting a management server before its database is ready would cause connection errors and failed health checks.
- Root management servers (RMS) come next — they coordinate the cluster and need the database layer already available.
- Management servers depend on RMS for configuration and task assignment.
- Gateways are the external-facing entry points (DMZ) — they should only start accepting traffic once all backend services are healthy.
- The reporting server starts last because it aggregates data from all other components and is non-critical to core operations.

**On shutdown**, you tear down from the top:
- Gateways shut down first to stop incoming traffic, preventing new requests from hitting a system that is going offline.
- Management and reporting servers go next — they are consumers of data, not providers.
- RMS shuts down after the servers it manages are already stopped.
- Databases shut down last to ensure no active service is still trying to write data, which could cause corruption or data loss.

**Node ordering within a tier:** When a tier has multiple nodes (e.g., SQL1 has two nodes), the startup/shutdown notes specify which node goes first. This is typically because of **cluster quorum** — the node that holds the primary role or was the last to sync must be handled in a specific order to avoid split-brain or data inconsistency.

[Back to Contents](#contents)

---

## 15. AI System Architecture

### Overview

Designing the architecture of AI services requires balancing multiple pillars simultaneously. Each pillar addresses a specific concern, and together they form a resilient, maintainable, and efficient system. The examples below use a **RAG (Retrieval-Augmented Generation) service** as a running illustration.

### Key Pillars

#### 1. Scalability

**Goal:** Handle growing load by adding resources without redesigning the system.

**Principles:**
- **Horizontal scaling** — deploying multiple service instances (e.g., via Kubernetes, Docker Swarm) to distribute the load across nodes. When traffic spikes, new pods are spun up automatically.
- **Vertical scaling** — increasing CPU, RAM, or GPU on a single node. Simpler but has a ceiling — eventually one machine cannot grow further.
- **Auto-scaling** — configuring rules (e.g., Kubernetes HPA) to scale instances based on CPU usage, request queue depth, or custom metrics.
- **Component decomposition** — splitting the service into independent parts (e.g., retriever, generator, cache) so that each module can be scaled individually based on its own load profile.
- **Asynchronous processing** — using message queues (Kafka, RabbitMQ) to handle tasks without blocking the main thread, allowing the system to absorb traffic bursts gracefully.

**Example:** A RAG service deployed in Kubernetes can automatically scale retriever pods from 2 to 10 when query volume doubles during peak hours, while heavy inference tasks are offloaded to a Kafka queue so the API remains responsive.

#### 2. Fault Tolerance

**Goal:** Ensure continuous operation even when individual components fail.

**Principles:**
- **Replication** — copying data and components across nodes (e.g., database replicas, cache replicas) so no single point of failure exists.
- **Standby nodes** — launching backup service instances when primary ones fail. Can be hot (already running), warm (partially ready), or cold (started on demand).
- **Automatic recovery** — mechanisms for restarting failed services and redirecting traffic (e.g., Nginx health checks, Kubernetes liveness/readiness probes).
- **Error tolerance** — handling errors without stopping the entire system: retry logic, graceful degradation, structured logging of failures.

**Example:** If one instance of a RAG service goes down, Nginx automatically redirects requests to other healthy nodes — users experience no interruption.

#### 3. Performance

**Goal:** Optimize request processing time and resource utilization.

**Principles:**
- **Model optimization** — using quantization and compression (e.g., GGUF, ONNX formats) to reduce resource consumption while maintaining acceptable accuracy.
- **Caching** — storing frequent query results (e.g., in Redis, Memcached) to return answers instantly without re-running the model.
- **Asynchronous processing** — separating requests into background tasks (heavy inference) and synchronous tasks (lightweight queries), so the system does not block on long operations.
- **Infrastructure optimization** — using GPU/TPU for compute-heavy inference, distributing load across nodes to prevent bottlenecks.

**Example:** Caching RAG service responses in Redis allows instant responses for repeated queries, drastically reducing average latency.

#### 4. Modularity & Flexibility

**Goal:** Simplify updating, testing, and integrating components independently.

**Principles:**
- **Microservice decomposition** — splitting the system into independent modules (e.g., retriever, generator, API gateway) that can be developed and deployed separately.
- **API-oriented architecture** — using REST or gRPC for communication between components, providing clear contracts and loose coupling.
- **External service integration** — supporting connections to databases, analytics tools, vector stores, and third-party APIs.
- **Multi-format data support** — handling text, images, audio, and video within the same architecture.

**Example:** A RAG service can be split into separate microservices for the retriever and generator — each can be updated, scaled, or replaced independently without affecting the other.

#### 5. Security & Data Protection

**Goal:** Protect data and prevent leaks, attacks, and unauthorized access.

**Principles:**
- **Data encryption** — protecting data in transit (TLS) and at rest (AES). All communication between services should be encrypted.
- **Access control** — using IAM roles and tokens (JWT) to restrict who can call which endpoints and access which data.
- **Logging & audit** — tracking all operations to detect anomalies, investigate incidents, and maintain compliance.
- **Vulnerability management** — regular security scans (e.g., OWASP ZAP) to identify and patch weaknesses before they are exploited.

**Example:** All requests to a RAG service must pass through authentication and encrypted channels to prevent unauthorized access to sensitive knowledge bases.

#### 6. Feedback & Learning

**Goal:** Continuously improve the model and service based on real-world data and user behavior.

**Principles:**
- **Feedback collection** — using metrics (accuracy, response time, user ratings/likes) to measure model effectiveness and identify weak spots.
- **Model updates** — regularly retraining the model on new data, validating improvements through A/B testing before full rollout.
- **Log analysis** — analyzing query patterns and error logs to find systematic problems (e.g., a category of queries the retriever consistently fails on).

**Example:** Analyzing errors in a RAG service reveals that the retriever misses certain document types — this insight drives a targeted retraining cycle that reduces incorrect answers.

#### 7. Infrastructure & Tools

**Goal:** Provide the operational foundation for scalability, reliability, and observability.

**Principles:**
- **Cloud platforms** — using AWS, Azure, or GCP for automated provisioning, scaling, and managed services (e.g., managed databases, GPU instances).
- **Containerization** — deploying services in Docker containers for consistent environments across development, staging, and production.
- **Orchestration** — using Kubernetes to manage clusters, handle auto-scaling, rolling updates, and self-healing of failed pods.
- **Monitoring & logging** — integrating with Prometheus (metrics), Grafana (dashboards), and ELK stack (log aggregation) to track system health in real time.

**Example:** A RAG service deployed in Kubernetes automatically scales resources based on load, while Prometheus + Grafana dashboards alert the team if latency exceeds SLA thresholds.

### Pillar Summary

| Pillar | Goal | Key Tools / Techniques |
|--------|------|------------------------|
| **Scalability** | Handle growing load | Kubernetes, HPA, Docker Swarm |
| **Fault Tolerance** | Survive component failures | Replication, Nginx failover, K8s probes |
| **Performance** | Fast response times | Redis caching, ONNX/GGUF, GPU, async |
| **Modularity** | Independent components | Microservices, REST/gRPC, API gateway |
| **Security** | Protect data & access | TLS, AES, IAM, JWT, OWASP ZAP |
| **Feedback** | Continuous improvement | A/B testing, metrics, error log analysis |
| **Infrastructure** | Operational foundation | AWS/Azure/GCP, Docker, K8s, Prometheus |

[Back to Contents](#contents)

---

## 16. Fault-Tolerant Design Patterns

Fault tolerance is the ability of a system to continue operating correctly when failures occur. The following architectural patterns help **prevent**, **detect**, and **recover** from failures.

---

### Retry (Retry with Backoff)

**Description:** When an operation fails, the system automatically retries it.

**Use case:** Temporary failures such as network timeouts, transient database errors, or brief service unavailability.

**Recommendations:**
- Use **exponential backoff** — wait 1s, 2s, 4s, 8s... between retries instead of hammering the failing service
- **Limit the number of attempts** (e.g., max 3-5 retries) to avoid infinite loops
- Ensure the operation is **idempotent** — retrying it multiple times must produce the same result

---

### Circuit Breaker

**Description:** Monitors failure frequency and **temporarily blocks calls** to a service if failures exceed a threshold. Prevents cascading failures across dependent services.

**Use case:** A downstream service is degraded or unresponsive — without a circuit breaker, every caller would hang waiting for timeouts.

**Three states:**

```
  [CLOSED] ---failures exceed threshold---> [OPEN]
     ^                                        |
     |                              timeout expires
     |                                        v
     +------success on probe---------- [HALF-OPEN]
```

- **Closed** — operating normally, requests pass through
- **Open** — circuit tripped, all requests immediately return an error (no actual call made)
- **Half-Open** — after a cooldown period, a few probe requests are allowed through to test if the service has recovered

---

### Fallback (Fail-Safe Path)

**Description:** When an operation fails, the system uses an **alternative behavior** instead of returning an error.

**Use case:** Returning cached data, default values, or simplified logic when the primary path is unavailable.

**Recommendations:**
- If a service is unavailable, return a **static/cached response** rather than an error
- If an external API is down, use a **local copy of the data**
- Clearly indicate to the caller that a fallback response was served (e.g., via headers or metadata)

---

### Bulkhead (Compartmentalization)

**Description:** Limits the resources (threads, connections, memory) allocated to a specific operation, so that a failure in one part **does not affect others**. Named after ship bulkheads that prevent a single hull breach from sinking the entire vessel.

**Use case:** Isolating components from overload — ensuring that a slow external API does not consume all threads and starve other parts of the system.

**Recommendations:**
- Allocate a **limited thread/connection pool** for each external API call
- **Separate resource pools** between critical and non-critical tasks (e.g., payment processing gets its own pool, separate from analytics)
- Combine with circuit breakers for defense in depth

---

### Idempotency

**Description:** An operation can be executed **one or many times** and the result will be the same. This is a property, not a runtime mechanism — but designing for it is critical to fault tolerance.

**Use case:** Simplifies retry logic and handling of duplicate requests — safe to retry without side effects.

**Recommendations:**
- HTTP methods **GET**, **PUT**, and **DELETE** should always be idempotent by design
- Use **idempotency keys** for transactions (e.g., a unique request ID sent by the client — if the server sees the same key again, it returns the cached result instead of processing twice)
- POST operations that create resources should check for duplicates before inserting

---

### Monitoring & Health Checks

**Description:** The system continuously monitors its own state and the state of its dependencies.

**Use case:** Automatic detection and isolation of unhealthy components; collecting metrics and logs for diagnostics.

**Recommendations:**
- Implement **liveness probes** (is the process alive?) and **readiness probes** (can it handle traffic?) — Kubernetes uses both
- Monitor key metrics: latency, error rate, throughput, resource utilization
- Set up **alerting thresholds** so problems are detected before users notice

---

### Rate Limiting

**Description:** Limits the number of requests processed per unit of time.

**Use case:** Prevents system overload and protects against DDoS attacks or misbehaving clients.

**Algorithms:**
- **Token bucket** — tokens refill at a fixed rate; each request consumes a token; no tokens = rejected
- **Sliding window** — counts requests in a rolling time window (e.g., max 100 requests per minute per client)

---

### Load Balancing

**Description:** Distributes incoming traffic across multiple service instances.

**Use case:** Increases both fault tolerance (if one instance dies, others handle traffic) and performance (parallel processing).

**Strategies:**
- **Round Robin** — requests go to each instance in turn (simplest)
- **Least Connections** — sends to the instance with fewest active connections (best for uneven request durations)
- **IP Hash** — same client IP always goes to the same instance (useful for session affinity)

---

### Redundancy

**Description:** Creating duplicate components (services, hosts, databases) so that if one fails, another takes over.

**Types:**
- **Hot standby** — backup is running and synced in real time, ready to serve instantly
- **Active-Passive** — primary handles traffic, passive takes over on failure
- **Active-Active** — all instances serve traffic simultaneously, providing both redundancy and load distribution

---

### Consensus Algorithms

**Description:** Allow a group of distributed nodes to **agree on a value** even when some nodes fail or messages are lost.

**Use case:** Maintaining consistent state in distributed systems (e.g., databases, configuration stores, leader election). Managing quorum — the minimum number of nodes that must agree for a decision to be valid.

**Common algorithms:**
- **Paxos** — foundational consensus protocol, mathematically proven but complex to implement
- **Raft** — designed as a more understandable alternative to Paxos; used in etcd, Consul
- **ZAB (ZooKeeper Atomic Broadcast)** — used by Apache ZooKeeper for distributed coordination

---

### Graceful Degradation

**Description:** When a failure occurs, the system switches to a **simplified mode** rather than crashing entirely.

**Use case:** Non-essential features are disabled while core functionality remains available.

**Recommendations:**
- Disable non-critical features (e.g., recommendation engine, analytics) while keeping core operations running
- Serve a simplified interface (e.g., a website without JavaScript falls back to basic HTML)
- Communicate clearly to users that the system is in degraded mode

---

### Caching

**Description:** Storing frequently requested data to reduce load on backend services and improve fault tolerance.

**Use case:** If a backend service goes down, cached data can still be served — the system survives longer on stale-but-available data.

**Recommendations:**
- Cache responses from external services (so outages do not immediately cascade)
- Cache database query results to reduce load and latency
- Define appropriate **TTL (time-to-live)** values — balance freshness vs. resilience

---

### Pattern Summary

| Pattern | What It Does | Key Technique |
|---------|-------------|---------------|
| **Retry** | Re-attempts failed operations | Exponential backoff, idempotency |
| **Circuit Breaker** | Blocks calls to failing services | Closed / Open / Half-Open states |
| **Fallback** | Uses alternative when primary fails | Cached data, default values |
| **Bulkhead** | Isolates failures to subsystems | Separate thread/connection pools |
| **Idempotency** | Safe to retry without side effects | Idempotency keys, deduplication |
| **Health Checks** | Detects unhealthy components | Liveness/readiness probes, metrics |
| **Rate Limiting** | Prevents overload | Token bucket, sliding window |
| **Load Balancing** | Distributes traffic evenly | Round Robin, Least Connections, IP Hash |
| **Redundancy** | Duplicate components for failover | Hot/Active-Passive/Active-Active |
| **Consensus** | Agreement across distributed nodes | Paxos, Raft, ZAB |
| **Graceful Degradation** | Reduces functionality instead of crashing | Feature toggles, simplified mode |
| **Caching** | Serves stored data to reduce load | TTL-based cache, response caching |

[Back to Contents](#contents)

---

## Final Recall Checklist

Try to recall from memory:

- [ ] Difference: RAID 5 vs RAID 10
- [ ] RTO vs RPO (define + example)
- [ ] Hypervisor vs Container engine
- [ ] Why containers are less secure
- [ ] NameNode vs DataNode
- [ ] Active/Active vs Active/Passive
- [ ] DRP steps order
- [ ] MapReduce phases

[Back to Contents](#contents)
