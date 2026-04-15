# General Active Recall Notes

<a id="contents"></a>

## Contents

- [Strategy, Product & Enterprise Architecture](#strategy-product-enterprise-architecture)
  - [TCO (Total Cost of Ownership)](#tco-total-cost-of-ownership)
  - [Work-Breakdown Structure (WBS)](#work-breakdown-structure-wbs)
  - [Statement of Work (SOW)](#statement-of-work-sow)
  - [Service-Level Agreement (SLA)](#service-level-agreement-sla)
  - [Product Requirements Document (PRD)](#product-requirements-document-prd)
    - [Components (from Wikipedia):](#components-from-wikipedia)
    - [Explanation of PRD from ChatGPT (February 2026)](#explanation-of-prd-from-chatgpt-february-2026)
  - [Average Order Value (AOV)](#average-order-value-aov)
  - [Conversion Rate (CR)](#conversion-rate-cr)
  - [Gross Merchandise Value (GMV)](#gross-merchandise-value-gmv)
  - [Product Detail Page (PDP)](#product-detail-page-pdp)
  - [4+1 architectural view model](#4-1-architectural-view-model)
  - [The Open Group Architecture Framework (TOGAF)](#the-open-group-architecture-framework-togaf)
  - [Enterprise Architecture (EA)](#enterprise-architecture-ea)
  - [Procurement Life Cycle (Procurement Process)](#procurement-life-cycle-procurement-process)
    - [Procurement Lifecycle — AI/Software Architect View (Customer-Side/Buying Organization Perspective)](#procurement-lifecycle-ai-software-architect-view-customer-side-buying-organization-perspective)
    - [Procurement Lifecycle — AI/Software Architect View (Vendor/Solution Provider Perspective)](#procurement-lifecycle-ai-software-architect-view-vendor-solution-provider-perspective)
- [Reliability, Quality & Delivery](#reliability-quality-delivery)
  - [SLO (Service Level Objective) & SLI (Service Level Indicator)](#slo-service-level-objective-sli-service-level-indicator)
  - [Probabilistic SLA (Service Level Agreement)](#probabilistic-sla-service-level-agreement)
  - [CI/CD Pipelines](#ci-cd-pipelines)
  - [User Acceptance Testing (Software)](#user-acceptance-testing-software)
  - [Pair Programming](#pair-programming)
  - [Unit Tests & Edge Cases](#unit-tests-edge-cases)
  - [pytest-archon](#pytest-archon)
- [Web, APIs & Integration](#web-apis-integration)
  - [Server-Sent Events (SSE)](#server-sent-events-sse)
  - [WebSocket, HTTP, URI, URL, URN](#websocket-http-uri-url-urn)
  - [TCP/UDP Ports](#tcp-udp-ports)
    - [How many ports are there?](#how-many-ports-are-there)
    - [TCP vs. UDP Ports](#tcp-vs-udp-ports)
  - [Round-Trip Time (RTT)](#round-trip-time-rtt)
  - [Unix Domain Socket (UDS)](#unix-domain-socket-uds)
  - [Server-Side Rendering (SSR)](#server-side-rendering-ssr)
  - [gRPC](#grpc)
  - [gRPC vs. WebSocket vs. HTTP vs. Server-Sent Events (SSE)](#grpc-vs-websocket-vs-http-vs-server-sent-events-sse)
  - [Webhooks](#webhooks)
  - [OpenAPI Schemas: API data models / DTO (Data Transfer Objects)](#openapi-schemas-api-data-models-dto-data-transfer-objects)
  - [Green-API](#green-api)
  - [Real-Time Streaming Protocol](#real-time-streaming-protocol)
  - [Simple Mail Transfer Protocol](#simple-mail-transfer-protocol)
  - [Backend for Frontend Pattern (BFF)](#backend-for-frontend-pattern-bff)
- [Cloud, Containers & Platform Infrastructure](#cloud-containers-platform-infrastructure)
  - [K8s (Kubernetes)](#k8s-kubernetes)
    - [Why Kubernetes uses pods (not just containers)](#why-kubernetes-uses-pods-not-just-containers)
    - [Why architects usually avoid drawing nodes](#why-architects-usually-avoid-drawing-nodes)
    - [“Cluster” in deployment diagrams doesn’t always mean Kubernetes cluster](#cluster-in-deployment-diagrams-doesnt-always-mean-kubernetes-cluster)
    - [EKS (Amazon Elastic Kubernetes Service)](#eks-amazon-elastic-kubernetes-service)
    - [Helm in Kubernetes](#helm-in-kubernetes)
    - [YAML](#yaml)
    - [Kubernetes Cluster Installation & Bootstrapping Tools](#kubernetes-cluster-installation-bootstrapping-tools)
    - [Kubeadm, Kubespray, kubeone and Rancher (RKE2)](#kubeadm-kubespray-kubeone-and-rancher-rke2)
      - [Kubeadm vs. Kubespray](#kubeadm-vs-kubespray)
      - [Kubeadm vs. Kubespray vs. kubeone vs. RKE2](#kubeadm-vs-kubespray-vs-kubeone-vs-rke2)
    - [CNI (Container Network Interface)](#cni-container-network-interface)
    - [ClusterIP, NodePort & LoadBalancer in K8s](#clusterip-nodeport-loadbalancer-in-k8s)
    - [Kubernetes UI/Dashboard & Core YAML Files](#kubernetes-ui-dashboard-core-yaml-files)
  - [Docker Swarm](#docker-swarm)
  - [Serverless and FaaS](#serverless-and-faas)
  - [Virtual Private Cloud (VPC)](#virtual-private-cloud-vpc)
  - [Amazon Route 53](#amazon-route-53)
  - [Virtual Private Server (VPS)](#virtual-private-server-vps)
  - [Fibre Channel (FC) Storage Area Network (SAN)](#fibre-channel-fc-storage-area-network-san)
  - [Sidecar Pattern](#sidecar-pattern)
  - [Content Delivery Network (CDN)](#content-delivery-network-cdn)
    - [Point of Presence (PoP)](#point-of-presence-pop)
    - [CDN Edge Server](#cdn-edge-server)
  - [Host 0.0.0.0](#host-0-0-0-0)
  - [Makefile](#makefile)
- [Data, AI & Knowledge Systems](#data-ai-knowledge-systems)
  - [RAGAS score](#ragas-score)
  - [Avro vs. Parquet](#avro-vs-parquet)
  - [KAG: Knowledge Augmented Generation](#kag-knowledge-augmented-generation)
  - [GPTCache: A Library for Creating Semantic Cache for LLM Queries](#gptcache-a-library-for-creating-semantic-cache-for-llm-queries)
  - [Neo4j](#neo4j)
  - [ACID (Atomicity, Consistency, Isolation, Durability)](#acid-atomicity-consistency-isolation-durability)
  - [Master Data Management (MDM)](#master-data-management-mdm)
  - [Time-to-Live (TTL)](#time-to-live-ttl)
- [Observability & Distributed Operations](#observability-distributed-operations)
  - [Grafana Widgets](#grafana-widgets)
  - [Grafana Spans](#grafana-spans)
  - [Prometheus & Grafana](#prometheus-grafana)
  - [Event Bus](#event-bus)
  - [Messaging Systems. Message Queue (MQ), Event Bus & Message Broker](#messaging-systems-message-queue-mq-event-bus-message-broker)
- [Security, Identity & Compliance](#security-identity-compliance)
  - [VPN Server](#vpn-server)
  - [Payment Card Industry Data Security Standard (PCI DSS)](#payment-card-industry-data-security-standard-pci-dss)
  - [Write once read many (WORM)](#write-once-read-many-worm)
  - [Role-Based Access Control (RBAC)](#role-based-access-control-rbac)
  - [Security Information and Event Management (SIEM)](#security-information-and-event-management-siem)
  - [General Data Protection Regulation (GDPR)](#general-data-protection-regulation-gdpr)
  - [Self-Assessment Questionnaires (SAQs)](#self-assessment-questionnaires-saqs)
  - [TLS 1.2+ (Data in Transit) & AES-256 (Data at Rest)](#tls-1-2-data-in-transit-aes-256-data-at-rest)
  - [Identity Provider (IdP)](#identity-provider-idp)
  - [OAuth](#oauth)
  - [SSO & MFA](#sso-mfa)
  - [Security Information and Event Management (SIEM)](#security-information-and-event-management-siem-2)
  - [Mutual TLS (mTLS)](#mutual-tls-mtls)
  - [DLP (Data Loss Prevention) Filter](#dlp-data-loss-prevention-filter)
  - [Short-Lived Leases in Vault](#short-lived-leases-in-vault)
  - [JSON Web Token (JWT)](#json-web-token-jwt)
  - [Static Application Security Testing (SAST)](#static-application-security-testing-sast)

---

<a id="strategy-product-enterprise-architecture"></a>

## Strategy, Product & Enterprise Architecture

<a id="tco-total-cost-of-ownership"></a>

### TCO (Total Cost of Ownership)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-001.png)

**TCO, or Total Cost of Ownership,** is a comprehensive financial metric that calculates all direct and indirect costs of owning and operating an asset or system over its entire lifecycle, going far beyond the initial purchase price to include expenses like maintenance, training, support, energy, and eventual disposal, helping businesses make smarter investment decisions by revealing hidden costs and true long-term value. It's used for IT, vehicles, buildings, and more, with related concepts like TCO Certified focusing on sustainable IT product procurement.

**Key Components of TCO**

**Direct Costs**: Initial purchase, installation, setup, licensing, customization.

**Indirect Costs (Hidden Costs)**: Training, maintenance, repairs, support, downtime, energy, upgrades, security, disposal, time spent managing.

**Lifecycle**: Covers the entire period from acquisition to disposal.

<u>**Why TCO is Important**</u>

**Informed Decisions**: Moves beyond sticker price to show the true financial impact, preventing costly surprises.

**Better ROI**: Enables more realistic return on investment (ROI) analysis.

**Resource Optimization**: Helps allocate resources efficiently and reduce waste.

**Supplier Comparison**: Allows for better comparison between different vendors or solutions (e.g., used vs. new car).

**Examples of TCO in Action**

**IT**: Comparing an on-premise server (high upfront, ongoing maintenance) vs. cloud storage (subscription, potentially lower hidden costs).

**Automotive**: A cheap used car might cost more long-term due to frequent repairs compared to a new car with a warranty.

**Facilities**: Calculating costs for a building's entire life, not just construction.

**Related Certification**: TCO Certified

A global standard for sustainable IT products focusing on environmental and social criteria like climate, substances, circularity, and supply chain responsibility.

[Back to Contents](#contents)

---

<a id="work-breakdown-structure-wbs"></a>

### Work-Breakdown Structure (WBS)

**A work-breakdown structure (WBS) in project management and systems engineering** is a breakdown of a project into smaller components. It is a key project management element that organizes the team's work into manageable sections. The Project Management Body of Knowledge defines the work-breakdown structure as a "hierarchical decomposition of the total scope of work to be carried out by the project team to accomplish the project objectives and create the required deliverables."

A WBS provides the necessary framework for detailed cost estimation and control while providing guidance for schedule development and control.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-002.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-003.png)

[Back to Contents](#contents)

---

<a id="statement-of-work-sow"></a>

### Statement of Work (SOW)

**A statement of work (SOW)** is a document routinely employed in the field of project management. It is the narrative description of a project's work requirement. It defines project-specific activities, deliverables and timelines for a vendor providing services to the client. The SOW typically also includes detailed requirements and pricing, with standard regulatory and governance terms and conditions. It is often an important accompaniment to a master service agreement or request for proposal (RFP).

[Back to Contents](#contents)

---

<a id="service-level-agreement-sla"></a>

### Service-Level Agreement (SLA)

**A service-level agreement (SLA)** is an agreement, typically a binding contract, between a service provider and a customer that define particular aspects of the service – such as quality of service, availability, responsibilities – as terms of contract. SLAs are commonly used in the technology industry, and often contain specific service-level objectives for defined service level indicators, such as mean time between failures, mean time to repair, mean time to recovery, network throughput, jitter, uptime, first-call resolution, and turnaround time.

Unlike traditional contracts, SLAs often require parties involved to meet regularly to create an open forum for communication, apply rewards and penalties for performance, and leave room for periodic revisitation to make changes. SLAs may be supported by operational-level agreements, or OLAs.

[Back to Contents](#contents)

---

<a id="product-requirements-document-prd"></a>

### Product Requirements Document (PRD)

https://en.wikipedia.org/wiki/Product\_requirements\_document

**A product requirements document (PRD)** is a document containing all the requirements for a certain product. It is written to allow people to understand what a product should do. A PRD should, however, generally avoid anticipating or defining how the product will do it in order to later allow interface designers and engineers to use their expertise to provide the optimal solution to the requirements.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-004.jpeg)

*A well written product requirements document can get the entire team on the same page.*

PRDs are most frequently written for software products, but they can be used for any type of product and also for services. Typically, a PRD is created from a user's point-of-view by a user/client or a company's marketing department (in the latter case it may also be called a Marketing Requirements Document (MRD)). The requirements are then analyzed by a (potential) maker/supplier from a more technical point of view, broken down and detailed in a Functional Specification (sometimes also called Technical Requirements Document).

The form of the PRD will vary from project to project and depends, for example, on the approach to project implementation. The two most common approaches in software development are the cascading model and agile development methodology. In a cascading development model, product requirements are defined at the very beginning of the project, in their entirety, and development does not begin until they are ready. In the case of an agile development model, requirements are formulated initially at a higher level to allow for prioritization and then elaborated in detail at the beginning of each new cycle.

PRDs also help prevent critical technical issues in software development, including architecture mismatch with product requirements, overlooked technical dependencies, and underestimated implementation complexity.

<a id="components-from-wikipedia"></a>

#### Components (from Wikipedia):

**Typical components of a product requirements document (PRD) are**: \[citation needed\]

- **Title & author information**
- **Purpose and scope, from both a technical and business perspective**
- **Stakeholder identification**
- **Market assessment and target demographics**
- **Product overview and use cases**
- **Requirements, including**
  - **functional requirements (e.g. what a product should do)**
  - **usability requirements**
  - **technical requirements (e.g. security, network, platform, integration, client)**
  - **environmental requirements**
  - **support requirements**
  - **interaction requirements (e.g. how the product should work with other systems)**
- **Assumptions**
- **Constraints**
- **Dependencies**

High level workflow plans, timelines and milestones (more detail is defined through a project plan)

- **Evaluation plan and performance metrics**

Not all PRDs have all of these components. In particular, PRDs for other types of products (manufactured goods, etc.) will eliminate the software-specific elements from the list above, and may add in additional elements that pertain to their domain, e.g. manufacturing requirements.

<a id="explanation-of-prd-from-chatgpt-february-2026"></a>

#### Explanation of PRD from ChatGPT (February 2026)

**What is a Product Requirements Document (PRD)?**

- A Product Requirements Document (PRD) is the single source of truth that explains what you’re building, why you’re building it, and what success looks like—without prescribing detailed implementation.
- *Think of it as the contract between product, engineering, design, and business.*

For an AI-Software Architect, the PRD is especially important because it defines the problem space and constraints before you decide:

- **models vs. rules**
- **pipelines vs agents**
- **offline vs real-time**
- **accuracy vs latency vs cost**

**What a PRD Is Not**

❌ **Not a design doc**

❌ **Not an architecture doc**

❌ **Not a technical spec**

❌ **Not a backlog or ticket dump**

*Those come after the PRD.*

**Core Sections of a Standard PRD**

**1. Problem Statement**

**What user problem are we solving and why does it matter now?**

**Good**:

```text
Customer support agents spend 30% of their time searching internal docs for answers, leading to slow response times and inconsistent answers.
```

**Bad**:

```text
Build an AI chatbot for support.
```

**2. Goals & Non-Goals**

*What success looks like—and what’s explicitly out of scope.*

**Example**:

- **Goals**
  Reduce average response time by 40%
  Improve first-contact resolution by 20%
- **Non-Goals**
  Fully autonomous agent (human-in-the-loop required)
  Replacing existing CRM system

👉 **This protects the AI architect from scope creep.**

**3. User Personas & Use Cases**

**Who uses this and how?**

**Example personas**:

- **Tier-1 support agent**
- **Support team lead**
- **QA auditor**

**Use cases should be concrete**:

```text
As a Tier-1 agent, I can ask a natural-language question and receive a citation-backed answer in under 5 seconds.
```

**4. Functional Requirements**

*What the system must do (behavior-level).*

**Examples**:

- **Accept natural-language queries**
- **Retrieve answers from internal documentation**
- **Provide citations for each answer**
- **Escalate to human review when confidence is low**

<u>**No mention of**</u>:

- **LLM brand**
- **vector DB**
- **embeddings**
- **cloud provider**

*That’s your (AI Architect) domain later.*

**5. Non-Functional Requirements (Critical for AI)**

<u>**This section is 🔥 for AI-Software Architects.**</u>

**Typical categories**:

- **Latency**: ≤ 5s P95
- **Accuracy / Quality**: ≥ 85% helpfulness score
- **Reliability**: 99.9% uptime
- **Scalability**: 10k concurrent users
- **Security & Privacy**: PII redaction, SOC2
- **Cost constraints**: ≤ $0.02 per query

*These constraints drive your architecture choices.*

**6. Metrics & Success Criteria**

**How do we know this works?**

**For AI products**:

- **Task success rate**
- **Hallucination rate**
- **User satisfaction score**
- **Deflection rate**
- **Cost per successful task**

<u>**If it’s not measurable, it’s not a requirement.**</u>

**7. Constraints & Assumptions**

**Examples**:

- **Must use existing company knowledge base**
- **No training on customer data**
- **Must support EU data residency**
- **Legal approval required for model updates**

<u>**Architects should read this section twice.**</u>

**8. Dependencies & Risks**

**Examples**:

- **Dependency on documentation quality**
- **Risk of model drift**
- **Risk of prompt injection**
- **Risk of regulatory changes**

<u>**This is where AI risks should be made explicit—even if mitigation comes later.**</u>

**PRD Through the Lens of an AI-Software Architect**

**When you read a PRD, you’re asking**:

- ❓ **Is this prediction, generation, retrieval, decision-making, or control?**
- ❓ **Real-time or async?**
- ❓ **Deterministic or probabilistic?**
- ❓ **Human-in-the-loop required?**
- ❓ **What happens when the model is wrong?**

**Your output after the PRD is usually**:

- **System Architecture Doc**
- **Model selection rationale**
- **Data pipeline design**
- **Evaluation framework**
- **MLOps / monitoring plan**

**AI-Specific Additions You Often Add to PRDs**

**In practice, many AI teams extend PRDs with**:

- **AI Constraints**
  Explainability requirements
  Allowed error rates
  Guardrail requirements
- **Data Requirements**
  Data sources
  Update frequency
  Labeling strategy (if any)
- **Ethics & Safety**
  Bias considerations
  User transparency
  Appeal/escalation paths

[Back to Contents](#contents)

---

<a id="average-order-value-aov"></a>

### Average Order Value (AOV)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-005.png)

Average Order Value (AOV) representing the average total of each order placed by customers over a specific period of time.

The importance of AOV metric is it provides a glimpse into customer behavior indicating how much they’re prepared to spend each time they shop from your online store.

Using the ecommerce AOV metric is useful in measuring and evaluating your pricing strategy, promotional offers, and product selection. Moreover, monitoring AOV enables insight into whether customers buy just one product or add multiple items to their cart every time they make a purchase. AOV can also reveal whether they’re purchasing high-value items or inexpensive products.

To calculate AOV, you simply divide your total revenue by the number of orders. For example, if your total revenue is $20,000 and you’ve had 400 orders, your AOV is $50.

**What does high/low AOV in e-commerce means?**

**Higher AOV**

- Having a high Average Order Value (AOV) means that, on average, customers are spending a significant amount of money each time they place an order with your online store. This could indicate that they are purchasing multiple items or buying high-cost products.
- A high AOV is generally seen as favorable as it increases revenues.

**Lower AOV**

- On the other hand, a low AOV ecommerce metric suggests that customers are spending less money per transaction. This could mean they are only purchasing one or two low-cost items at a time.
- A low AOV ecommerce isn't necessarily bad, but it might signal opportunities to up-sell or cross-sell to increase the total purchase amount of each order, and hence, increase the overall revenue.

[Back to Contents](#contents)

---

<a id="conversion-rate-cr"></a>

### Conversion Rate (CR)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-006.png)

The Conversion Rate (CR) stands as one of the vital heartbeat metrics to ecommerce business.

Conversion Rate signifies how successful your site is at funneling visitors to complete a conversion, usually a purchase. It’s a window into the effectiveness of your marketing initiatives, offering insights on customer behavior and your website’s usability.

Simply put, if you have a high site traffic volume but low sales figures, your conversion rate is low. A high conversion rate, conversely, indicates your ecommerce strategy is on the right track - your site design is user-friendly, your product offerings and prices are attractive, and your users find what they’re looking for on your site.

Calculating your conversion rate is reasonably straightforward. It’s the total number of conversions divided by your site’s total visitors within a set timeframe, multiplied by 100%. For example, if your site had 15,000 visitors in a month and made 1,500 sales, your conversion rate for that month would be (1500/15000) x 100% = 10%.

Remember, optimizing conversion rates shouldn’t be a one-time project, but an ongoing strategy. Keep testing and tweaking various site elements to continually improve the user experience and bolster sales.

**What is a good conversion rate?**

A "good" ecommerce Conversion Rate (CR) can vary greatly depending on the industry, region, and the type of goods being sold. However, across industries, a general benchmark is that a good ecommerce CR is typically around 2.5% to 3%.

This means that 2 to 3 out of every 100 visitors to your online store are making a purchase, keep in mind that even a small improvement in CR can significantly boost your revenue!

[Back to Contents](#contents)

---

<a id="gross-merchandise-value-gmv"></a>

### Gross Merchandise Value (GMV)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-007.png)

Gross Merchandise Value (GMV), also known as Gross Merchandise Volume, gives insights into the overall volume of sales over a specific period. Technically, GMV refers to the total value of goods sold through an ecommerce platform before any deductions for fees, commissions, returns, or cancellations.

This metric helps you understand the raw sales volume, giving an immediate understanding of the business’s scope and size. Therefore, tracking GMV can help you gauge the growth rate of your business and determine if you’re reaching your sales targets.

It’s worth mentioning that while GMV can show impressive sales volumes, it’s NOT indicative of profit. Nevertheless, a steady increase in GMV signals a positive wave in attracting and selling to customers.

Calculating GMV can be quite straightforward; you multiply the price at which each product was sold by the number of those items sold within a given time period. For instance, if you sold 250 laptops at $300 each, your GMV for that particular product would be $75,000 in that specific time period.

Strategizing and focusing on optimizing GMV can help increase your user base and order volumes, and when paired with other vital ecommerce metrics like AOV, CR, RPV, you will have a comprehensive assessment of your business’s performance and profitability.

**What is the difference between Revenue and GMV?**

While GMV and Revenue are both important metrics for ecommerce businesses, they represent different aspects of your sales performance.

GMV, as mentioned earlier, is the total value of merchandise sold through your platform, without deducting any fees or expenses. It includes the total sales price of all products, including those sold by third-party sellers on your platform, if applicable.

Revenue, on the other hand, is the actual income your business generates from sales after deducting any fees, discounts, or expenses. For example, if you're selling products on behalf of third-party sellers, your revenue would be the commission or fees you earn from those sales, rather than the full GMV.

In summary, GMV represents the total sales volume, while Revenue represents the actual income your business earns from those sales. Both metrics are crucial for understanding your ecommerce business's performance and making informed decisions to drive growth.

[Back to Contents](#contents)

---

<a id="product-detail-page-pdp"></a>

### Product Detail Page (PDP)

**A product detail page (PDP)** is a web page on an eCommerce site that presents the description of a specific product in view. The details displayed often include size, color, price, shipping information, reviews, and other relevant information customers may want to know before making a purchase.

[Back to Contents](#contents)

---

<a id="4-1-architectural-view-model"></a>

### 4+1 architectural view model

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-008.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-009.png)

**4+1** is a view model used for "describing the architecture of software-intensive systems, based on the use of multiple, concurrent views". The views are used to describe the system from the viewpoint of different stakeholders, such as end-users, developers, system engineers, and project managers. The four views of the model are logical, development, process, and physical view. In addition, selected use cases or scenarios are used to illustrate the architecture serving as the 'plus one' view. Hence, the model contains 4+1 views:

- **Logical view**: The logical view is concerned with the functionality that the system provides to end-users. UML diagrams are used to represent the logical view, and include class diagrams, and state diagrams.
- **Process view**: The process view deals with the dynamic aspects of the system, explains the system processes and how they communicate, and focuses on the run time behavior of the system. The process view addresses concurrency, distribution, integrator, performance, and scalability, etc. UML diagrams to represent process view include the sequence diagram, communication diagram, activity diagram.
- **Development view**: The development view (aka the implementation view) illustrates a system from a programmer's perspective and is concerned with software management. UML Diagrams used to represent the development view include the Package diagram and the Component diagram.
- **Physical view**: The physical view (aka the deployment view) depicts the system from a system engineer's point of view. It is concerned with the topology of software components on the physical layer as well as the physical connections between these components. UML diagrams used to represent the physical view include the deployment diagram.
- **Scenarios**: The description of an architecture is illustrated using a small set of use cases, or scenarios, which become a fifth view. The scenarios describe sequences of interactions between objects and between processes. They are used to identify architectural elements and to illustrate and validate the architecture design. They also serve as a starting point for tests of an architecture prototype. This view is also known as the use case view.

The 4+1 view model is generic and is not restricted to any notation, tool or design method. Quoting Kruchten,

- **The “4+1” view model is rather “generic”**: other notations and tools can be used, other design methods can be used, especially for the logical and process decompositions, but we have indicated the ones we have used with success.
- — Philippe Kruchten, Architectural Blueprints—The “4+1” View Model of Software Architecture

[Back to Contents](#contents)

---

<a id="the-open-group-architecture-framework-togaf"></a>

### The Open Group Architecture Framework (TOGAF)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-010.jpeg)

**The Open Group Architecture Framework (TOGAF)** is the most used framework for enterprise architecture as of 2020 that provides an approach for designing, planning, implementing, and governing an enterprise information technology architecture. TOGAF is a high-level approach to design. It is typically modeled at four levels: Business, Application, Data, and Technology. It relies heavily on modularization, standardization, and already existing, proven technologies and products.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-011.png)

TOGAF began to be developed in 1995 by The Open Group, based on the United States Department of Defense's TAFIM and Capgemini's Integrated Architecture Framework (IAF). As of 2016, The Open Group claims that TOGAF is employed by 80% of Global 50 companies and 60% of Fortune 500 companies.

TOGAF is a widely used framework for Enterprise Architecture (EA) that provides a structured method and common language for designing, planning, implementing, and governing enterprise architecture.

**In simple terms**:

```text
👉 TOGAF is a step-by-step guide that helps organizations build and manage their EA in a consistent way.
```

**What TOGAF Provides**

- **A method (ADM – Architecture Development Method)**: a cycle to develop and improve architecture
- **Standard architecture views**: business, data, application, and technology
- **Common terminology**: so, business and IT teams understand each other
- **Governance guidance**: to ensure architecture is followed over time

**Quick Example**

**A company adopting TOGAF**:

- Uses the ADM cycle to move from current systems to a desired future state
- **Aligns business strategy with IT decisions**
- **Avoids fragmented systems and costly redesigns**

**One-line summary**:

```text
TOGAF is a proven framework that helps organizations design and evolve enterprise architecture in a clear, repeatable, and controlled way.
```

[Back to Contents](#contents)

---

<a id="enterprise-architecture-ea"></a>

### Enterprise Architecture (EA)

**Enterprise Architecture (EA)** is a high-level blueprint that shows how an organization’s business processes, information, applications, and technology fit together to achieve its goals. Enterprise Architecture (EA) translates business strategy into actionable implementation plans across processes, information, applications, and technology.

**In simple terms**:

```text
👉 EA is the plan that makes sure business strategy and IT systems work together, not against each other.
```

**One-sentence version**:

- EA helps an organization understand what it has, how it works, and what needs to change so it can operate efficiently and grow.

**Illustrative Example of Enterprise Architecture (EA)**

**Scenario**: A retail company wants to improve online sales and customer experience.

**Without EA**

- **Sales systems, inventory, and customer data are all separate**
- **Customers see items online that are actually out of stock**
- **IT teams build fixes independently, causing duplication and delays**

**With EA**

- **Business goal**: Increase online sales by 20%
- **Processes**: Order fulfillment, inventory updates, customer support are clearly mapped
- **Information**: One shared customer and product database
- **Applications**: Website, mobile app, warehouse system all connected
- **Technology**: Cloud platform supports scalability during sales peaks
- **Result**: When a customer buys online, inventory updates instantly, delivery is faster, and management can see real-time performance.

```text
👉 EA acts like a city map—it ensures roads (processes), buildings (systems), and utilities (technology) are designed to work together as the city grows.
```

**Курс от OTUS “Enterprise Architect”**: https://otus.ru/lessons/arch\_ent/

[Back to Contents](#contents)

---

<a id="procurement-life-cycle-procurement-process"></a>

### Procurement Life Cycle (Procurement Process)

**The procurement life cycle (or procurement process)** is a structured, multi-stage process for acquiring goods or services, ranging from initial need identification to final, post-purchase evaluation. The steps involving RFI, RFP, RFQ, and SLA are critical, strategic components of this cycle used to evaluate, select, and manage vendors.

**Here are the comprehensive steps of the procurement life cycle**:

- **1. Identification of Need & Specifications**
  **Action**: Identifying a business need for a product or service and defining the specifications.
  **Goal**: Determine what is needed, the required quality, quantity, and budget.
- **2. Market Research & RFI (Request for Information)**
  **Action**: Conducting market research and issuing an RFI to gather information about potential suppliers.
  **Purpose**: To understand the market, identify potential suppliers, and assess their capabilities without committing to a purchase.
- **3. Supplier Selection & RFx Process (RFP/RFQ)**
  **Action**: Shortlisting vendors and issuing formal requests based on the complexity of the need.
  **RFQ (Request for Quotation)**: Used when the requirements are standardized, fixed, and well-defined. The focus is on getting the best price and terms (e.g., buying laptops).
  **RFP (Request for Proposal)**: Used for complex projects where the vendor is expected to propose a solution, methodology, and pricing (e.g., implementing new software).
  **Goal**: Compare proposals and select the best vendor.
- **4. Negotiation & Contract Management**
  **Action**: Negotiating terms and finalizing the contract with the selected vendor.
  **SLA (Service Level Agreement) Integration**: The contract includes an SLA, which defines the performance metrics, quality standards, and responsibilities (e.g., uptime, delivery time) the vendor must meet.
- **5. Purchase Order (PO) Issuance**
  **Action**: Sending a formal Purchase Order to the supplier to authorize the purchase.
  **Goal**: Clearly define the price, specifications, terms, and conditions to avoid disputes.
- **6. Delivery, Inspection, & Receipt**
  **Action**: Receiving the goods or services and inspecting them against the purchase order and contract.
  **Goal**: Verify accuracy, quality, and compliance.
- **7. Invoice Approval & Payment**
  **Action**: Reconciling the invoice with the PO and the receiving documents (3-way matching).
  **Goal**: Processing payment according to agreed terms.
- **8. Vendor Performance Evaluation & Closure**
  **Action**: Monitoring vendor performance against the SLA, conducting audits, and closing out the project.
  **Goal**: Evaluating the efficiency of the procurement and deciding on future engagements (renewal/termination).

**Summary of Key Documents**

| **Document** | **Name** | **Purpose** | **When to Use** |
| --- | --- | --- | --- |
| **RFI** | **Request for Information** | Gather vendor info & capabilities. | Early research; unknown solutions. |
| **RFP** | **Request for Proposal** | Solicit solutions & proposals. | Complex, custom, or strategic projects. |
| **RFQ** | **Request for Quotation** | Request price quotes. | Standardized goods/services; known specs. |
| **SLA** | **Service Level Agreement** | Define performance metrics. | During contracting to set performance standards. |

<a id="procurement-lifecycle-ai-software-architect-view-customer-side-buying-organization-perspective"></a>

#### Procurement Lifecycle — AI/Software Architect View (Customer-Side/Buying Organization Perspective)

**0️⃣ Internal Need Identification (Pre-Procurement)**

- **Purpose**: Define what the organization actually needs.
- **Architect focus**:
  - **Business problem definition (avoid “solution-first” bias)**
  - **Build vs Buy vs Partner analysis**
- **Constraints**:
  - **Security / compliance (SOC2, ISO, HIPAA, GDPR, etc.)**
  - **Data residency**
  - **AI governance requirements**
  - **Integration landscape**
  - **High-level architecture draft**
  - **Rough TCO estimation**
- **Deliverables**:
  - **Architecture concept**
  - **Technical requirements outline**
  - **Risk register (initial)**

**1️⃣ RFI — Request for Information**

- **Purpose**: Market research. Understand available vendors, capabilities, and trends.
- **Used when**:
  - **Problem space unclear**
  - **New technology area (e.g., GenAI platforms, MLOps tools)**
- **Architect focus**:
  - **Evaluate**:
    - **Architecture compatibility**
    - **Deployment models (SaaS, PaaS, on-prem, hybrid)**
    - **API maturity**
    - **AI model transparency**
    - **Data handling & security posture**
  - **Ask about**:
    - **Scalability limits**
    - **Customization extensibility**
    - **Roadmap alignment**
    - **Identify risks early**
- **Output**:
  - **Shortlist of vendors**
  - **Updated technical requirements**

**2️⃣ RFP — Request for Proposal**

- **Purpose**: Formal request for detailed solution proposals.
- **Used when**:
  - **Requirements are well defined**
  - **Competitive vendor evaluation is needed**
- **Architect focus**:
  - **Provide structured requirements**:
    - **Functional**
    - **Non-functional (performance, availability, latency)**
    - **AI-specific (model retraining, explainability, evaluation metrics)**
  - **Require**:
    - **Reference architecture**
    - **Integration diagrams**
    - **Data flow diagrams**
    - **SLA commitments**
    - **Security certifications**
  - **Evaluate**:
    - **Technical fit**
    - **Architecture quality**
    - **Vendor engineering maturity**
    - **AI lifecycle support (data → model → deployment → monitoring)**
  - **Scoring criteria often include**:
    - **Architecture robustness**
    - **Extensibility**
    - **Security posture**
    - **Total cost of ownership**
    - **Vendor stability**

**3️⃣ RFQ — Request for Quotation**

- **Purpose**: Price-focused comparison (when solution scope is clear).
- **Used when**:
  - **Standardized products**
  - **Scope already locked**
- **Architect focus**:
  - **Verify**:
    - **Licensing model (per-user, per-token, per-core, per-node)**
    - **AI usage pricing (token-based? training compute?)**
    - **Scaling cost curve**
    - **Hidden infra costs**
  - **Model TCO**:
    - **3–5-year cost projections**
    - **Cloud vs vendor pricing comparison**
    - **Identify cost-risk drivers**

**4️⃣ PoC / Pilot Phase**

**Purpose**: Validate the solution technically before full commitment.

*Often happens after RFP shortlisting.*

- **Architect focus**:
  - **Define**:
    - **Success metrics (technical KPIs + business KPIs)**
    - **Data validation scenarios**
    - **Performance benchmarks**
  - **Test**:
    - **Integration complexity**
    - **Latency & throughput**
    - **Model accuracy / hallucination rates**
    - **Monitoring & observability**
  - **Evaluate**:
    - **Operational burden**
    - **Deployment automation**
    - **CI/CD compatibility**
- **Deliverables**:
  - **Architecture validation report**
  - **Risk assessment update**
  - **Recommendation memo**

**5️⃣ Vendor Evaluation & Selection**

**Purpose**: Final decision-making.

- **Architect role**:
  - **Provide technical scoring matrix**
  - **Present risk profile**
  - **Challenge unrealistic SLAs**
  - **Validate**:
    - **Vendor roadmap alignment**
    - **Exit strategy feasibility**
    - **Data portability**
  - **AI-specific concerns**:
    - **Model retrain rights**
    - **IP ownership**
    - **Responsible AI alignment**
    - **Model auditability**

**6️⃣ Contract Negotiation**

*Usually led by procurement/legal, but architect heavily influences terms.*

- **Architect involvement**:
  - **SLA definitions**:
    - **Availability %**
    - **Response time**
    - **Support tiers**
    - **Data ownership clauses**
    - **Security incident response commitments**
    - **API stability guarantees**
    - **Versioning policy**
    - **AI model update control**
  - **Red flags**:
    - **Black-box model with no transparency**
    - **No export capability**
    - **Unlimited vendor lock-in**

**7️⃣ Implementation & Onboarding**

**Purpose**: Solution rollout.

- **Architect focus**:
  - **Final architecture approval**
  - **Integration plan**
  - **Security review**
  - **Migration plan**
  - **Rollback strategy**
  - **Monitoring integration**
  - **AI model performance tracking**

**8️⃣ Vendor Management & Lifecycle Governance**

*Ongoing phase.*

- **Architect responsibilities**:
  - **Architecture drift monitoring**
  - **Performance review**
  - **SLA validation**
  - **Cost optimization**
  - **Renewal evaluation**
  - **Exit planning**
  - **Risk reassessment**
- **For AI vendors**:
  - **Model degradation tracking**
  - **Bias monitoring**
  - **Explainability validation**
  - **Compliance updates**

🧠 **Architect-Level Meta Competencies in Procurement**

**To operate at AI Architect level, you must understand**:

- **1. Technical Due Diligence**
  - **Deep API review**
  - **Security architecture assessment**
  - **Scalability validation**
  - **Observability maturity**
- **2. Financial & Risk Modeling**
  - **TCO modeling**
  - **OPEX vs CAPEX**
  - **Cost of lock-in**
  - **Regulatory risk exposure**
- **3. Governance & Compliance**
  - **Data residency**
  - **Responsible AI**
  - **Audit trails**
  - **Model traceability**
- **4. Strategic Fit**
  - **Vendor roadmap alignment**
  - **Ecosystem compatibility**
  - **Long-term innovation potential**

🔥 **What Differentiates a Senior Architect in Procurement**

| **Engineer Mindset** | **Architect Mindset** |
| --- | --- |
| “**Does it work?”** | “**Will it still work in 5 years?”** |
| “**Is the API good?”** | “**What is the exit cost?”** |
| “**Does it scale?”** | “**What is the marginal scaling cost curve?”** |
| “**Is it secure?”** | “**Can we prove compliance under audit?”** |

<a id="procurement-lifecycle-ai-software-architect-view-vendor-solution-provider-perspective"></a>

#### Procurement Lifecycle — AI/Software Architect View (Vendor/Solution Provider Perspective)

**0️⃣ Opportunity Identification (Pre-RFI Stage)**

- **Purpose**: Identify and qualify potential deals.

*This usually happens before any formal RFI/RFP.*

- **Architect involvement**:
  - **Support sales with technical feasibility input**
  - **Shape early solution vision**
  - **Assess**:
    - **Customer technical maturity**
    - **Budget probability**
    - **Integration complexity**
    - **Regulatory environment**
  - **Flag red flags**:
    - **Unrealistic scope**
    - **Custom development disguised as “product fit”**
    - **Compliance blockers**
- **Deliverable**:
  - **High-level solution positioning**
  - **Rough solution architecture draft**
  - **Effort/complexity estimate**

**Architect’s mindset shift**:

“**Is this deal winnable AND deliverable?”**

**1️⃣ RFI Response Stage**

**Purpose**: Demonstrate capability & differentiate technically.

*Customer is exploring market options.*

- **Architect responsibilities**:
  - **Provide technical capability descriptions**
  - **Clarify**:
    - **Architecture overview**
    - **Scalability envelope**
    - **Deployment models**
    - **Security posture**
  - **Align marketing claims with reality**
  - **Avoid overselling features that don’t scale**
  - **For AI vendors specifically**:
    - **Model training process transparency**
    - **Data governance model**
    - **Evaluation & monitoring capabilities**
    - **Bias mitigation approach**

**Key risk**:

```text
Marketing-driven exaggeration without architectural validation.
```

**2️⃣ Pre-RFP Solution Shaping (Competitive Positioning Phase)**

*Often occurs between RFI and RFP.*

**Critical vendor-side architect role**:

- **Influence requirements (legally and strategically)**
- **Run workshops**
- **Conduct discovery sessions**
- **Steer architecture direction toward strengths**

**Strategic moves**:

- **Push for**:
  - **Standard APIs instead of custom interfaces**
  - **Managed deployment vs on-prem (if SaaS vendor)**
  - **Usage-based pricing if advantageous**
- **Identify competitor weaknesses**
- **Propose PoC to anchor solution direction**

<u>**This is where senior architects create major advantage.**</u>

**3️⃣ RFP Response Development**

<u>**Now things get formal and high-stakes.**</u>

**Architect is central here.**

**Responsibilities**:

- **A. Solution Architecture Definition**
  - **Reference architecture**
  - **Logical & physical diagrams**
  - **Integration patterns**
  - **Data flow**
  - **Security controls**
  - **AI lifecycle diagram (data → training → inference → monitoring)**
- **B. Compliance Mapping**
  - **Map requirements → capabilities**
  - **Identify gaps**
  - **Propose mitigation plans**
- **C. Delivery Model**
  - **Implementation timeline**
  - **Required customer responsibilities**
  - **Dependencies**
- **D. Risk Analysis**
  - **Performance risk**
  - **Customization risk**
  - **Integration risk**
  - **AI model performance variability**

**Architect challenge**:

```text
Balance win strategy vs realistic delivery capability.
```

**4️⃣ Pricing & Cost Modeling (RFQ Stage)**

<u>**Now commercial pressure increases.**</u>

**Architect supports**:

- **Infrastructure cost modeling**
- **Cloud resource estimations**
- **AI compute estimation (training + inference)**
- **Scalability cost curves**
- **Margin protection**

**For AI products**:

- **Token usage assumptions**
- **GPU cost modeling**
- **Model retraining resource allocation**
- **Observability tooling cost**

**Architect must avoid**:

- **Underestimating infra costs to win deal**
- **Committing to unrealistic SLA**

**5️⃣ PoC / Pilot Execution**

<u>**Now architecture is validated under scrutiny.**</u>

**Vendor-side architect must**:

- **Define controlled scope**
- **Select representative datasets**
- **Ensure success criteria are measurable**
- **Protect IP boundaries**
- **Avoid custom snowflake implementations**

**Critical evaluation areas**:

- **Performance under real data**
- **Latency under production-like load**
- **AI accuracy / hallucination behavior**
- **Deployment friction**

**Common vendor mistake**:

```text
Over-customizing PoC → creating future technical debt.
```

**6️⃣ Technical Negotiation Phase**

<u>**This is where senior architects shine.**</u>

**Topics**:

- **SLA commitments**
- **RPO / RTO guarantees**
- **Security controls**
- **Audit rights**
- **AI retraining frequency**
- **Model explainability obligations**
- **Data ownership & portability**
- **Escrow arrangements (in some industries)**

**Architect must assess**:

- **What is contractually survivable**
- **What creates operational risk**
- **What increases long-term cost**

**7️⃣ Contract Award → Transition to Delivery**

**Architect involvement shifts to**:

- **Final architecture validation**
- **Delivery alignment**
- **Internal handover to implementation team**

**Identify**:

- **Customizations**
- **Integration workstreams**
- **Deployment dependencies**

<u>**This is where architecture realism becomes critical.**</u>

**8️⃣ Implementation Phase (Customer Delivery)**

**Vendor-side architect ensures**:

- **Architecture integrity**
- **Scope control**
- **Change management discipline**
- **Performance benchmarks**
- **AI monitoring implementation**

**Must monitor**:

- **Scope creep**
- **Customer “just one more integration” risk**
- **Shadow feature commitments from sales**

**9️⃣ Post-Go-Live & Account Expansion**

**Architect becomes strategic again.**

**Responsibilities**:

- **Performance optimization**
- **AI model evolution strategy**
- **Upsell roadmap alignment**
- **Architecture scaling strategy**
- **Renewal risk monitoring**

**For AI systems**:

- **Model drift detection**
- **Dataset expansion strategy**
- **Responsible AI compliance updates**
- **Observability enhancement**

🧠 **What Changes from Customer → Vendor Perspective?**

| **Customer-Side Architect** | **Vendor-Side Architect** |
| --- | --- |
| **Minimize risk** | **Manage risk while enabling revenue** |
| **Evaluate vendors** | **Be evaluated** |
| **Seek flexibility** | **Protect product boundaries** |
| **Avoid lock-in** | **Increase stickiness (ethically)** |
| **Challenge SLA** | **Defend SLA feasibility** |
| **Reduce cost** | **Protect margin** |

🔥 **Advanced Vendor-Side Architect Skills**

**To grow into Solution Architect / Enterprise Architect on vendor side, you must master**:

- **1️⃣ Bid Strategy Thinking**
  - **Competitive positioning**
  - **Solution differentiation**
  - **Requirement shaping**
- **2️⃣ Commercial Awareness**
  - **Cost-to-serve modeling**
  - **Margin sensitivity**
  - **Pricing elasticity**
- **3️⃣ Controlled Customization**
  - **How much deviation product can absorb**
  - **When to refuse custom development**
- **4️⃣ AI-Specific Competencies**
  - **Training vs inference cost dynamics**
  - **GPU resource economics**
  - **Hallucination risk communication**
  - **Governance defensibility**
  - **Model drift operationalization**

🎯 <u>**Key Insight**</u>

**On the customer side**:

```text
Architecture protects the enterprise.
```

**On the vendor side**:

```text
Architecture protects the product AND the company’s profitability.
```

[Back to Contents](#contents)

---

<a id="reliability-quality-delivery"></a>

## Reliability, Quality & Delivery

<a id="slo-service-level-objective-sli-service-level-indicator"></a>

### SLO (Service Level Objective) & SLI (Service Level Indicator)

**SLO (Service Level Objective) and SLI (Service Level Indicator)** are core concepts in reliability, where an SLI is the measured metric (e.g., % uptime), the SLO is the target for that metric (e.g., 99.9% uptime), and these support the broader SLA (Service Level Agreement), which is the formal contract with customers, outlining these performance goals and consequences. In short, SLIs measure, SLOs set the goal, and SLAs are the promises.

**SLI (Service Level Indicator)**

- **What it is**: A specific, quantitative measurement of a service's performance (e.g., latency, error rate, throughput, availability).
- **Purpose**: To objectively track how well a service is performing.
- **Example**: The actual percentage of successful user requests in the last 30 days.

**SLO (Service Level Objective)**

- **What it is**: A specific, measurable target value or range for an SLI.
- **Purpose**: To define what "good" performance looks like for internal teams and stakeholders, often driving reliability work.
- **Example**: "99.9% of user requests must be successful over a 30-day period" (the SLO for the SLI above).

**Relationship to SLA (Service Level Agreement)**

An SLA is the external, often legal, commitment to a customer that includes one or more SLOs and details penalties if they aren't met.

SLOs live inside the SLA, giving concrete targets to the general promises made in the agreement.

**How they work together (Example)**

- **SLA**: "We guarantee 99.9% uptime for our service."
- **SLO**: "Our internal target is 99.95% uptime to give us a buffer before hitting the SLA."
- **SLI**: "We measure uptime as the percentage of successful pings to our server, and it's currently 99.92%."

If the SLI (99.92%) falls below the SLO (99.95%), the team knows they need to act quickly to improve reliability before the SLA (99.9%) is breached.

[Back to Contents](#contents)

---

<a id="probabilistic-sla-service-level-agreement"></a>

### Probabilistic SLA (Service Level Agreement)

A Probabilistic SLA (Service Level Agreement) moves beyond simple "yes/no" performance metrics (like 99.9% uptime) to define service quality using statistical probabilities, focusing on the likelihood of meeting targets (e.g., 99% chance of &lt;100ms latency) and using AI/modeling to predict violations, manage resource allocation better, and provide more realistic expectations for complex cloud or distributed systems. It's about managing risk and ensuring expected service levels most of the time, not guaranteeing perfection.

**Key Concepts**

- **Probabilistic Metrics**: Instead of hard guarantees, it uses distributions (e.g., latency follows a certain curve) and probabilities (e.g., 95% of requests finish in &lt;1s).
- **Predictive Capabilities**: Uses AI, machine learning, and statistical models (like Extreme Value Theory) to forecast potential violations before they happen, enabling proactive adjustments.
- **Resource Optimization**: Helps cloud providers allocate resources more efficiently by understanding peak demand probabilities, leading to cost savings.
- **Managing Expectations**: Acknowledges that failures happen and focuses on defining acceptable risk, allowing for realistic, rather than over-pessimistic, contracts.

**How it Works**

- **Data Analysis**: Analyzes historical data and patterns to build models of service behavior.
- **Statistical Modeling**: Uses techniques like Deep Mixture Density Networks (MDNs) or Extreme Value Theory (EVT) to predict delay distributions or resource demand.
- **Monitoring & Prediction**: Continuously monitors, but also anticipates, potential breaches, sending early alerts.
- **Dynamic Adaptation**: Allows for real-time reconfiguration or adaptation of services when a violation probability becomes too high.

**Benefits**

- **More Realistic Contracts**: Avoids overly strict, costly "hard contracts".
- **Proactive vs. Reactive**: Shifts from reacting to failures to preventing them.
- **Better Resource Use**: Optimizes infrastructure for actual, probable demand.
- **Comprehensive Validation**: Allows for checking potential consequences before deployment.

[Back to Contents](#contents)

---

<a id="ci-cd-pipelines"></a>

### CI/CD Pipelines

Choosing the right CI/CD tool depends heavily on your existing ecosystem, team size, and specific needs, as each tool offers distinct advantages in integration, flexibility, or performance.

| **Platform** | **Best For** | **Key Strengths** | **Considerations** |
| --- | --- | --- | --- |
| **GitHub Actions** | Teams already using GitHub repositories and looking for seamless integration and ease of use. | • Deep native integration with GitHub workflows (PRs, issues, etc.).<br>• Massive marketplace of prebuilt actions.<br>• Auto-scaling hosted runners and generous free tier for public repos. | Locks you into the GitHub ecosystem; complex pipelines can become verbose. |
| **GitLab CI/CD** | Enterprises and teams seeking a single, integrated DevOps platform from source code to deployment. | • A complete all-in-one DevOps solution (registry, security scanning, etc.) built-in.<br>• Strong support for self-hosting and Kubernetes integration.<br>• Employs a simpler, stage-based YAML structure, easier to learn initially. | Steeper learning curve due to the breadth of features; requires significant infrastructure for self-hosting. |
| **Jenkins** | Teams with legacy systems or unique hardware constraints requiring maximum flexibility and control. | • Unparalleled flexibility and vast, mature open-source plugin ecosystem.<br>• Runs on virtually any infrastructure (self-hosted, hybrid cloud).<br>• Highly customizable for complex enterprise setups. | High maintenance overhead and requires strong ops capacity; no managed cloud offering (requires self-management of scaling and infrastructure). |
| **Azure DevOps** | Organizations heavily invested in the Microsoft ecosystem and Azure services. | • A complete suite of DevOps tools (Boards, Repos, Pipelines, etc.) with excellent governance.<br>• Optimized integration for .NET and Azure deployments.<br>• Strong performance for large-scale, complex enterprise projects. | Less ideal if you are not using Azure or other Microsoft products extensively. |
| **CircleCI** | Teams prioritizing high performance, speed, and portability across different Git providers. | • Performance-focused with advanced caching and parallelism for faster builds.<br>• Platform independence (works with GitHub, GitLab, Bitbucket).<br>• Good web UI and curated "orbs" (reusable configuration packages) ecosystem. | Primarily a CI/CD tool, lacks the broader integrated DevOps suite of GitLab or Azure DevOps. |
| **Bitbucket Pipelines** | Teams already using Bitbucket for source control and other Atlassian tools (Jira, Confluence). | • Seamless integration within the Atlassian ecosystem.<br>• Offers unlimited private repositories with its core offering.<br>• Simple, usage-based pricing model. | Smaller community and fewer third-party integrations compared to GitHub or Jenkins. |

**Summary & Recommendations**

For maximum convenience and ease of use, especially for open-source projects or small-to-midsize teams, GitHub Actions is the new standard if your code is already on GitHub.

For a unified, enterprise-grade, end-to-end DevOps platform, GitLab CI/CD is an excellent choice that includes built-in security and compliance features.

For specialized needs, legacy systems, or complete control over infrastructure, Jenkins remains the most flexible, albeit maintenance-heavy, option.

For large organizations within the Microsoft ecosystem, Azure DevOps offers a powerful, integrated suite.

For teams focused purely on build speed and portability, CircleCI provides high performance and a clean interface.

[Back to Contents](#contents)

---

<a id="user-acceptance-testing-software"></a>

### User Acceptance Testing (Software)

**UAT** is the final phase of the software testing process before a product goes live. Its primary purpose is to verify that the software meets business requirements and works correctly in real-world scenarios from the perspective of the end-user.

- **Who performs it**: Actual end-users, clients, or business stakeholders, rather than the development or QA teams.

**Key Types**:

- **Alpha Testing**: Internal testing by employees in a controlled environment.
- **Beta Testing**: Testing by external users in their own real-world environment.
- **Operational Acceptance Testing (OAT)**: Checks for backup, recovery, and maintenance readiness.
- **Regulation/Contractual Acceptance**: Ensures the software complies with legal standards or specific contract terms.

- **UAT vs. QA**: While Quality Assurance (QA) focuses on technical bug-fixing and functional correctness, UAT focuses on whether the software is "fit for purpose" and handles business workflows correctly.

[Back to Contents](#contents)

---

<a id="pair-programming"></a>

### Pair Programming

**Pair programming** is an Agile software development technique where two developers share one workstation to design and code, with one acting as the "driver" (typing) and the other as the "navigator" (reviewing, strategizing). They swap roles frequently to improve code quality, reduce bugs, and share knowledge.

**Key Aspects of Pair Programming**

- **Roles**: The Driver focuses on the immediate code details, while the Navigator focuses on the strategic direction, potential bugs, and code quality.
- **Communication**: Effective, continuous dialogue (programming out loud) is essential to ensure both partners are engaged.
- **Techniques**: Partners often switch roles every 15–30 minutes. A common approach is "Ping Pong," where one person writes a test and the other makes it pass, alternating roles.
- **Remote Work**: Modern pair programming often uses screen-sharing tools (e.g., Tuple, VS Code Live Share).

**Benefits**

- **Higher Quality Code**: Real-time, continuous review catches errors immediately, reducing bugs and the need for later debugging.
- **Knowledge Transfer**: It is an effective way to onboard new members or pair senior/junior developers, accelerating learning.
- **Reduced Risk**: It minimizes the "bus factor" (dependency on one person) by ensuring shared knowledge of the codebase.
- **Improved Focus**: The collaborative nature keeps developers engaged and reduces fatigue.

**Challenges & Best Practices**

- **Not Just "Double Costs"**: Although it requires two developers for one task, it reduces the time spent on formal code reviews and debugging, improving long-term productivity.
- **Active Engagement**: Both developers must be active. Passive observing by the navigator leads to poor results.
- **Pair Compatibility**: Different personalities and experience levels (e.g., expert-expert, expert-novice) can work together, but require good communication skills.

[Back to Contents](#contents)

---

<a id="unit-tests-edge-cases"></a>

### Unit Tests & Edge Cases

Unit tests verify the smallest, isolated parts of an application (functions or methods) act as expected, while edge cases are extreme, rare, or boundary-condition inputs (e.g., maximum/minimum values, empty inputs, null) designed to push code to its limits. Together, they ensure code reliability and robust error handling.

**Key Aspects of Unit Tests and Edge Cases**

- **Unit Tests**: These are automated scripts that check if a specific code unit works correctly by verifying inputs against expected outputs. They are crucial for testing individual functions in isolation.
- **Edge Cases**: These are scenarios that fall outside of normal, everyday usage. They often represent the boundaries of acceptable input, such as:
  - **Boundary Conditions**: Testing exactly at, just below, or just above maximum allowed limits.
  - **Empty/Null Input**: Passing empty strings, null, zero, or empty lists to functions that expect data.
  - **Unexpected Types**: Entering characters into numeric fields or extremely large numbers.
  - **Rare Scenarios**: Unlikely but possible events (e.g., Leap Day, extremely long usernames).
- **Importance**: Testing these cases ensures the software does not crash and behaves gracefully under unusual conditions.

**Common Examples of Edge Cases**

- **Numbers**: Maximum, minimum, zero, or negative numbers.
- **Strings**: Empty strings, extremely long strings, or strings with special characters/SQL injection attempts.
- **Dates**: February 29th (leap year), invalid dates, or dates before/after system support ranges (e.g., 1901-2038).
- **Lists/Arrays**: Empty lists, single-element lists, or excessively large lists.

[Back to Contents](#contents)

---

<a id="pytest-archon"></a>

### pytest-archon

**pytest-archon** is a pytest plugin that enforces architectural boundaries and forbidden dependencies in Python projects. It helps maintain code structure, prevent circular dependencies, and ensure that new developers are aware of project structure rules through automated tests.

**Key Features**

- **Architectural Rule Definition**: Allows developers to codify architectural rules within simple Python test cases using the pytest framework.
- **Dependency Checks**: Primarily focuses on analyzing and validating import statements in the codebase to ensure certain modules do not import from forbidden areas.
- **Integration with CI/CD**: Since the rules are implemented as tests, they can be seamlessly integrated into a Continuous Integration/Continuous Deployment (CI/CD) pipeline to catch violations early.
- **Pythonic Syntax**: The tool uses standard Python code for defining rules, eliminating the need for separate configuration files (like YAML), which makes it more natural for Python developers and allows for static analysis tools like formatters and linters to work effectively.
- **Flexibility**: It supports fnmatch syntax (default) and regular expressions for matching module names and defining constraints.

**Usage**

To use pytest-archon, you define architectural rules in your test suite using the provided fixtures and methods, such as archrule(), match(), should\_import(), and should\_not\_import().

**Installation**

**You can install pytest-archon using pip**:

```bash
pip install pytest-archon
```

More installation options and details are available on the PyPI page and the GitHub repository.

**Example Rule**

A simple example of an architectural test might look like this (within a standard pytest test file):

```python
from pytest_archon import archrule
def test_architecture_boundaries():
    (archrule()
        .match("my_app.domain.*")
        .should_not_import("my_app.data.*")
    )
```

This test asserts that no module within the my\_app.domain package should import anything from the my\_app.data package. The rules are enforced automatically when running your tests with pytest.

[Back to Contents](#contents)

---

<a id="web-apis-integration"></a>

## Web, APIs & Integration

<a id="server-sent-events-sse"></a>

### Server-Sent Events (SSE)

**Server-Sent Events (SSE)** is a server push technology enabling a client to receive automatic updates from a server via an HTTP connection, and describes how servers can initiate data transmission towards clients once an initial client connection has been established. They are commonly used to send message updates or continuous data streams to a browser client and designed to enhance native, cross-browser streaming through a JavaScript API called EventSource, through which a client requests a particular URL in order to receive an event stream.

Server-Sent Events (SSE) is a web technology allowing servers to push real-time updates to clients (browsers) over a single, persistent HTTP connection, enabling unidirectional (server-to-client) streaming for live data like notifications, stock tickers, or news feeds without constant client polling. It uses the native browser EventSource API, relies on standard HTTP, and is simpler than WebSockets for scenarios where only server pushes are needed, automatically handling reconnections and heartbeats.

**How it Works**

- **Client Initiates**: The client creates an EventSource object, pointing to a specific URL on the server.
- **Persistent Connection**: The server responds with a text/event-stream MIME type, establishing a long-lived HTTP connection.
- **Server Pushes Data**: The server sends text-based events, formatted with data: (payload) and event: (type) fields, separated by double newlines (\\n\\n).
- **Automatic Reconnection**: Browsers automatically try to re-establish the connection if it drops, and the server can send heartbeats (comment messages like : heartbeat\\n\\n) to keep it alive.

**Key Characteristics**

- **Unidirectional**: Data flows only from server to client.
- **HTTP-based**: Works over standard HTTP, making it firewall-friendly and compatible with existing infrastructure.
- **Simpler Than WebSockets**: Less overhead and complexity than full-duplex (bidirectional) WebSockets for simple push scenarios.
- **Native Browser API**: Uses the EventSource interface, providing built-in support for reconnection and event handling.

**Common Use Cases**

- **Live sports scores & stock market tickers**
- **Real-time notifications (new messages, likes, comments)**
- **Progress updates for long-running background tasks (e.g., AI processing)**
- **News feeds and real-time dashboards**

Server-Sent Events (SSE) in LLM generation streams responses token-by-token over a single HTTP connection, providing a fast, efficient, and simple way to create responsive AI applications like chatbots, making them appear instantaneous as text unfolds in real-time. This unidirectional protocol sends data from the server (LLM) to the client (browser), allowing users to see output as it's generated, unlike waiting for the whole response, and enabling features like early cancellation. SSE is lightweight, uses standard HTTP, and handles reconnections automatically, making it a popular choice over WebSockets for this specific use case.

**How it works**

- **Token Streaming**: The LLM generates text (tokens) and sends them in small chunks to the client as soon as they're ready, rather than buffering the entire message.
- **Unidirectional Flow**: Communication goes only from server to client, simplifying the protocol and reducing overhead.
- **HTTP-Based**: It leverages standard HTTP, making it easy to implement and compatible with existing infrastructure, load balancers, and firewalls.
- **EventSource API**: Clients use the browser's built-in EventSource API to listen for these streams, handling connection, reconnection, and message parsing.

**Key benefits for LLMs**

- **Improved User Experience**: Users see results instantly, creating a fluid, interactive, and less frustrating experience, similar to ChatGPT.
- **Efficiency**: Reduces perceived latency and allows for early user intervention (e.g., stopping a bad response).
- **Simplicity & Scalability**: Easier to implement and scale than bidirectional protocols like WebSockets for this type of push update.

**Implementation**

- **Backend**: Frameworks like FastAPI or Node.js with libraries (e.g., TanStack AI) can create an EventSource response from a generator function that yields streamed chunks.
- **Frontend**: JavaScript's EventSource object connects to the endpoint and listens for message events, appending data to the UI.
- **Completion**: The stream ends with a special \[DONE\] message, and the connection closes.

[Back to Contents](#contents)

---

<a id="websocket-http-uri-url-urn"></a>

### WebSocket, HTTP, URI, URL, URN

#### WebSocket

**Definition**:

- A protocol enabling full-duplex communication over a single TCP connection.

**Key Points**:

- **Uses TCP (ports 80 / 443)**
- **Starts as HTTP → upgrades**
- **Persistent connection**

👉 <u>**Format to remember (WebSocket URL)**</u>:

```text
ws://[IP_or_Domain]:[Port]/[Path]
wss://[IP_or_Domain]:[Port]/[Path]
```

**Real examples**:

```text
ws://192.168.1.50:8080/chat
wss://example.com:443/socket
```

**Example use**:

- **Live chat app**

#### WebSocket vs HTTP

- **HTTP**: Request → Response
- **WebSocket**: Continuous two-way connection

👉 <u>**Format to remember**</u>:

```text
HTTP: Client → Request → Server → Response (closed)
WebSocket:
Client ⇄ Server (open connection)
```

**Example**:

- **HTTP → refresh page**
- **WebSocket → live updates**

#### WebSocket Handshake

**Definition**:

- **Switch from HTTP → WebSocket using Upgrade header**

👉 <u>**Format to remember**</u>:

```http
GET /chat HTTP/1.1
Host: example.com
Upgrade: websocket
Connection: Upgrade
```

**Example flow**:

```text
Client → "Upgrade request"
Server → "101 Switching Protocols"
```

#### Full-Duplex Communication

**Definition**:

- **Both sides send data at the same time**

👉 <u>**Format to remember**</u>:

```text
Client ⇄ Server (simultaneous)
```

**Example**:

- **Video call / multiplayer game**

#### ws vs wss

- **ws → unsecured**
- **wss → encrypted (like HTTPS)**

👉 <u>**Format to remember**</u>:

```text
ws  = WebSocket (HTTP)
wss = Secure WebSocket (HTTPS)
```

**Examples**:

```text
ws://example.com/socket
wss://example.com/socket
```

#### URI (Uniform Resource Identifier)

👉 <u>**Format to remember**</u>:

```text
scheme:[//authority]path[?query][#fragment]
```

**Components (quick meaning)**

- **scheme → protocol/type (http, mailto, urn)**
- **authority → domain/IP (+ optional port)**
- **path → specific resource location**
- **query → extra parameters (?key=value)**
- **fragment → section inside resource (#section)**

**All Common Combinations**

- **1. Only scheme + path**

```text
mailto:user@example.com
```

*Sends email to a user.*

- **scheme = mailto**
- **path = email address**

- **2. Scheme + authority**

```text
https://example.com
```

*Basic website access.*

- **scheme = https**
- **authority = domain**

- **3. Scheme + authority + path**

```text
https://example.com/docs
```

*Access specific page.*

- **path = /docs**

- **4. + Query**

```text
https://example.com/search?q=websocket
```

*Search request.*

- **query = q=websocket**

- **5. + Fragment**

```text
https://example.com/docs#section2
```

✔ **Jump to section**

- **fragment = section2**

- **6. Full combination**

```text
https://example.com:8080/docs/api?q=test#top
```

✔ **Full URI**

- **scheme = https**
- **authority = example.com**: 8080
- **path = /docs/api**
- **query = q=test**
- **fragment = top**

- **7. No authority (URN-style URI)**

```text
urn:isbn:9780131103627
```

✔ **Identifies a book**

- **scheme = urn**
- **path = isbn**: ...

🔹 **URL (Uniform Resource Locator)**

👉 <u>**Format to remember**</u>:

```text
protocol://domain:port/path?query
```

🧩 **Components**

**protocol → how to access (http, https, ftp)**

**domain → server location**

**port → optional (default hidden)**

**path → resource**

**query → parameters**

✅ **Combinations**

**1. Basic URL**

```text
https://google.com
```

✔ **Loads homepage**

**protocol = https**

**domain = google.com**

**2. With path**

```text
https://example.com/blog
```

✔ **Specific page**

**3. With port**

```text
http://192.168.1.10:3000
```

✔ **Local server**

**port = 3000**

**4. With query**

```text
https://example.com/search?q=chatgpt
```

✔ **Search request**

**5. Full combination**

```text
https://api.example.com:443/users?id=10
```

✔ **API call**

**path = /users**

**query = id=10**

**6. Different protocol**

```text
ftp://files.example.com/download.zip
```

✔ **File download via FTP**

🔹 **URN (Uniform Resource Name)**

👉 <u>**Format to remember**</u>:

```text
urn:<namespace>:<resource-name>
```

🧩 **Components**

**urn → scheme**

**namespace → category/type**

**resource-name → unique identifier**

✅ **Examples & Variations**

**1. ISBN (books)**

```text
urn:isbn:9780131103627
```

✔ **Identifies a book**

**2. UUID**

```text
urn:uuid:123e4567-e89b-12d3-a456-426614174000
```

✔ **Unique system identifier**

**3. ISSN (journals)**

```text
urn:issn:2049-3630
```

✔ **Journal identifier**

**4. Custom namespace**

```text
urn:example:user12345
```

✔ **App-specific ID**

🔁 **Quick Comparison (with formats)**

```text
URI  = scheme:[//authority]path[?query][#fragment]
URL  = protocol://domain:port/path?query
URN  = urn:<namespace>:<resource-name>
```

🧠 **Super Quick Memory Trick**

**URI → general structure (everything)**

**URL → “where + how to access”**

**URN → “just a name”**

🔹 **cURL**

**Definition**:

- **CLI tool to transfer data using URLs**

👉 <u>**Format to remember**</u>:

```text
curl [options] [URL]
```

**Common examples**:

```text
curl https://api.example.com/data
curl -X POST https://api.example.com/login
curl -O https://example.com/file.zip
```

- The command curl -X POST is used to send data to a server using the HTTP POST method. While curl often defaults to POST when you include data (using -d), explicitly using -X POST ensures the request method is forced, which is common for interacting with REST APIs.
- In the curl command-line tool, the -O (uppercase "O") flag is used to download a file and save it using its original remote filename.

🔹 **Why WebSocket is Useful**

**Benefits**:

- **Real-time communication**
- **Low latency**
- **Less overhead than polling**

👉 <u>**Format to remember (comparison)**</u>:

```text
Polling: Request → Wait → Request → Wait
WebSocket: Open → Stream data continuously
```

**Examples**:

- **Chat apps**
- **Live dashboards**
- **Online games**

🔁 **Quick Self-Test (Active Recall)**

**What is the WebSocket URL format?**

- **What header enables protocol switch?**
- **Difference**: URI vs URL vs URN?
- **When use wss instead of ws?**
- **Basic curl command format?**

[Back to Contents](#contents)

---

<a id="tcp-udp-ports"></a>

### TCP/UDP Ports

**Ports (in short)**:

👉 **Definition**:

- A port is a number that identifies a specific service or application on a computer over a network.

🔹 **Simple Idea**

**IP address = computer**

- **Port = specific app/service on that computer**

👉 **Think of it like**:

**IP = building address**

- **Port = room number**

🔹 <u>**Format to remember**</u>

```text
[IP or Domain]:[Port]
```

**Examples**:

```text
192.168.1.1:80
example.com:443
```

🔹 **Common Ports**

**80 → HTTP (web)**

- **443 → HTTPS (secure web)**
- **21 → FTP**
- **22 → SSH**
- **3306 → MySQL**

🔹 **Why ports matter**

They allow multiple services on one machine to work at the same time.

👉 **Example**:

**example.com**: 80 → website

- **example.com**: 22 → remote login (SSH)

🔁 **One-line recall**

👉 **Port = “which service on this computer?”**

<a id="how-many-ports-are-there"></a>

#### How many ports are there?

👉 **Total ports**:

- **65,536 ports (numbered from 0 to 65535)**

🔹 <u>**Format to remember**</u>

```text
0 → 65535
```

🔹 <u>**Port Ranges (important for recall)**</u>

**1. Well-Known Ports**

```text
0 – 1023
```

✔ **Reserved for common services**

**Examples**:

- **80 → HTTP**
- **443 → HTTPS**
- **22 → SSH**

**2. Registered Ports**

```text
1024 – 49151
```

✔ **Used by applications/services**

**Example**:

- **3306 → MySQL**

**3. Dynamic / Private Ports**

```text
49152 – 65535
```

✔ **Temporary ports (used by clients)**

🔁 **Quick Memory Trick**

```text
0–1023     → System (well-known)
1024–49151 → Apps (registered)
49152–65535 → Temporary (dynamic)
```

🧠 **One-line recall**

👉 A computer has 65,536 ports (0–65535), divided into system, application, and temporary ranges.

<a id="tcp-vs-udp-ports"></a>

#### TCP vs. UDP Ports

👉 **Short answer**:

Yes — when we say 65,536 ports (0–65535), we usually mean TCP ports, but the same range also exists for UDP.

🔹 **Key Idea**

👉 **Ports are tied to protocols, not shared globally**

**TCP → has ports 0–65535**

- **UDP → also has ports 0–65535**

✔ **So technically**:

```text
TCP ports:  0–65535
UDP ports:  0–65535
```

🔹 <u>**Important Insight**</u>

👉 **Port numbers can repeat across protocols**

**Example**:

- **Port 80 (TCP) → HTTP**
- **Port 80 (UDP) → could be used by something else**

✔ These are different because protocol is different.

🔹 <u>**Format to remember**</u>

```text
[Protocol] + [IP]:[Port]
```

**Examples**:

```text
TCP 192.168.1.1:80
UDP 192.168.1.1:53
```

🔹 **Real Examples**

**TCP 443 → HTTPS**

- **UDP 53 → DNS**
- **TCP 22 → SSH**

🔁 **One-line recall**

👉 Each protocol (TCP/UDP) has its own 0–65535 port range — ports are not shared between them.

**Kong API Gateway Platform**

https://konghq.com/

Kong is a leading open-source, cloud-native API gateway and management platform designed for high-performance and scalability. It acts as a central orchestration layer between clients and microservices, managing traffic through proxying and routing.

**Core Capabilities**

- **High Performance**: Built on NGINX and LuaJIT, it can handle over 50,000 transactions per second per node with ultra-low latency.
- **Extensibility**: Features a pluggable architecture with over 400 community and enterprise plugins for authentication, security, and traffic control.
- **Deployment Flexibility**: Supports hybrid-cloud, multi-cloud, and Kubernetes-native deployments via the Kong Ingress Controller.
- **AI & LLM Gateway**: Provides advanced traffic management for AI-native applications, including multi-LLM support and semantic security.

**Key Components**

- **Kong Gateway**: The core reverse proxy that routes and secures requests.
- **Admin API**: A RESTful interface for managing and configuring the gateway's services, routes, and plugins.
- **Kong Konnect**: A SaaS-based unified API platform for managing the entire API lifecycle.
- **Insomnia**: An open-source tool for API design and testing.

**Essential Features**

- **Security**: Implements authentication (OAuth2, Key Auth, OIDC), ACLs, and IP restrictions.
- **Traffic Management**: Native support for rate limiting, load balancing, health checks, and caching.
- **Observability**: Centralized logging and real-time analytics integrated with tools like Elasticsearch and Prometheus.
- **Dev Portal**: A customizable catalog for internal and external developers to discover and access APIs.

You can get started by following the official installation guides for Docker, Kubernetes, or various Linux distributions.

[Back to Contents](#contents)

---

<a id="round-trip-time-rtt"></a>

### Round-Trip Time (RTT)

**Round-Trip Time (RTT)**, also called **round-trip delay (RTD)**, is the time it takes for a packet or signal to go from sender to receiver and for the response or acknowledgement to come back.

In simple terms:

```text
RTT = time to go there + time to come back
```

**RTT vs. One-Way Latency**

- **One-way latency**: how long traffic takes to travel in one direction.
- **RTT**: the full there-and-back time.
- A rough estimate is:

```text
one-way delay ≈ RTT / 2
```

This is only an approximation because the path there and the path back may not be identical.

**RTT and Ping**

- RTT is often discussed as **ping time**.
- The `ping` command measures response time using ICMP packets.
- Ping is useful, but it may not perfectly match the delay experienced by real application traffic.
- Different protocols can have different packet sizes, priorities, routing behavior, and processing overhead.

**What Contributes to RTT**

| **Delay source** | **Simple meaning** |
| --- | --- |
| **Propagation delay** | Time for the signal to physically travel across distance. |
| **Transmission delay** | Time needed to put all bits of a packet onto the link. Larger packets take longer. |
| **Queueing delay** | Time spent waiting in network device queues during congestion. |
| **Processing delay** | Time routers, switches, servers, or endpoints spend handling the packet. |

**Bandwidth vs. RTT**

- Higher bandwidth can reduce **transmission delay**, because data can be pushed onto the link faster.
- Higher bandwidth does **not** remove propagation delay.
- If two endpoints are far apart, the speed-of-light/distance part of the delay still remains.

**Long Fat Networks**

**Long fat networks** are networks with:

- **high bandwidth**
- **high RTT**

This means a lot of data can be “in flight” at the same time.

```text
high bandwidth × high RTT = large bandwidth-delay product
```

These networks need protocol support that can keep enough data moving without waiting too much for acknowledgements. One example is the **TCP window scale option**, which allows TCP to use larger windows.

**RTT in TCP**

- TCP estimates RTT continuously so it can decide how long to wait before retransmitting lost data.
- Older TCP logic used a weighted average of old RTT and new RTT samples.
- A weighting factor close to `1` makes the estimate stable and less sensitive to short spikes.
- A weighting factor close to `0` makes the estimate react quickly to changes.
- Later algorithms, such as Jacobson/Karels, improved this by also considering delay variation.

**Wi-Fi Positioning**

- Accurate RTT measurement over Wi-Fi can also be used for positioning.
- IEEE `802.11mc` uses round-trip timing measurements to estimate distance between Wi-Fi devices.

**One-Line Summary**

```text
RTT tells you how long a network conversation takes to make a full round trip, and it is a key metric for latency, TCP behavior, and user-perceived responsiveness.
```

[Back to Contents](#contents)

---

<a id="unix-domain-socket-uds"></a>

### Unix Domain Socket (UDS)

**A Unix domain socket (UDS)**, also called a **local socket** or **inter-process communication (IPC) socket**, is a communication endpoint used for data exchange between processes running on the same Unix or Unix-like operating system.

**Socket Domain**

- The term **Unix domain socket** refers to the `domain` argument value `AF_UNIX` passed to the system call that creates the socket.
- The same communication domain can also be selected with `AF_LOCAL`.\[1\]

**Valid UDS Socket Types**

| **Socket type** | **Comparable network concept** | **Meaning** |
| --- | --- | --- |
| `SOCK_STREAM` | TCP | A stream-oriented socket. |
| `SOCK_DGRAM` | UDP | A datagram-oriented socket that preserves message boundaries. On most Unix implementations, Unix domain datagram sockets are reliable and do not reorder datagrams. |
| `SOCK_SEQPACKET` | SCTP | A connection-oriented sequenced-packet socket that preserves message boundaries and delivers messages in the order sent. |

**POSIX / Operating System Behavior**

- The **UDS facility** is a standard component of a POSIX operating system.
- The API for a UDS is similar to that of an Internet socket.
- Instead of using an underlying network protocol, communication takes place entirely within the operating system kernel.
- A UDS may use the file system as its address namespace.
- Some operating systems, such as Linux, provide additional namespaces.
- Processes refer to a UDS through a file system inode, allowing two processes to communicate by opening the same socket.

**File Descriptor Passing**

- In addition to sending data, processes can pass file descriptors over a UDS connection by using the `sendmsg()` and `recvmsg()` system calls.
- This allows one process to grant another process access to a file descriptor that it would not otherwise be able to use.\[2\]\[3\]
- This can be used to implement a rudimentary form of capability-based security.

#### Understanding Unix Sockets

In the intricate web of digital communication, few tools are as fundamental yet versatile as Unix sockets in Linux. These Unix sockets act as unsung heroes, enabling data exchange between programs, processes, and even remote machines.

Whether you're a seasoned programmer architecting complex systems or a curious tinkerer delving into the Linux underbelly, understanding Unix sockets is a valuable skill.

**What are Unix Sockets?**

- Imagine a two-way tunnel, not of dirt but of data, built within the digital terrain of your Linux system.
- That's essentially what a Unix socket represents.
- It's a software endpoint facilitating bidirectional communication between processes, regardless of their location within the system or even beyond its borders.

**Unix sockets offer two distinct flavors**

| **Socket flavor** | **Role** | **Mental model** |
| --- | --- | --- |
| **Network Sockets** | Enable communication across networks using protocols like TCP/IP. | Long-distance runners sending data packets across the internet, each bearing the socket's address as its destination. |
| **Domain Sockets** | Facilitate communication between processes within the same system. | Private pipes connecting programs within the Linux kingdom. |

**Building Blocks of Socket Communication**

Each Unix socket comprises several crucial elements:

- **Domain**: Specifies the communication protocol, like `AF_INET` for TCP/IP or `AF_UNIX` for domain sockets.
- **Type**: Defines the communication style, like `SOCK_STREAM` for reliable byte streams or `SOCK_DGRAM` for unreliable datagrams.
- **File Descriptor**: The unique identifier assigned to the socket, used for accessing and manipulating it.
- **Address**: Identifies the socket, either as an IP address and port for network sockets or a path on the filesystem for domain sockets.

**Socket Types and Their Roles**

| **Socket type** | **Role** | **Analogy** |
| --- | --- | --- |
| `SOCK_STREAM` | Reliable, sequential data transmission. | Flowing rivers, where each byte arrives in the correct order, much like the pages of a book. |
| `SOCK_DGRAM` | Individual packets carry the data. Faster, but without guaranteed delivery and order. | Mail delivery, like postcards mailed independently. |

**Connection and Exchange of Data Using Sockets**

Establishing a connection involves a four-step dance:

- **Socket Creation**: Use the `socket()` system call to define the socket's domain, type, and protocol.
- **Binding**: Assign an address to the socket using the `bind()` call. Think of hanging a sign with your address on the digital street.
- **Connecting (Server) or Listening (Client)**: Servers use `listen()` to wait for incoming connections, while clients use `connect()` to initiate communication with a designated server address.
- **Data Transfer**: Once connected, use `send()` and `recv()` to send and receive data through the established socket channel.

**Socket Programming Tools**

Several libraries and frameworks assist in harnessing the power of sockets:

- **Berkeley Sockets Interface (BSI)**: The foundational API for socket programming in C, providing low-level functions for socket creation, connection management, and data transfer.
- **GNU glibc Networking API**: Builds upon BSI, offering a higher-level interface with additional features like DNS resolution and network address manipulation.
- **Python Sockets Module**: Simplifies socket programming with a dedicated module, allowing concise and intuitive code for network and inter-process communication.

**Applications of Unix Sockets**

The versatility of Unix sockets makes them ubiquitous across various domains:

- **Web Servers**: Web servers like Apache utilize sockets to communicate with browsers, serving web pages and handling client requests.
- **Networking Tools**: Tools like `ping` and `netcat` rely on sockets to send and receive network packets, troubleshooting connectivity and performing network diagnostics.
- **Inter-Process Communication (IPC)**: Programs within the same system leverage domain sockets to exchange data efficiently, eliminating the need for external network connections.
- **Containerized Applications**: Containers rely on sockets for internal communication between containerized processes and for interacting with the host system.

**Security Concerns of Using Sockets**

- With great power comes great responsibility.
- Securing your socket communication is paramount.
- **Authentication and Authorization**: Implement mechanisms to verify the identity of communicating parties.

**Conclusion**

- Unix sockets are like digital tunnels that let different programs talk to each other, whether they're on the same computer or far away.
- They're useful for making web servers, networking tools, and containerized applications work smoothly.
- Understanding them is like having a secret code to make different parts of your computer share information.
- Whether you're a tech pro or just curious, Unix sockets are a cool tool to have in your digital toolbox.

[Back to Contents](#contents)

---

<a id="server-side-rendering-ssr"></a>

### Server-Side Rendering (SSR)

Server-Side Rendering (SSR) in web applications dynamically generates full HTML on the server for each request, sending a ready-to-display page to the browser to improve SEO and initial load performance. It is widely used in modern frameworks like Next.js, Nuxt, or Angular to balance fast, initial content delivery with client-side interactivity.

**Key Aspects of SSR Applications**

- **Process**: When a user requests a page, the server uses a runtime (like Node.js) to execute JavaScript, fetch data, and generate HTML, which is then sent to the browser.
- **Hydration**: After the browser receives the HTML, it downloads the JavaScript, allowing the application to become interactive (attaching event listeners).
- **Performance & SEO**: SSR improves perceived speed because users see content faster, and it enhances SEO because search engines can crawl fully rendered pages.
- **Frameworks**: Popular frameworks to implement SSR include Next.js (React), Nuxt (Vue), and Angular.
- **Use Cases**: Highly dynamic content, public-facing websites needing high SEO, and applications where fast initial load time is critical.

**SSR vs. Other Methods**

- **CSR (Client-Side Rendering)**: Loads a blank shell and renders content in the browser, which can be slower to load initially.
- **SSG (Static Site Generation)**: Pre-renders pages at build time rather than at request time.

[Back to Contents](#contents)

---

<a id="grpc"></a>

### gRPC

**gRPC** is a high-performance, open-source Remote Procedure Call (RPC) framework initially developed by Google, designed to connect services efficiently within and across data centers. It uses HTTP/2 for transport and Protocol Buffers (Protobuf) as the interface definition language, offering strong typing, code generation, and low-latency communication.

**Key features and benefits of gRPC include**:

- **High Performance**: Uses compact binary serialization (Protobuf), resulting in smaller, faster messages compared to JSON/XML.
- **Streaming**: Supports four types of communication: unary (single request/response), server streaming, client streaming, and bidirectional streaming.
- **Contract-First**: API contracts are strictly defined in .proto files, which are used to generate client and server code for multiple languages, ensuring type safety.
- **Modern Transport**: Leverages HTTP/2 for features like multiplexing requests over a single connection and bidirectional streaming.
- **Use Cases**: Highly effective for microservices, mobile applications, and real-time communication systems where low latency is critical.
- **gRPC vs REST**: While REST is standard for public APIs using JSON, gRPC is often preferred for internal microservices due to its superior performance, strict contract definitions, and streaming capabilities.

[Back to Contents](#contents)

---

<a id="grpc-vs-websocket-vs-http-vs-server-sent-events-sse"></a>

### gRPC vs. WebSocket vs. HTTP vs. Server-Sent Events (SSE)

Choosing between gRPC, WebSocket, HTTP (and the architectural style REST), and Server-Sent Events (SSE) depends entirely on your application's communication needs regarding directionality, performance, and browser compatibility.

| **Feature** | **REST (over HTTP)** | **gRPC** | **WebSocket** | **SSE** |
| --- | --- | --- | --- | --- |
| **Communication** | **Request-Response (Client-initiated)** | **RPC (Function/Method call)** | **Full-duplex (Bidirectional)** | **Unidirectional (Server-to-Client)** |
| **Protocol** | **HTTP/1.1 (mostly), HTTP/2** | **HTTP/2** | **WebSocket (upgrades from HTTP/1.1)** | **HTTP/1.1 or HTTP/2** |
| **Data Format** | **JSON (primarily), XML** | **Protocol Buffers (binary, compact)** | **Flexible (JSON, binary, etc.)** | **UTF-8 text (event-stream format)** |
| **Performance** | **Moderate (higher latency/overhead)** | **Very high (low latency, efficient)** | **High (low latency after handshake)** | **Moderate (persistent connection)** |
| **Browser Support** | **Excellent (native)** | **Requires proxy (gRPC-Web)** | **Excellent (native)** | **Excellent (native, built-in re-connection)** |
| **Primary Use Case** | **Public APIs, CRUD operations** | **Internal microservices, high-perf systems** | **Real-time chat, gaming, interactive apps** | **Live feeds, stock tickers, notifications** |

**Detailed Breakdown**

**REST (Representational State Transfer) over HTTP**:

- **Pros**: Widely supported, simple, easy to debug, and excellent browser compatibility. It is the standard for generic public APIs.
- **Cons**: High overhead and latency due to stateless, request-response cycles and verbose JSON payloads.
- **When to use**: For standard CRUD operations and public-facing APIs where simplicity and broad compatibility are more important than raw speed.

**gRPC (Google Remote Procedure Call)**:

- **Pros**: Extremely high performance, low latency, and efficient due to binary serialization (Protocol Buffers) and the use of HTTP/2. It supports various streaming patterns (unary, server, client, bidirectional) and uses a contract-first design for strong typing and code generation.
- **Cons**: Not browser-native (requires a proxy like gRPC-Web) and harder to debug due to its binary format.
- **When to use**: Ideal for efficient, high-throughput internal microservice communication and multi-language environments.

**WebSockets**:

- **Pros**: Provides a persistent, full-duplex (bidirectional) communication channel over a single TCP connection, enabling true real-time interaction with minimal overhead after the initial HTTP handshake.
- **Cons**: Lacks a built-in schema (developer manages message structure), and automatic reconnection logic must be manually implemented.
- **When to use**: For applications requiring continuous, low-latency, two-way communication, such as online games, live chat, and collaborative editing tools.

**SSE (Server-Sent Events)**:

- **Pros**: Simpler than WebSockets, uses standard HTTP, and has built-in automatic reconnection logic in the browser. It is perfect for server-to-client unidirectional data flow.
- **Cons**: Only allows data to flow from the server to the client (unidirectional).
- **When to use**: When you only need to push updates from the server to the client, such as for live stock prices, news feeds, LLM tokens streaming to UI, or system notifications.

[Back to Contents](#contents)

---

<a id="webhooks"></a>

### Webhooks

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-012.png)

**A webhook** is a way for one app to automatically send real-time data to another app when something happens.

🧠 **Simple idea**

Instead of constantly asking “Did anything happen?” (like refreshing a page), a webhook says:

```text
👉 “When this event happens, I’ll notify you instantly.”
```

🔄 **How it works (step-by-step)**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-013.png)

- **You give a service a URL (your webhook endpoint)**
- **Something happens in that service (event)**
- **Example**: a payment is completed
- **The service sends an HTTP request (usually POST) to your URL**
- **Your app receives the data and reacts**

📦 **Real-world examples**

- 💳 **Payment apps (like Stripe)**: notify you when a payment succeeds
- 📧 **Email tools**: tell you when someone opens an email
- 🛒 **Online stores**: send updates when an order is placed
- 💬 **Chat apps**: trigger bots when messages arrive (WhatsApp)

🔁 <u>**Webhook vs API (important difference)**</u>

| **Feature** | **Webhook** | **API** |
| --- | --- | --- |
| **Data flow** | **Push (automatic)** | **Pull (you request it)** |
| **Timing** | **Real-time** | **Whenever you ask** |
| **Efficiency** | **Saves resources** | **Can require frequent checking** |

👉 **Think of it like this**:

- **API = you keep asking “Anything new?”**
- **Webhook = you get a message “Hey, something happened!”**

🧩 **Simple analogy**

- **A webhook is like a doorbell 🔔**
- **You do not keep opening the door to check**
- **You wait until someone rings it**

[Back to Contents](#contents)

---

<a id="openapi-schemas-api-data-models-dto-data-transfer-objects"></a>

### OpenAPI Schemas: API data models / DTO (Data Transfer Objects)

**What are these schemas in OpenAPI?**

- **Short answer**: they are NOT database schemas.

These are OpenAPI schemas, which define the API contract — the structure of JSON requests and responses exchanged between services.

**Their correct technical name is**:

- 👉 **API schemas / DTOs (Data Transfer Objects)**

**They describe**:

- **which fields exist in a request or response**
- **which fields are required**
- **data types and formats**
- **example values**

**For example**:

```text
RecommendationResponse
ProductRecommendation
ErrorResponse
HealthResponse
```

*These describe HTTP payloads, not database tables.*

**How OpenAPI schemas differ from database schemas**

| **OpenAPI Schema** | **Database Schema** |
| --- | --- |
| **Defines JSON structures** | **Defines tables / collections** |
| **Used for service-to-service contracts** | **Used for data persistence** |
| **Can aggregate data from many sources** | **Usually normalized** |
| **May include computed or generated fields** | **Mostly stored fields** |
| **Designed for consumers (API clients)** | **Designed for storage efficiency** |

**Concrete example**

```text
ProductRecommendation:
  product_id
  name
  price
  score
  reason
```

**In a real system**:

- **score is typically computed at runtime**
- **reason is generated by an LLM**
- **price may come from a pricing service**
- **category from the product catalog**
- **in\_stock from inventory**

👉 The API schema is a presentation layer, not a storage model.

**Why an Architect defines these schemas**

**An architect is responsible for**:

- **defining clear API contracts between Backend and AI services**
- **ensuring backward compatibility**
- **documenting what the consumer can rely on**

**An architect is not required to**:

- **define SQL / Mongo / ClickHouse schemas**
- **decide how data is physically stored**

*Those belong to internal implementation details.*

**Typical system layering**

```text
[ Database Schemas ]
        ↓
[ Domain Models ]
        ↓
[ Scoring / LLM / Aggregation ]
        ↓
[ OpenAPI Schemas ]  ← THESE
        ↓
[ Backend / Frontend Consumers ]
```

**Can you infer the database from this?**

**Only partially**:

- **it will be incomplete**
- **it will be inaccurate**
- **many fields do not exist in storage at all**

**In practice**:

```text
Database → Domain → API
not the other way around.
```

**Final takeaway**

```text
✔ These are API schemas
✔ They define data exchange contracts
❌ They are not database schemas
❌ They are not ER models
```

[Back to Contents](#contents)

---

<a id="green-api"></a>

### Green-API

**Green-API (often written as Green-API)** is a third-party API service for WhatsApp. It lets developers connect apps, bots, or backends to WhatsApp so they can send and receive messages programmatically—without building everything from scratch.

**What Green-API is used for**

**People use Green-API to**:

- 🤖 **build WhatsApp bots**
- 📩 **send automated notifications (orders, reminders, OTPs)**
- 🧑‍💼 **run customer support over WhatsApp**
- 🔄 **integrate WhatsApp with CRMs, websites, or internal tools**

**Basically**: if you want your software to talk on WhatsApp, Green-API is one way to do it.

**How it works (high level)**

- **You create an instance in Green-API**
- **You link a WhatsApp number (often via QR code)**
- **Your app uses HTTP requests to send/receive messages**
- **Incoming messages are delivered to you via webhooks**

*Your code never “opens WhatsApp” — it just talks to the API.*

**What you can do with it**

**Typical features include**:

- **Send text, images, files**
- **Receive messages & replies**
- **Manage chats and contacts**
- **Get message status (sent / delivered / read)**
- **Work with group chats (limited, depends on plan)**

**Green-API vs official WhatsApp Business API**

**Green-API**

- **Easier & faster to set up**
- **Cheaper for small projects**
- **Popular for startups and experiments**
- ⚠️ **Not an official Meta product**

**Official WhatsApp Business API**

- **Fully official & compliant**
- **Required for large businesses**
- **More setup, approvals, and cost**

*So Green-API is often used when you want speed and flexibility.*

**Who it’s best for**

- **Solo devs & students**
- **Small businesses**
- **Hackathons & prototypes**
- **Automation and bots**

[Back to Contents](#contents)

---

<a id="real-time-streaming-protocol"></a>

### Real-Time Streaming Protocol

**The Real-Time Streaming Protocol (RTSP)** is an application-level network protocol designed for multiplexing and packetizing multimedia transport streams (such as interactive media, video and audio) over a suitable transport protocol. RTSP is used in entertainment and communications systems to control streaming media servers. The protocol is used for establishing and controlling media sessions between endpoints. Clients of media servers issue commands such as play, record and pause to facilitate real-time control of the media streaming from the server to a client (video on demand) or from a client to the server (voice recording).

[Back to Contents](#contents)

---

<a id="simple-mail-transfer-protocol"></a>

### Simple Mail Transfer Protocol

**The Simple Mail Transfer Protocol (SMTP)** is an Internet standard communication protocol for electronic mail transmission. Mail servers and other message transfer agents use SMTP to send and receive mail messages. User-level email clients typically use SMTP only for sending messages to a mail server for relaying, and typically submit outgoing email to the mail server on port 465 or 587 per RFC 8314. For retrieving messages, IMAP (which replaced the older POP3) is standard, but proprietary servers also often implement proprietary protocols, e.g., Exchange ActiveSync.

SMTP's origins began in 1980, building on concepts implemented on the ARPANET since 1971. It has been updated, modified and extended multiple times. The protocol version in common use today has extensible structure with various extensions for authentication, encryption, binary data transfer, and internationalized email addresses. SMTP servers commonly use the Transmission Control Protocol on port number 25 (between servers) and 587 (for submission from authenticated clients), both with or without encryption, and 465 with encryption for submission.

[Back to Contents](#contents)

---

<a id="backend-for-frontend-pattern-bff"></a>

### Backend for Frontend Pattern (BFF)

https://www.geeksforgeeks.org/system-design/backend-for-frontend-pattern/

The Backend for Frontend (BFF) Pattern addresses the challenge of serving diverse client applications by creating a dedicated backend for each type of client—such as web, mobile, or IoT. Instead of a single backend trying to cater to all, BFF allows tailored APIs that optimize performance, reduce client-side complexity, and improve the user experience. This architecture ensures each client receives only the data and functionality it needs, enhancing efficiency and scalability across platforms.

[Back to Contents](#contents)

---

<a id="cloud-containers-platform-infrastructure"></a>

## Cloud, Containers & Platform Infrastructure

<a id="k8s-kubernetes"></a>

### K8s (Kubernetes)

**K8s (Kubernetes)** is an open-source platform for automating the deployment, scaling, and management of containerized applications, originally developed by Google and now maintained by the CNCF. It orchestrates containers into logical units (pods) for efficient resource use, offering features like automated rollouts, self-healing, service discovery, and load balancing to simplify complex application management across diverse infrastructures.

**Key Functions**

- **Container Orchestration**: Manages the lifecycle of containers (like Docker) across clusters of machines.
- **Automated Deployments**: Handles rolling out new versions and rolling back if issues arise.
- **Scaling**: Automatically scales applications up or down based on demand.
- **Self-Healing**: Restarts failed containers, replaces dead ones, and reschedules them.
- **Service Discovery & Load Balancing**: Routes traffic to healthy containers.

**How it Works**

- **Cluster**: A group of machines (nodes).
- **Pods**: The smallest deployable units, hosting one or more containers.
- **Control Plane**: Manages the cluster's state (master nodes).
- **Worker Nodes**: Run the application containers.

**The Name "K8s"**

- It's a numeronym (a word shortened with numbers).
- The '8' represents the eight letters between the 'K' and the 'S' in "Kubernetes".

**Here’s the short, clean explanation most people use**:

**Cluster**
- A cluster is the whole Kubernetes system — a group of machines working together to run your applications. It includes the control plane + all worker machines.

**Node**
- A node is a single machine (virtual or physical) inside the cluster.
- It provides CPU, memory, and storage, and it’s where your applications actually run.

**Pod**
- A pod is the smallest deployable unit in Kubernetes.
- It wraps one or more containers that:

- **run together**

- **share the same IP**

- **share storage**

- **scale and live as a unit**

<u>**One-line mental model (very useful)**</u>

- Cluster = fleet, Node = vehicle, Pod = delivery box inside the vehicle

**Or even simpler**:

- Kubernetes runs pods on nodes inside a cluster.

<a id="why-kubernetes-uses-pods-not-just-containers"></a>

#### Why Kubernetes uses pods (not just containers)

**Because some containers must always run together.**

**A pod lets Kubernetes**:

- **Treat multiple containers as one unit**

- **Give them the same IP, ports, and storage**

- **Start, stop, scale, and move them together**

**Typical example**

- **Main app container**

- **Sidecar container (logging, proxy, metrics)**

*They depend on each other, so Kubernetes groups them into one pod.*

**One-liner**

- A pod exists so tightly coupled containers can behave like a single application.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-014.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-015.png)

<a id="why-architects-usually-avoid-drawing-nodes"></a>

#### Why architects usually avoid drawing nodes

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-016.png)

**Short answer first**

- ❌ **You do NOT explicitly show nodes in this diagram**

- ✅ **That is OK and normal for this level**

- ❌ **Pods are NOT implied to be one pod = one node**

- ✅ **Pods are scheduled dynamically onto nodes by Kubernetes**

*The diagram is cluster-level, not node-level.*

**What is missing vs what is implied**

**1. Do you have nodes in this diagram?**

**Implicitly yes, explicitly no.**

**You show**:

```text
Cloud Infrastructure
└── Kubernetes Cluster
    ├── Frontend Pods
    ├── Worker Pods
    ├── Gateway Pods
    ├── AI Service Pods
```

*Behind that box, Kubernetes always has nodes, but you didn’t draw them.*

**That means**:

```text
“Kubernetes manages nodes; this diagram focuses on workloads.”
```

*This is perfectly acceptable.*

**2. Are pods mapped 1**: 1 to nodes?

**Absolutely not.**

**In Kubernetes**:

- **A node can run many pods**

- **A pod runs on exactly one node**

- **Pods are scheduled dynamically**

**So, in reality, it might look like**:

```text
Node A: Frontend Pod, Gateway Pod
Node B: AI Service Pod, Redis client sidecar
Node C: Worker Pod
Node D: Worker Pod
```

**But**:

- **You do not decide this in the diagram**

- **Kubernetes decides this at runtime**

**Why your diagram is still correct**

**Your diagram answers**:

- **What runs in Kubernetes?**

- **How services talk to each other?**

- **Where data flows**

- **What is cloud vs on-prem**

- **What is managed vs self-hosted**

**It does not answer**:

- **How many machines exist**

- **Which pod is on which machine**

- **CPU / memory placement**

*And that’s by design.*

**Levels of abstraction (this is the key)**

**Level you are using → Application / Deployment level**

**You show**:

- **Pods**

- **Services**

- **Databases**

- **External systems**

**You hide**:

- **Nodes**

- **Autoscaling groups**

- **VM details**

✔️ This is what architects usually present.

**When DO you show nodes?**

**You only show nodes if you need to discuss**:

- **GPU vs CPU nodes**

- **Cost optimization**

- **Isolation / security boundaries**

- **Capacity planning**

- **Failure domains**

**Example (optional, separate diagram)**:

```text
Kubernetes Cluster
├── GPU Node Pool
│   └── AI Service Pods
├── General Node Pool
│   ├── Frontend Pods
│   ├── Gateway Pods
│   └── Worker Pods
```

**That’s a different diagram, often called**:

**Infra view**

**Runtime topology**

**Capacity diagram**

**Why architects usually avoid drawing nodes**

**Because nodes are**:

- **Elastic**
- **Auto-scaled**
- **Replaced automatically**
- **Implementation detail**

**If you draw nodes too early**:

- **People assume static machines**
- **Stakeholders misunderstand scaling**
- **Diagram becomes outdated quickly**

<u>**Very important Kubernetes rule (remember this)**</u>

- Architects design for pods.
- Kubernetes is responsible for nodes.

*Unless you have special constraints (GPU, compliance, isolation).*

**How to answer if someone asks you this**

**You can confidently say**:

```text
“Nodes are intentionally abstracted away. This diagram shows workloads at the Kubernetes cluster level. Pods are scheduled dynamically across nodes, which may change at runtime.”
```

*That is a correct, professional answer.*

<u>**Final mental model**</u>

- **Cluster → shown explicitly**
- **Nodes → exist, but hidden**
- **Pods → logical deployment units**
- **Scheduling → Kubernetes’ responsibility**

<a id="cluster-in-deployment-diagrams-doesnt-always-mean-kubernetes-cluster"></a>

#### “Cluster” in deployment diagrams doesn’t always mean Kubernetes cluster

<u>**This is an excellent architecture question — you’re noticing an important overloaded term**</u>: “cluster” does not always mean Kubernetes cluster.

*Let’s clarify cleanly and precisely.*

**Short answer**

**No** — the other “clusters” in your diagram are **NOT** Kubernetes clusters.
They are **technology-specific clusters**, managed independently of Kubernetes.

**Only one thing in your diagram is a Kubernetes cluster**:

- **Kubernetes Cluster (k8s 1.28)**

*All the others use the word cluster in a different sense.*

**Why this is confusing (but normal)**

“**Cluster” simply means**:

```text
Multiple instances of something working together for scalability, availability, or performance.
```

*Different technologies use the word for different layers.*

**Let’s go through each “cluster” in your diagram**

**1. ✅ Kubernetes Cluster (special, control-plane–based)**

**This is the orchestration cluster.**

**It provides**:

- **Scheduling**
- **Autoscaling**
- **Self-healing**
- **Networking**
- **Secrets/config management**

**It runs**:

- **Frontend Pods**
- **Gateway Pods**
- **Worker Pods**
- **AI Service Pods**

**This is the only Kubernetes cluster in the diagram.**

**2. ❌ Redis Cluster (AWS ElastiCache)**

**Not Kubernetes**

**This is a data service cluster**

**Managed by AWS**

**Internally consists of**:

- **Multiple Redis nodes**
- **Replication**
- **Sharding (optional)**
- **Failover**

**From Kubernetes’ perspective**:

- **It’s just an external service**
- **Accessed via a hostname + port**

**Even though it’s called a “cluster”, it**:

- **Has no pods**
- **Has no kube-scheduler**
- **Has no control plane**

**3. ❌ Vector DB Cluster (Qdrant Cloud)**

**Also, not Kubernetes (from your system’s point of view)**

**It’s a logical database cluster**

**Designed for**:

- **Horizontal scaling**
- **High availability**
- **Partitioning embeddings**

**Internally**:

- **Qdrant may run on Kubernetes**
- **Or on VMs**
- **Or hybrid**

*But that’s their internal concern, not yours.*

**Architecturally**:

It is an **external managed service**, not part of your Kubernetes cluster.

**4. ❌ Elasticache / MySQL / RDS “clusters”**

**Same story**:

**These are service-level clusters**

**Implemented using**:

- **Replicas**
- **Primary/secondary nodes**
- **Failover logic**

**Exposed as**:

- **A stable endpoint**

They are **not orchestrators**, just distributed systems.

**So why does Kubernetes get a dedicated “Cluster” box?**

Because **Kubernetes is a platform, not just a component.**

**Kubernetes cluster responsibilities**

- **Runs your application code**
- **Controls pod lifecycle**
- **Enforces deployment strategies**
- **Handles scaling and recovery**

**All other “clusters”**:

- **Do not run your workloads**
- **Do not orchestrate anything**
- **Do not control deployments**

**That’s why Kubernetes is drawn as**:

- **An enclosing container**
- **A first-class runtime boundary**

<u>**Correct mental model (very important)**</u>

**Think in layers**:

- **Orchestration layer**
  Kubernetes cluster ← only one here
- **Application layer**
  Pods, services, APIs
- **Data / platform layer**
  Redis cluster
  Vector DB cluster
  MySQL/RDS cluster

*They all say “cluster”, but they live at different abstraction layers.*

**One sentence you can confidently use in reviews**

“Only the Kubernetes cluster is an orchestration cluster; the other clusters are technology-specific distributed services and are external dependencies from Kubernetes’ perspective.”

*That is architecturally precise.*

**Optional improvement (if you want clarity)**

**If you want to avoid confusion in future reviews, you can rename boxes slightly**:

- **Redis Cluster → Managed Redis Service**
- **Vector DB Cluster → Vector Database Service**
- **Kubernetes Cluster → Application Runtime (Kubernetes)**

*Same architecture — clearer semantics.*

**Final takeaway**

- “**Cluster” ≠ “Kubernetes cluster”**
- **Kubernetes is special because it orchestrates**
- **Other clusters store or process data**

<u>**Your diagram is correct — this is a terminology issue, not a design flaw.**</u>

<a id="eks-amazon-elastic-kubernetes-service"></a>

#### EKS (Amazon Elastic Kubernetes Service)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-017.png)

EKS stands for Amazon Elastic Kubernetes Service, a managed service from Amazon Web Services (AWS) that simplifies running Kubernetes for deploying, managing, and scaling containerized applications on AWS and on-premises, handling the complex Kubernetes control plane, offering high availability, security, and integration with other AWS services, allowing users to focus on their applications rather than infrastructure management. It's certified Kubernetes-conformant, ensuring compatibility with existing tools, and features like EKS Auto Mode further automate cluster management, cost optimization, and security.

**Key Aspects of EKS**:

- **Managed Control Plane**: AWS manages the Kubernetes control plane (scheduling, availability, data storage) across multiple Availability Zones, providing reliability and eliminating single points of failure.
- **Scalability & Reliability**: Leverages AWS infrastructure for high performance, scale, and availability for production workloads.
- **Kubernetes Conformant**: Supports standard Kubernetes, allowing easy migration of applications from other Kubernetes environments.
- **Hybrid & Multi-Cloud**: Can run containers on Amazon EC2, AWS Fargate, and even on-premises or edge locations.
- **Automation & Optimization**: Features like EKS Auto Mode automate node provisioning, scaling, patching, and cost optimization.
- **Security & Integration**: Integrates with AWS security services (IAM, VPC) and provides tools for monitoring and observability.

**Common Uses**:

- Deploying microservices and cloud-native applications.
- Modernizing existing applications.
- Running AI/ML workloads.
- Managing containerized applications at scale.

**Others are**:

- **Azure Kubernetes Service (AKS)**
- Google Kubernetes Engine (GKE).

<a id="helm-in-kubernetes"></a>

#### Helm in Kubernetes

**In Kubernetes, Helm** is basically the package manager for Kubernetes — think apt, yum, or npm, but for K8s 🚢⚓

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-018.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-019.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-020.png)

**What Helm does (in plain terms)**

Helm helps you define, install, and manage Kubernetes applications as a single unit, instead of juggling dozens of YAML files by hand.

**Without Helm**:

- **You apply many YAML files manually**
- **Updating configs is error-prone**
- **Rollbacks are painful 😵**

**With Helm**:

- **Apps are bundled, versioned, and reusable**

- **Deployments are repeatable and customizable**

- **Upgrades and rollbacks are one command**

**Key concepts you should know**

**Helm Chart**

**A chart is a package that contains**:

- **Kubernetes manifests (Deployments, Services, ConfigMaps, etc.)**

- **Default configuration values**

- **Metadata about the app**

📦 Think of a chart as a blueprint for an app.

**Values**

- **Configurable parameters for a chart**

- **Stored in values.yaml**

- **Can be overridden per environment (dev/staging/prod)**

**Example**:

```yaml
replicaCount: 3
image:
  repository: nginx
  tag: 1.25
```

**Release**

- **A running instance of a chart in a cluster**

- **Each install creates a new release**

- **You can have multiple releases of the same chart**

**What problems Helm solves**

| **Problem** | **How Helm helps** |
| --- | --- |
| **Too many YAML files** | **Packages them into one chart** |
| **Environment differences** | **Uses values files** |
| **App upgrades** | **helm upgrade** |
| **Rollbacks** | **helm rollback** |
| **Reuse** | **Share charts via repositories** |

**Typical Helm workflow**

- **Write or download a chart**

- **Configure values**

- **Install it into the cluster**

- **Upgrade or rollback when needed**

**Commands**:

- **helm install my-app ./chart**

- **helm upgrade my-app ./chart**

- **helm rollback my-app 1**

**Helm vs plain Kubernetes YAML**

**Kubernetes**: what resources exist

- **Helm**: how they’re packaged, configured, and versioned

*Helm does not replace Kubernetes — it organizes and automates how you deploy to it.*

**When Helm is especially useful**

- **Microservices apps**

- **CI/CD pipelines**

- **Multi-environment deployments**

- **Shared platform tooling (databases, ingress, monitoring)**

<a id="yaml"></a>

#### YAML

**YAML ("YAML Ain't Markup Language")** is a human-readable data serialization language frequently used for configuration files, widely adopted in DevOps tools like Docker, Kubernetes, and Ansible. It utilizes indentation, key-value pairs, and sequences (lists) for structure, serving as a strict superset of JSON while allowing comments and simpler syntax.

**Key Features and Structure**

- **Indentation-based**: Uses spaces (not tabs) for nesting and structuring data.

- **Data Types**: Supports scalars (strings, integers, booleans, nulls), sequences (lists), and mappings (dictionaries/objects).

- **Key-Value Pairs**: Defines data as key: value.

- **Comments**: Allows comments using the # character.

**Common Use Cases**

- **Configuration**: Used in Kubernetes (k8s), Docker Compose, and Ansible playbooks.

- **CI/CD**: Pipelines for GitHub Actions and GitLab.

- **Data Serialization**: As a more readable alternative to JSON or XML.

**Syntax Example**

```yaml
# A simple configuration file
app:
  name: "My App"
  port: 8080
  enabled: true
  tags:
    - web
    - production
```

**YAML vs. JSON vs. XML**

YAML is designed for readability; it is a superset of JSON, meaning it can handle any JSON file.

*JSON is faster for machine parsing.*

*XML is more verbose and generally harder to read.*

**Best Practices**

- **Use spaces, not tabs.**

<u>**Use yaml.safe\_load() in Python to prevent execution of arbitrary code.**</u>

OpenAPI documents, which describe REST APIs, are commonly written in YAML due to its human-readable nature and support for multiline descriptions. The specification itself is a JSON object, and YAML is a superset of JSON, making the two formats interchangeable for defining an API.

**Basic Structure of an OpenAPI YAML File**

An OpenAPI YAML file generally follows a required structure and should be named openapi.yaml or openapi.yml. A basic structure includes:

- **openapi**: Specifies the version of the OpenAPI Specification being used (e.g., 3.1.0).

- **info**: Provides metadata about the API, including the title, description, and version.

- **servers**: (Optional but recommended) Defines the base URLs for the API endpoints.

- **paths**: The core of the document, which defines the available API endpoints and the operations (GET, POST, etc.) on each endpoint.

- **components**: (Optional) Used to define reusable data structures (schemas), parameters, security schemes, and other objects.

**Sample OpenAPI 3.0 YAML**

**A simple, structured openapi.yaml file for a "Sample API" might look like this**:

```yaml
openapi: 3.0.4
info:
  title: Sample API
  description: Optional multiline or single-line description in [CommonMark](http://commonmark.org/help/) or HTML.
  version: 0.1.9
servers:
  - url: http://api.example.com/v1
    description: Optional server description, e.g. Main (production) server
paths:
  /users:
    get:
      summary: Returns a list of users.
      responses:
        '200':
          description: A JSON array of user names.
          content:
            application/json:
              schema:
                type: array
                items:
                  type: string
```

<a id="kubernetes-cluster-installation-bootstrapping-tools"></a>

#### Kubernetes Cluster Installation & Bootstrapping Tools

**Here** is a comprehensive overview of the main tools used to install and bootstrap Kubernetes clusters. I will group them so it is easier to understand when and why each is used.

🧱 **Official Kubernetes Installation Tools**

**1. kubeadm**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-021.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-022.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-023.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-024.png)

- **Type**: Official CLI tool
- **Best for**: Manual cluster setup, learning, custom environments
- **Maintained by**: Kubernetes
- **What it does**:
  - **Bootstraps a cluster (kubeadm init)**
  - **Adds nodes (kubeadm join)**
  - **Leaves configuration mostly to you**
- **Pros**:
  - **Flexible and transparent**
  - **Great for learning Kubernetes internals**
- **Cons**:
  - **Requires manual setup (networking, storage, etc.)**

**2. minikube**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-025.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-026.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-027.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-028.png)

- **Type**: Local single-node cluster
- **Best for**: Development and testing
- **What it does**:
  - **Runs Kubernetes locally (VM or container)**
  - **Includes addons like dashboard, ingress**
- **Pros**:
  - **Easy to install and use**
  - **Works offline**
- **Cons**:
  - **Not production-ready**

**3. kind (Kubernetes IN Docker)**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-029.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-030.png)

- **Type**: Local cluster using Docker
- **Best for**: CI/CD, testing
- **What it does**:
  - **Runs Kubernetes nodes as Docker containers**
- **Pros**:
  - **Lightweight and fast**
  - **Great for automated testing**
- **Cons**:
  - **Not for production**

☁️ **Managed Kubernetes Installers (Cloud)**

**4. Cloud Provider Services**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-031.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-032.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-033.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-034.png)

**These are not “tools” you install manually but fully managed platforms**:

- **Amazon EKS**
- **Google Kubernetes Engine**
- **Azure Kubernetes Service**

**Pros**:

- **No control plane management**
- **Auto-scaling, upgrades handled**

**Cons**:

- **Less control**
- **Costs money**

⚙️ **Infrastructure & Automation Tools**

**5. kops**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-035.gif)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-036.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-037.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-038.png)

- **Best for**: Production clusters on AWS
- **Creates, upgrades, deletes clusters**

**6. Kubespray**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-039.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-040.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-041.png)

- **Built with**: Ansible
- **Best for**: Bare metal or cloud
- **Features**:
  - **Highly configurable**
  - **Supports multiple platforms**

**7. Rancher (RKE / RKE2 / K3s)**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-042.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-043.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-044.jpeg)

- **From**: Rancher Labs
- **Includes**:
  - **RKE – production clusters**
  - **RKE2 – hardened Kubernetes**
  - **K3s – lightweight Kubernetes**
- **Best for**:
  - **Edge computing (K3s)**
  - **Enterprise cluster management**

**8. MicroK8s**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-045.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-046.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-047.jpeg)

- **From**: Canonical
- **Best for**: Lightweight local or edge setups
- **Pros**:
  - **Simple install (snap install)**
  - **Production-capable in small setups**

🧩 **Other Notable Tools**

**9. k3d**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-048.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-049.png)

- **Runs K3s inside Docker**
- **Faster than minikube in many cases**

**10. kubeone**

- **Focused on production clusters**
- **Often used with Terraform**

**11. Cluster API (CAPI)**

- **Kubernetes-native cluster lifecycle management**
- **Uses CRDs to manage clusters**

**12. Talos Linux**

- **Minimal OS designed for Kubernetes**
- **Declarative cluster setup**

**13. OpenShift Installer**

- **From Red Hat**
- **Enterprise Kubernetes distribution**

🧭 **Quick Summary Table**

| **Tool** | **Use Case** | **Complexity** | **Production** |
| --- | --- | --- | --- |
| **kubeadm** | **Manual setup** | **Medium** | **✅** |
| **minikube** | **Local dev** | **Easy** | **❌** |
| **kind** | **Testing/CI** | **Easy** | **❌** |
| **kops** | **AWS clusters** | **Medium** | **✅** |
| **Kubespray** | **Multi-platform** | **High** | **✅** |
| **Rancher** | **Enterprise mgmt** | **Medium** | **✅** |
| **MicroK8s** | **Lightweight setups** | **Easy** | ⚠**️** |
| **k3d** | **Local Docker** | **Easy** | **❌** |
| **kubeone** | **Production IaC** | **Medium** | **✅** |
| **CAPI** | **Advanced automation** | **High** | **✅** |
| **Talos** | **Secure OS + K8s** | **High** | **✅** |

🧠 **How to Choose**

- **Learning**: minikube, kind
- **Local dev**: minikube, k3d, MicroK8s
- **Production (cloud)**: EKS, GKE, AKS
- **Production (self-managed)**: kubeadm, Kubespray, kubeone, RKE2
- **Edge / IoT**: K3s, MicroK8s

<a id="kubeadm-kubespray-kubeone-and-rancher-rke2"></a>

#### Kubeadm, Kubespray, kubeone and Rancher (RKE2)

<a id="kubeadm-vs-kubespray"></a>

##### Kubeadm vs. Kubespray

**This** is one of the most important distinctions when learning Kubernetes setup.

🧱 **kubeadm vs Kubespray (Simple View)**

- **kubeadm → a tool to bootstrap Kubernetes**
- **Kubespray → a full automation framework that uses kubeadm under the hood**

👉 **In short**:

- **Kubespray = kubeadm + Ansible + automation + best practices**

⚙️ **kubeadm**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-050.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-051.png)

**Official tool from Kubernetes**

- **CLI-based (kubeadm init, kubeadm join)**

**What YOU do manually**:

- **Provision servers (VMs/bare metal)**
- **Install container runtime (containerd, etc.)**
- **Configure networking (CNI like Calico)**
- **Handle upgrades and scaling**

**What kubeadm does**:

- **Sets up control plane**
- **Generates certificates**
- **Joins nodes to cluster**

✅ **Best for**:

- **Learning Kubernetes internals**
- **Full control environments**

❌ **Downside**:

- **Lots of manual work**
- **Easy to misconfigure**

🤖 **Kubespray**

**Uses Ansible**

- **Open-source project to deploy production clusters**

**What Kubespray does**:

- **Provisions cluster using playbooks**
- **Installs**:
  - **container runtime**
  - **Kubernetes (via kubeadm internally)**
  - **networking (Calico, etc.)**
- **Handles**:
  - **HA setup**
  - **scaling**
  - **upgrades**

**What YOU do**:

- **Define inventory (nodes, IPs)**
- **Run Ansible playbooks**

✅ **Best for**:

- **Production clusters**
- **Bare metal or multi-cloud**

❌ **Downside**:

- **More complex to understand**
- **Requires Ansible knowledge**

🔍 **Key Difference (Core Idea)**

```text
👉 kubeadm is a building block
👉 Kubespray is a complete solution
```

⚖️ **Side-by-Side Comparison**

| **Feature** | **kubeadm** | **Kubespray** |
| --- | --- | --- |
| **Type** | **CLI tool** | **Automation framework** |
| **Uses Ansible** | **❌** | **✅** |
| **Uses kubeadm inside** | **—** | **✅** |
| **Setup effort** | **High (manual)** | **Medium (automated)** |
| **Production-ready** | ⚠️ (**manual work needed)** | **✅** |
| **HA setup** | **Manual** | **Automated** |
| **Upgrades** | **Manual** | **Automated** |
| **Networking** | **Manual** | **Automated** |

🧠 **Real-World Analogy**

**kubeadm → assembling a PC yourself (you pick and install every part)**

**Kubespray → using a pre-configured build script that assembles it for you**

🧭 **When to Choose What**

**Use kubeadm if**:

**You want to learn deeply**

**You need custom setups**

**You’re debugging or experimenting**

**Use Kubespray if**:

**You want production clusters**

**You prefer automation**

**You manage multiple nodes/servers**

✅ **Bottom Line**

**kubeadm = low-level, manual, flexible**

**Kubespray = automated, production-ready, built on kubeadm**

<a id="kubeadm-vs-kubespray-vs-kubeone-vs-rke2"></a>

##### Kubeadm vs. Kubespray vs. kubeone vs. RKE2

**These four tools sit at different layers of the Kubernetes “installation stack,” and that** is where most confusion comes from. Let us make it crystal clear.

🧠 **Big Picture (1 sentence each)**

**kubeadm → low-level bootstrap tool**

**Kubespray → Ansible automation around kubeadm**

**kubeone → declarative production cluster manager (with Terraform)**

**Rancher (RKE2) → full Kubernetes distribution + lifecycle platform**

🧱 **1. kubeadm (foundation layer)**

**Official tool from Kubernetes**

**CLI-based**: kubeadm init, kubeadm join

**Role**:

👉 **Bootstraps Kubernetes cluster only**

**You handle**:

**Infrastructure (VMs, servers)**

**Networking (CNI)**

**Upgrades**

**HA setup**

**Key idea**:

➡️ **Raw building block**

🤖 **2. Kubespray (automation layer)**

**Uses Ansible**

**Internally uses kubeadm**

**Role**:

👉 **Automates cluster setup using best practices**

**What it adds**:

**Automated HA setup**

**Networking (Calico, etc.)**

**OS configuration**

**Scaling & upgrades**

**Key idea**:

➡️ **kubeadm + automation scripts**

📦 **3. kubeone (declarative production layer)**

**From Kubermatic**

**Works with Terraform**

**Role**:

👉 **Manages production clusters declaratively**

**What it does**:

**Separates**:

**Infra → Terraform**

**Kubernetes → kubeone**

**Handles**:

**HA clusters**

**Upgrades**

**Lifecycle management**

**Key idea**:

➡️ **Infrastructure-as-Code + Kubernetes lifecycle**

🏢 **4. Rancher / RKE2 (distribution + platform layer)**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-043.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-042.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-052.png)

**From Rancher Labs**

**RKE2**:

**Hardened Kubernetes distribution**

**Security-focused (used in gov/enterprise)**

**Rancher**:

**UI + API for managing clusters**

**Role**:

👉 **Full Kubernetes platform + distribution**

**What it includes**:

**Preconfigured Kubernetes**

**Built-in security defaults**

**Multi-cluster management**

**UI dashboard**

**Key idea**:

➡️ “**Ready-to-use Kubernetes platform”**

⚖️ **Side-by-Side Comparison**

| **Feature** | **kubeadm** | **Kubespray** | **kubeone** | **RKE2 / Rancher** |
| --- | --- | --- | --- | --- |
| **Type** | **CLI tool** | **Automation (Ansible)** | **Declarative manager** | **Distribution + platform** |
| **Uses kubeadm** | **—** | **✅** | **✅** | ❌ (**own implementation)** |
| **Infra provisioning** | **❌** | ⚠️ **partial** | ✅ (**via Terraform)** | ⚠️ (**via Rancher drivers)** |
| **Automation level** | **Low** | **Medium** | **High** | **Very High** |
| **Ease of use** | **Hard** | **Medium** | **Medium** | **Easy** |
| **Production ready** | ⚠️ **manual work** | **✅** | **✅** | **✅** |
| **UI included** | **❌** | **❌** | **❌** | **✅** |
| **Multi-cluster** | **❌** | **❌** | ⚠️ **limited** | ✅ **strong** |
| **Best for** | **Learning** | **Flexible prod setups** | **IaC environments** | **Enterprise platforms** |

🧭 <u>**Mental Model (Very Important)**</u>

**Think of it like building vs automating vs platform**:

```text
kubeadm → Kubespray → kubeone → Rancher/RKE2
(build)   (automate)   (declare)   (platform)
```

🧠 **Real-World Analogy**

**kubeadm → assembling a car manually**

**Kubespray → using a robotic assembly line**

**kubeone → designing + managing fleets with automation**

**Rancher/RKE2 → buying a fully built, managed car system**

🧭 **When to Use What**

**Use kubeadm if**:

**You want full control / learning**

**Use Kubespray if**:

**You want automation on bare metal or cloud**

**Use kubeone if**:

**You use Terraform / IaC**

**You want reproducible production clusters**

**Use Rancher / RKE2 if**:

**You want**:

**UI + multi-cluster management**

**enterprise-ready Kubernetes**

**simplified operations**

✅ **Final Takeaway**

**kubeadm = foundation**

**Kubespray = automated installer**

**kubeone = declarative lifecycle manager**

**Rancher/RKE2 = full Kubernetes platform**

<a id="cni-container-network-interface"></a>

#### CNI (Container Network Interface)

**Container Network Interface (CNI)** is a Cloud Native Computing Foundation project consisting of a specification and libraries for writing plugins to configure network interfaces in Linux and Windows containers. It enables networking for container orchestration systems like Kubernetes, OpenShift, and Amazon ECS by managing IP address allocation and connectivity, typically via plugins such as Calico, Cilium, or Flannel.

**Key Aspects of CNI**

- **Purpose**: CNI focuses solely on network connectivity of containers and removing allocated resources when the container is deleted.
- **How it Works**: When a container is created, the runtime (e.g., CRI) calls the CNI plugin to configure the network interface within the container's network namespace, assigns an IP, and sets up routes.
- **Pluggable Architecture**: CNI plugins are generally divided into two types: interface plugins (e.g., bridge, vlan) and IPAM plugins (e.g., host-local, dhcp).
- **Popular Plugins**: Calico (network policy, BGP), Cilium (eBPF-based security), and Flannel (simple overlay) are commonly used.
- **Features**: Plugins support network interface creation, IP address management, traffic shaping, and bandwidth control.

<a id="clusterip-nodeport-loadbalancer-in-k8s"></a>

#### ClusterIP, NodePort & LoadBalancer in K8s

**ClusterIP** is the default, internal-only service for pod-to-pod communication within a cluster. NodePort exposes services externally on a static port on each node's IP, suitable for testing. LoadBalancer integrates with cloud providers to provision a dedicated, public-facing IP for production workloads, building upon NodePort.

**Key Differences Summary**

| **Feature** | `ClusterIP` | `NodePort` | `LoadBalancer` |
| --- | --- | --- | --- |
| **Access** | **Internal Only (within cluster)** | **External (via NodeIP**: Port) | **External (via Cloud Load Balancer)** |
| **Default?** | **Yes** | **No** | **No** |
| **Use Case** | **Internal backend services** | **Development/Testing** | **Production/Public apps** |
| **Cost** | **Free** | **Free** | **Extra Cloud Costs** |
| **Port Range** | **N/A** | **30000-32767** | **Dynamic** |

**Detailed Breakdown**

**ClusterIP (Default)**: Assigns a stable internal IP address within the Kubernetes cluster, making it accessible only by other services inside the same cluster. It is ideal for microservices that do not need to be exposed to the internet, such as databases or internal backend APIs.

**NodePort (External)**: Exposes the service on a specific port (usually 30000–32767) on all nodes. Anyone can access the service externally using &lt;NodeIP&gt;:&lt;NodePort&gt;, even without a cloud provider load balancer.

**LoadBalancer (Public/Production)**: Creates a NodePort and ClusterIP automatically, then requests a cloud provider (e.g., AWS, Azure, GCP) to create a dedicated network load balancer. This allows external users to access the service via a single dedicated public IP address.

**Choosing the Right Type**

Use ClusterIP for backend services that should never be public, like connecting to a database.

Use NodePort for quick demos, development environments, or when you don't have a cloud provider's load balancer.

Use LoadBalancer for public-facing production applications needing a dedicated, reliable external IP.

<a id="kubernetes-ui-dashboard-core-yaml-files"></a>

#### Kubernetes UI/Dashboard & Core YAML Files

⚙️ **Cluster Setup Tools**

**kubeadm
→ Minimal cluster setup (manual add-ons)**

**Kubespray
→ Automated, production-ready, configurable add-ons**

👉 ❗ **Neither installs a dashboard by default**

🖥️ **Kubernetes UI / Dashboard**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-053.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-054.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-055.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-056.png)

**Kubernetes Dashboard**

**Manual install**

**Requires token + RBAC**

**Basic UI**

**Headlamp**

**Modern UI (desktop/web)**

**Uses kubeconfig (easy login)**

**Multi-cluster support**

👉 **Preferred today**: Headlamp (simpler + safer)

📦 **Core YAML Files**

**1. deployment.yaml → Run app**

**Defines**:

**Image**

**Replicas**

**Updates**

**Ensures pods stay running**

**Example concept**:

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: my-app
spec:
  replicas: 3
```

👉 “**How many copies of my app should run?”**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-057.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-058.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-059.gif)

**2. service.yaml → Expose app**

**Gives stable network access**

**Types**:

**ClusterIP (internal)**

**NodePort (node access)**

**LoadBalancer (external)**

**Example**:

```yaml
kind: Service
spec:
  type: NodePort
```

👉 “**How do users reach my app?”**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-060.png)

**3. values.yaml → Helm config**

**Used by Helm**

**Stores variables for templates**

**Example**:

```yaml
replicaCount: 2
image:
  tag: latest
service:
  type: ClusterIP
```

👉 “**What settings customize my deployment?”**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-061.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-062.jpeg)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-063.png)

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-064.png)

🔄 **How Everything Connects**

**Deployment → runs pods**

**Service → exposes pods**

**values.yaml → config for Helm templates**

**UI (Headlamp/Dashboard) → visualize + manage cluster**

⚡ **One-Line Memory Hooks**

**Deployment = run & scale app**

**Service = network access**

**values.yaml = configuration input**

**Dashboard/Headlamp = visual control panel**

✅ **Exam-Style Recall Questions**

**What ensures pods stay alive? → Deployment**

**Why not access pods directly? → Dynamic IPs → use Service**

**What is values.yaml used for? → Helm configuration**

**Does kubeadm install dashboard? → No**

**Modern alternative to Dashboard? → Headlamp**

[Back to Contents](#contents)

---

<a id="docker-swarm"></a>

### Docker Swarm

**Docker Swarm** is Docker’s built-in system for running containers across multiple machines as if they were one big computer.

If Docker lets you run one container on one machine, Docker Swarm lets you run many containers on many machines, automatically.

**The idea in plain terms**

**You give Docker Swarm**:

```text
“Run 5 copies of this app and keep them alive.”
```

**Swarm handles**:

- **which machines run them**
- **restarting containers if one crashes**
- **spreading traffic between them**
- **adding/removing machines easily**

You focus on what should run. Swarm figures out where.

**Core concepts**

**1. Nodes**

Each machine is a node in the swarm.

- **Manager nodes → make decisions (scheduling, cluster state)**
- **Worker nodes → actually run containers**

**2. Services**

**Instead of running containers directly, you define a service**:

- **which image to use**
- **how many replicas**
- **ports, environment variables, volumes**

Swarm keeps the service running exactly as declared.

**3. Stack**

A stack is a group of services defined in a docker-compose.yml file and deployed to the swarm:

```bash
docker stack deploy -c docker-compose.yml my_app
```

**Why people use Docker Swarm**

🚀 **Simple (comes with Docker, no extra tools)**

🔁 **Self-healing (restarts failed containers)**

⚖️ **Load balancing built in**

🔐 **Secure by default (mutual TLS between nodes)**

🧩 **Easy transition from docker-compose**

**Docker Swarm vs Kubernetes**

| **Docker Swarm** | **Kubernetes** |
| --- | --- |
| **Very easy to learn** | **Steeper learning curve** |
| **Built into Docker** | **Separate ecosystem** |
| **Great for small–medium setups** | **Best for large, complex systems** |
| **Less powerful** | **Very powerful & flexible** |

Swarm is often chosen when you want cluster orchestration without the complexity.

**When Docker Swarm makes sense**

- **Small production clusters**
- **Internal tools**
- **Side projects**
- **Teams already using Docker Compose**
- **You don’t need heavy Kubernetes features**

**When it might not**

- **Very large clusters**
- **Advanced autoscaling needs**
- **Complex networking policies**
- **Cloud-native platform requirements**

[Back to Contents](#contents)

---

<a id="serverless-and-faas"></a>

### Serverless and FaaS

**Serverless Function-as-a-Service (FaaS)** is an event-driven cloud computing model where developers run modular code functions in response to events without managing servers. The provider automatically handles scaling and infrastructure, charging only for active execution time. Common use cases include API backends, data processing, and IoT triggers.

**Key Features and Benefits**

- **No Server Management**: Developers focus solely on code, as infrastructure management is completely abstracted.
- **Cost Efficiency**: You only pay for the exact computation time used (pay-per-use), reducing costs for idle services.
- **Automatic Scalability**: Functions scale horizontally, automatically adjusting to workload fluctuations.
- **Event-Driven**: Ideal for executing code in response to specific triggers like HTTP requests, database updates, or file uploads.

**Common FaaS Providers**

- **AWS Lambda**: AWS Lambda
- **Google Cloud Functions**: Google Cloud Functions
- **Azure Functions**: Azure Functions
- **OpenFaaS**: OpenFaaS (Open-source alternative)

**Disadvantages and Challenges**

- **Cold Start Latency**: A delay can occur when a function wakes up after being idle.
- **Limited Control**: Inability to access or customize the underlying infrastructure.
- **Debugging Difficulty**: Testing and debugging distributed, stateless functions can be more complex than traditional apps.
- **Vendor Lock-in**: Code might become heavily dependent on a specific provider's ecosystem.

**Key Differences from Traditional Serverless**

While "serverless" is the broad category (including databases and storage), FaaS is a specific subset focused on computing. FaaS is best for short-lived, functional tasks rather than long-running, complex monolith applications.

[Back to Contents](#contents)

---

<a id="virtual-private-cloud-vpc"></a>

### Virtual Private Cloud (VPC)

**A Virtual Private Cloud (VPC)** is a secure, logically isolated private network hosted within a public cloud provider's infrastructure, such as AWS, Google Cloud, or DigitalOcean. It allows users to define their own IP address ranges, subnets, route tables, and network gateways, providing full control over network configuration and security.

**Key Features and Benefits**

- **Isolation & Security**: Resources within a VPC are isolated from other customers in the public cloud, enhancing security.
- **Customization**: Users can define their own network topology, including public and private subnets, like a traditional data center.
- **Connectivity**: VPCs can connect to the internet via internet gateways or to on-premises data centers using VPNs or dedicated connections.
- **Scalability & Cost**: Provides scalable infrastructure without the high cost of maintaining physical network hardware.

**Common Use Cases**

- **Hosting Web Applications**: Deploying multi-tier applications with public-facing web servers and private database servers.
- **Data Security**: Storing sensitive data in a private subnet with no direct internet access.
- **Network Segmentation**: Separating development, testing, and production environments.

**Components**

- **Subnets**: Subdivisions of the VPC's IP address range, which can be public (connected to the internet) or private.
- **Route Tables**: Control the flow of network traffic between subnets and out to the internet.
- **Gateways**: Enable connectivity to the internet (Internet Gateway) or virtual private networks (VPN).
- **Security Groups/NACLs**: Act as firewalls to control inbound and outbound traffic.

Major providers include AWS, Google Cloud, and DigitalOcean, each offering similar networking capabilities to manage resources securely.

[Back to Contents](#contents)

---

<a id="amazon-route-53"></a>

### Amazon Route 53

**Amazon Route 53** is AWS’s managed DNS service. It helps translate human-readable domain names, such as `example.com`, into the technical addresses that browsers and services need to reach applications.

The name **Route 53** likely refers to two ideas:

- **Route**: routing users to the right destination.
- **53**: DNS commonly uses TCP/UDP port `53`.

**What Route 53 Is Used For**

- **DNS hosting**: Manage DNS records for a domain.
- **Domain registration**: Buy and manage domain names through AWS.
- **Traffic routing**: Send users to AWS services or external/non-AWS infrastructure.
- **Health checks**: Monitor whether an application endpoint is healthy.
- **High availability**: Use AWS’s globally distributed DNS infrastructure.
- **IPv6 support**: Route 53 supports end-to-end DNS resolution over IPv6.

**Hosted Zones**

- A **hosted zone** is a container for DNS records for a domain.
- When you create a hosted zone, Route 53 gives you a set of authoritative name servers.
- You add, delete, and update DNS records inside that hosted zone.
- These records decide where traffic for names like `api.example.com` or `www.example.com` should go.

**Common DNS Records in Route 53**

| **Record type** | **Simple meaning** |
| --- | --- |
| `A` | Points a domain name to an IPv4 address. |
| `AAAA` | Points a domain name to an IPv6 address. |
| `CNAME` | Points one DNS name to another DNS name. |
| `MX` | Defines mail servers for a domain. |
| `TXT` | Stores text values, often used for verification and security records. |
| `Alias` | AWS-specific virtual record that points to selected AWS resources. |

**Alias Records**

- Route 53 has an AWS-specific record type called an **Alias** record.
- An Alias record behaves somewhat like a `CNAME`, because it can point one name to another DNS name.
- Unlike a normal `CNAME`, Route 53 resolves the Alias on the server side and returns address records to the client.
- This makes Alias records useful for AWS resources that expose DNS names instead of fixed IP addresses, such as:
  - **Elastic Load Balancer**
  - **CloudFront distribution**
  - **some S3 website endpoints**
- Alias records can also be used at the domain apex/root, such as `example.com`, where normal `CNAME` records are not allowed.

**Programmatic Access**

- Route 53 can be managed through AWS APIs, SDKs, CLI commands, and Infrastructure as Code tools.
- This allows teams to automatically create or update DNS records when infrastructure changes.
- Example: a deployment can create a new load balancer and then automatically point a DNS record to it.

**One-Line Summary**

```text
Route 53 is AWS DNS: it maps domain names to AWS or non-AWS resources, supports health-aware routing, and lets DNS records be automated through AWS tooling.
```

[Back to Contents](#contents)

---

<a id="virtual-private-server-vps"></a>

### Virtual Private Server (VPS)

**A Virtual Private Server (VPS)** is an isolated virtual machine hosted on a physical server, offering dedicated resources (CPU, RAM, storage) and full root access for websites or applications. It acts as a middle ground between cheap shared hosting and expensive dedicated servers, providing better performance, security, and scalability with fixed monthly pricing.

**Key Features and Benefits of VPS**

- **Isolation & Stability**: Because each VPS is isolated, high traffic on one site will not affect the performance of others on the same physical machine.
- **Root Access**: Users have administrative control to install custom software and modify server settings.
- **Dedicated Resources**: Guaranteed CPU, RAM, and storage, offering faster load times and higher reliability.
- **Scalability**: Resources like RAM or CPU can be upgraded easily as your website or application grows.

**VPS vs. Other Hosting Types**

- **vs. Shared Hosting**: VPS provides more power, security, and flexibility than shared hosting, which shares resources among many users.
- **vs. Dedicated Server**: More affordable than a dedicated server, offering high performance at a lower price point.
- **vs. Cloud Hosting**: A traditional VPS usually lives on one physical server, whereas cloud hosting uses a distributed architecture, offering better reliability.

**Common Uses for VPS**

- **Website Hosting**: Ideal for high-traffic websites, e-commerce stores, or complex web applications.
- **Game Servers**: Provides the dedicated resources needed to host online games.
- **Development & Testing**: A secure, isolated environment for developers to test apps.
- **VPN or File Storage**: Dedicated space to run a private Virtual Private Network (VPN) or store files securely.

**Popular VPS Providers**

**Several providers offer managed and unmanaged VPS solutions, including**:

- **Contabo (offers high-value, cost-effective plans)**
- **OVHcloud (provides diverse VPS plans with dedicated resources)**
- **DigitalOcean (popular for developer-friendly cloud servers)**
- **Hostinger (known for affordable, high-performance plans)**
- **Namecheap (provides managed, secure VPS solutions)**

**When to Choose a VPS**

- A VPS is recommended when your site outgrows shared hosting, requires specific software configurations, or handles sensitive data that needs high security.

[Back to Contents](#contents)

---

<a id="fibre-channel-fc-storage-area-network-san"></a>

### Fibre Channel (FC) Storage Area Network (SAN)

**Fibre Channel (FC) Storage Area Network (SAN)** components interact to provide a dedicated, high-speed, and lossless network for block-level storage traffic, separating it from standard LAN traffic.

The interaction involves a complex, automated process of login, address assignment, discovery, and path management, often implemented using redundant fabrics for high availability.

**Core SAN Components**

Components of a Storage Area Network (SAN) involve three basic components:

- **Server**
- **Network Infrastructure**
- **Storage**

The above elements are classified into the following elements:

- **Node port**
- **Cables**
- **Interconnection Devices**
- **Storage Array**
- **SAN Management Software**

#### FC SAN Physical Components and Connectivity

| **Component** | **Function / Role** |
| --- | --- |
| **Nodes** | Hosts, servers, storage arrays, and tape libraries are referred to as nodes. They act as source/destination for data. |
| **Host Bus Adapters (HBAs)** | Installed in servers and connected to the network via fiber optic cables. They offload storage traffic processing from the server CPU and appear as SCSI adapters to the OS. |
| **Storage Controllers / Storage Ports** | Front-end ports on storage arrays that accept connections from switches and incoming traffic to storage disks. |
| **FC Switches** | Act as the fabric, directing data traffic directly from one port to another. They also route traffic, perform zoning, and provide fabric services. |
| **Cabling** | Optical fiber cables carry data via light, ensuring high-speed data transmission. |
| **Management Software** | Configures, monitors, and provides visibility into the fabric. |

**Node Ports**

- In Fibre Channel, devices like **hosts**, **storage**, and **tape libraries** are referred to as nodes.
- Nodes consist of ports for transmission between other nodes.
- Ports operate in full-duplex data transmission mode with transmit (`Tx`) and receive (`Rx`) links.

**Cables**

- SAN implements optical fiber cabling.
- Copper cables are used for short-distance connectivity.
- Optical cables are used for long-distance connection establishment.

**Optical Cable Types**

| **Cable type** | **Description** | **Typical distance / note** |
| --- | --- | --- |
| **Multi-mode fiber (MMF)** | Carries multiple rays of light projected at different angles simultaneously onto the core of the cable. In MMF transmission, light beams tend to disperse and collide. This collision weakens the signal strength after it travels a certain distance, called modal dispersion. | Used for distances up to 500 meters because of signal degradation/attenuation due to modal dispersion. |
| **Single-mode fiber (SMF)** | Carries a single beam of light through the core of the fiber. The small core reduces modal dispersion. | Used for distances up to 10 kilometers due to less attenuation. SMF is costlier than MMF. |

**Connectors and Transceivers**

- **Standard Connectors (SC)** and **Lucent Connectors (LC)** are commonly used fiber connectors.
- Data transmission speeds can reach up to **1 Gbps** and **4 Gbps** respectively.
- **Small Form-factor Pluggable (SFP)** is an optical transceiver used in optical communication with transmission speed up to **10 Gbps**.

#### FC SAN Logical Interaction & Fabric Services

The interaction is governed by the **Fibre Channel Protocol (FCP)**, which facilitates the transport of SCSI commands.

| **Fabric service / step** | **What happens** |
| --- | --- |
| **Fabric Login (FLOGI)** | When an HBA (initiator) or storage port (target) is connected, it sends a FLOGI request to the switch. The switch assigns a 24-bit `FCID` (Fibre Channel ID) address to the port, similar to an IP address. |
| **Fabric Name Service (FCNS)** | After login, the device registers its Worldwide Name (`WWN`) and `FCID` with the switch's Name Server, allowing other devices in the fabric to find it. |
| **Port Login (PLOGI)** | Initiator nodes (servers) perform a port login to communicate with target nodes (storage). |
| **Process Login (PRLI)** | The initiator exchanges upper-layer protocol parameters, such as SCSI, with the target. |

#### Traffic Control & Security

- **Zoning**: A security mechanism configured on switches to isolate traffic. Zoning defines which nodes, such as server HBAs, can communicate with other nodes, such as storage ports. This prevents unauthorized access between servers and limits communication to intended storage targets.
- **LUN Masking**: A function of the storage array, not the switch. It restricts which Logical Unit Numbers (`LUNs`) a specific server HBA, based on its `WWN`, can see and access, enhancing security at the destination.

#### Redundancy and Reliability

- **Fabric A & B**: For critical applications, two physically separate SAN networks, Fabric A and Fabric B, are used. Each server has two HBAs, one connected to each fabric.
- **Multipathing**: Software on the host server manages multiple paths. If a cable, HBA, or switch fails, traffic automatically reroutes to the alternative path, ensuring no downtime.
- **Inter-Switch Links (ISLs)**: Link switches together, allowing them to form a single, larger fabric.

#### Interconnection Devices

The commonly used interconnection devices in SAN are:

- **Hubs**
- **Switches**
- **Directors**

| **Device** | **Role** |
| --- | --- |
| **Hubs** | Communication devices used in fiber cable implementations. They connect nodes in loop or star topology. |
| **Switches** | More intelligent than hubs. They directly route data from one port to another. They are cheap and their performance is better than hubs. |
| **Directors** | Larger than switches and used for data center implementations. Directors have high fault tolerance and higher port count than switches. |

#### Storage Array

**A disk array**, also called a **storage array**, is a data storage system used for block-based storage, file-based storage, or object storage.

The term describes dedicated storage hardware that contains spinning hard disk drives (`HDDs`) or solid-state drives (`SSDs`).

**The fundamental purpose of a SAN** is to provide host access to storage resources.

SAN storage implementations provide:

- **high availability and redundancy**
- **improved performance**
- **business continuity**
- **multiple host connectivity**

#### SAN Management Software

**SAN Management Software** manages the interface between the host, interconnection devices, and storage arrays.

It includes key management functions such as:

- **mapping of storage devices**
- **mapping of switches**
- **logical partitioning of SAN, called zoning**
- **management of storage devices**
- **management of interconnection devices**

#### Summary of Component Roles

| **Component** | **Function** |
| --- | --- |
| **HBA** | Connects host to fabric and offloads SCSI commands. |
| **Switch** | Routes traffic, performs zoning, and provides fabric services. |
| **Storage Port** | Accepts incoming traffic to storage disks. |
| **Management Software** | Configures, monitors, and provides visibility into the fabric. |

[Back to Contents](#contents)

---

<a id="sidecar-pattern"></a>

### Sidecar Pattern

**The sidecar pattern** is a cloud-native design pattern where a helper container (the sidecar) runs alongside a main application container, sharing the same lifecycle, network, and storage. It offloads peripheral tasks—such as logging, proxying, or monitoring—from the main application, improving modularity and allowing different languages/runtimes for each component.

**Key Aspects of the Sidecar Pattern**:

- **Structure**: It is a single-node pattern where both containers reside in the same host or Kubernetes Pod.
- **Separation of Concerns**: The main application handles business logic, while the sidecar handles infrastructure or cross-cutting concerns.
- **Shared Resources**: Sidecars share the same filesystem, localhost, and network namespace as the main app.
- **Examples**: Using a logging agent to stream application logs, a service mesh proxy (like Istio/Envoy) to handle networking, or a config loader.

**Advantages**:

- **Language Independence**: The sidecar can be written in a different language than the main app.
- **Improved Reusability**: The same sidecar component can be used across multiple applications.
- **Independent Scalability/Maintenance**: Sidecars can be updated or scaled without altering the primary application.
- **Low Latency**: High-speed, local communication between the app and the sidecar.

**Common Use Cases**:

- **Service Mesh**: Envoy proxying service-to-service communication.
- **Observability**: Collecting logs or metrics (e.g., Fluentd, Prometheus exporter).
- **Networking**: Handling SSL/TLS termination.
- **Configuration**: Dynamic configuration updates.

This pattern is foundational to modern cloud-native architecture, providing a clean separation of application logic from infrastructure concerns.

[Back to Contents](#contents)

---

<a id="content-delivery-network-cdn"></a>

### Content Delivery Network (CDN)

https://www.cloudflare.com/learning/cdn/what-is-a-cdn/

**A content delivery network (CDN)** is a geographically distributed group of servers that caches content close to end users. A CDN allows for the quick transfer of assets needed for loading Internet content, including HTML pages, JavaScript files, stylesheets, images, and videos.

The popularity of CDN services continues to grow, and today most of the web traffic is served through CDNs, including traffic from major sites like Facebook, Netflix, and Amazon.

A properly configured CDN may also help protect websites against some common malicious attacks, such as Distributed Denial of Service (DDOS) attacks.

**Is a CDN the same as a web host?**

While a CDN does not host content and can’t replace the need for proper web hosting, it does help cache content at the network edge, which improves website performance. Many websites struggle to have their performance needs met by traditional hosting services, which is why they opt for CDNs.

By utilizing caching to reduce hosting bandwidth, helping to prevent interruptions in service, and improving security, CDNs are a popular choice to relieve some of the major pain points that come with traditional web hosting.

**What are the benefits of using a CDN?**

Although the benefits of using a CDN vary depending on the size and needs of an Internet property, the primary benefits for most users can be broken down into four different components:

Improving website load times - By distributing content closer to website visitors by using a nearby CDN server (among other optimizations), visitors experience faster page loading times. As visitors are more inclined to click away from a slow-loading site, a CDN can reduce bounce rates and increase the amount of time that people spend on the site. In other words, a faster a website means more visitors will stay and stick around longer.

Reducing bandwidth costs - Bandwidth consumption costs for website hosting is a primary expense for websites. Through caching and other optimizations, CDNs can reduce the amount of data an origin server must provide, thus reducing hosting costs for website owners.

Increasing content availability and redundancy - large amounts of traffic or hardware failures can interrupt normal website function. Thanks to their distributed nature, a CDN can handle more traffic and withstand hardware failure better than many origin servers.

Improving website security - A CDN may improve security by providing DDoS mitigation, improvements to security certificates, and other optimizations.

**How does a CDN work?**

At its core, a CDN is a network of servers linked together with the goal of delivering content as quickly, cheaply, reliably, and securely as possible. In order to improve speed and connectivity, a CDN will place servers at the exchange points between different networks.

These Internet exchange points (IXPs) are the primary locations where different Internet providers connect in order to provide each other access to traffic originating on their different networks. By having a connection to these high speed and highly interconnected locations, a CDN provider can reduce costs and transit times in high-speed data delivery.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-069.png)

Beyond placement of servers in IXPs, a CDN makes a few optimizations on standard client/server data transfers. CDNs place Data Centers at strategic locations across the globe, enhance security, and are designed to survive various types of failures and Internet congestion.

<a id="point-of-presence-pop"></a>

#### Point of Presence (PoP)

**A Point of Presence (PoP)** is a physical network location where users, providers, or networks connect to each other.

In simpler words:

```text
PoP = a local network access point where traffic enters or leaves a provider's network.
```

**Common Example**

- An ISP may have a local PoP in a city or region.
- Customers connect to that PoP to reach the ISP network.
- From there, the ISP routes traffic onward to the internet or to other networks.

**What a PoP Usually Contains**

- **Servers**
- **Routers**
- **Network switches**
- **Multiplexers**
- **Network interface equipment**
- **Optical Line Terminals (OLTs)** for fibre internet

**Where PoPs Are Usually Located**

- **Data centers**
- **Internet Exchange Points (IXPs)**
- **Colocation centers**

**Why Providers Use Multiple PoPs**

- **Lower latency**: users connect to a nearby network location.
- **Better reliability**: traffic can use another PoP if one location has problems.
- **More capacity**: traffic is spread across many access points.
- **Regional coverage**: providers can serve users in many cities or countries.

**PoP vs. Edge Server**

| **Concept** | **Simple meaning** |
| --- | --- |
| **PoP** | The physical network location or access point. |
| **Edge server** | A server inside or near that PoP that serves cached content or handles traffic close to users. |

**Historical Telecom Meaning**

- In U.S. telecom history, the term became important during the breakup of the Bell telephone system.
- A PoP was a location where a long-distance carrier could connect its services into a local telephone network area.

**One-Line Summary**

```text
A PoP is the place where networks meet, users connect, and providers place equipment close to customers to improve access, latency, and reliability.
```

<a id="cdn-edge-server"></a>

#### CDN Edge Server

https://www.cloudflare.com/learning/cdn/glossary/edge-server/

**What is a CDN edge server?**

A CDN edge server is a computer that exists at the logical extreme or “edge” of a network. An edge server often serves as the connection between separate networks. A primary purpose of a CDN edge server is to store content as close as possible to a requesting client machine, thereby reducing latency and improving page load times.

An edge server is a type of edge device that provides an entry point into a network. Other edges devices include routers and routing switches. Edge devices are often placed inside Internet exchange points (IxPs) to allow different networks to connect and share transit.

**How does an edge server work?**

In any network layout, a few different devices will connect to each other using one or more predefined network pattern. If a network wants to connect to another network or the larger Internet, it must have some form of bridge for traffic to flow from one location to another. Hardware devices that create this bridge on the edge of a network are called edge devices.

**Networks connect across the edge**

In a typical home or office network with many devices connected, devices such as mobile phones or computers connect and disconnect to the network through a hub-and-spoke network model. All the devices exist within the same local area network (LAN), and each device connects to a central router, through which they can connect with each other.

To connect a second network to the first network, at some point the connection must be made between the networks. The device through which the networks can connect with each other is, by definition, an edge device.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-070.png)

Now, if a computer inside Network A needs to connect to a computer inside Network B, the connection must pass from network A, across the network edge, and into the second network. This same paradigm also works in more complex contexts, such when a connection is made across the Internet. The ability for networks to share transit is bottlenecked by the availability of edge devices between them.

When a connection must traverse the Internet, even more intermediary steps must be taken between network A and network B. For the sake of simplicity, let's imagine that each network is a circle, and the place in which the circles touch is the edge of the network. In order for connection to move across the Internet, it will typically touch many networks and move across many network edge nodes. Generally speaking, the farther the connection must travel, the greater the number of networks that must be traversed. A connection may traverse different Internet service providers and Internet backbone infrastructure hardware before reaching its target.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-071.png)

A CDN provider will place servers in many locations, but some of the most important are the connection points at the edge between different networks. These edge servers will connect with multiple different networks and allow for traffic to pass quickly and efficiently between networks. Without a CDN, transit may take a slower and/or more convoluted route between source and destination. In worst case scenarios, traffic will “trombone” large distances; when connecting to another device across the street, a connection may move across the country and back again. By placing edge servers in key locations, a CDN is able to quickly deliver content to users inside different networks. To learn more about the improvements of using CDN, explore how CDN performance works.

**What is the difference between an edge server and an origin server?**

An origin server is the web server that receives all Internet traffic when a web property is not using a CDN. Using an origin server without a CDN means that each Internet request must return to the physical location of that origin server, regardless of where in the world it resides. This creates an increase in load times which increases the further the server is from the requesting client machine.

CDN edge servers store (cache) content in strategic locations to take the load from one or more origin servers. By moving static assets like images, HTML and JavaScript files (and potentially other content) as close as possible to the requesting client machine, an edge server cache can reduce the amount of time it takes for a web resource to load. Origin servers still have an important function to play when using a CDN, as important server-side code such as a database of hashed client credentials used for authentication, typically is maintained at the origin. Learn about the Cloudflare CDN with edge servers all over the globe.

[Back to Contents](#contents)

---

<a id="host-0-0-0-0"></a>

### Host 0.0.0.0

The IP address 0.0.0.0 is a special non-routable meta-address used in different contexts within networking to mean "unspecified," "invalid," or "all addresses," depending on how it is used.

**Common Meanings of 0.0.0.0**

- **For Servers (Binding)**: When you configure a server or application to "listen" or "bind" on the address 0.0.0.0, it means the service will accept connections on all available IPv4 network interfaces (e.g., Wi-Fi, Ethernet, loopback). This is often used during development to make an application accessible from other devices on the local network or the internet (if firewalls and routers permit).
- **As a Source Address (Initialization)**: A device uses 0.0.0.0 as its source IP address when it has not yet been assigned a proper address, such as during the initial DHCP (Dynamic Host Configuration Protocol) discovery process.
- **In Routing Tables (Default Route)**: In a computer's routing table, 0.0.0.0 (often with a netmask of 0.0.0.0 or as 0.0.0.0/0 in CIDR notation) signifies the default route or "gateway of last resort". This is the route used for any destination IP address for which no more specific path is listed.
- **As a Placeholder/Error**: On a client device, an IP address of 0.0.0.0 typically indicates that the device is not connected to a TCP/IP network or has failed to obtain an IP address from a DHCP server.

**Key Distinction**: 0.0.0.0 vs. 127.0.0.1 (localhost)

<u>**It is important not to confuse 0.0.0.0 with 127.0.0.1 (localhost)**</u>:

- 0.0.0.0 means "any address" and is used by servers to listen for connections from both inside and outside the machine.
- 127.0.0.1 is the loopback address, a special address reserved for communications within a single host. A service listening on 127.0.0.1 can only be accessed by other processes on the exact same machine, not from the local network or the internet.

[Back to Contents](#contents)

---

<a id="makefile"></a>

### Makefile

**Makefile** is a configuration file used by the make build automation tool to manage and automate the compilation of software projects. It acts as a "to-do list" for your terminal, defining a set of rules and dependencies to transform source code into executable programs or libraries.

**Core Purpose**

**Efficiency**: make uses file timestamps to determine which parts of a project have changed. It only recompiles the modified files and their dependents, saving significant time in large projects.

**Automation**: It replaces long, complex manual compilation commands with a single command: make.

**Consistency**: It ensures that every team member builds the project using the exact same steps and compiler flags.

**Basic Structure**

**A Makefile consists of rules, which typically follow this format**:

```makefile
target: prerequisites
	recipe
```

**Target**: Usually, the name of the file to be generated (e.g., an executable or object file) or an action name like clean.

**Prerequisites (Dependencies)**: Files that must exist and be up-to-date before the target can be built.

**Recipe**: The shell commands executed to create the target. Crucially, every recipe line must start with a TAB character, not spaces.

**Key Components**

**Variables**: Used to store reusable strings like compiler names (CC = gcc) or flags (CFLAGS = -Wall).

**Phony Targets**: Labels for actions that don't produce a file of the same name. Common examples include all (to build everything) and clean (to remove generated files).

**Comments**: Any line starting with # is ignored by the make utility.

**Common Commands**

**make**: Runs the first target defined in the Makefile (often all).

**make \[target\]**: Runs a specific rule (e.g., make clean or make install).

**make -j \[n\]**: Parallelizes the build using n CPU cores to speed up compilation.

While originally designed for C/C++ compilation, Makefiles are language-agnostic and are frequently used to automate tasks in web development (e.g., npm scripts), Docker image building, or data processing pipelines.

[Back to Contents](#contents)

---

<a id="data-ai-knowledge-systems"></a>

## Data, AI & Knowledge Systems

<a id="ragas-score"></a>

### RAGAS score

**RAGAS score** is an evaluation metric used to measure the quality of Retrieval-Augmented Generation (RAG) systems (LLMs that answer questions using retrieved documents).

Instead of only checking whether an answer sounds good, RAGAS evaluates how well the model used the retrieved information.

**What RAGAS measures**

RAGAS breaks performance into several sub-scores (each usually from 0 to 1, where higher is better):

- **1. Faithfulness**
  - **Checks whether the answer is grounded in the retrieved context**
  - **Penalizes hallucinations or unsupported claims**
- **2. Answer Relevance**
  - **Measures how well the answer actually addresses the user’s question**
  - **An answer can be faithful but still irrelevant**
- **3. Context Precision**
  - **Evaluates whether the retrieved documents are useful and focused**
  - **High precision = little irrelevant context**
- **4. Context Recall**
  - **Measures whether the retriever fetched all necessary information**
  - **Low recall = missing key facts needed to answer correctly**

**Overall RAGAS score**

**The RAGAS score is typically an aggregate (average) of these components**

**Example**:

```text
RAGAS Score = (Faithfulness + Answer Relevance + Context Precision + Context Recall) / 4
```

(**Some teams customize which components they include.)**

**Why RAGAS matters**

- **Designed specifically for RAG pipelines, not just raw LLM output**
- **Helps diagnose where things go wrong**:
  - **Bad retrieval?**
  - **Hallucinations?**
  - **Off-topic answers?**
- **Useful for benchmarking, prompt tuning, and retriever comparison**

**Quick intuition**

| **Score range** | **Interpretation** |
| --- | --- |
| **0.9 – 1.0** | **Excellent, well-grounded RAG** |
| **0.7 – 0.9** | **Good, minor issues** |
| **0.5 – 0.7** | **Noticeable retrieval or grounding problems** |
| < **0.5** | **Poor RAG performance** |

[Back to Contents](#contents)

---

<a id="avro-vs-parquet"></a>

### Avro vs. Parquet

**Avro (row-based) and Parquet (columnar)** are both popular binary, self-describing big data formats.

Choose Avro for write-heavy, streaming data (e.g., Kafka) and complex schema evolution. Choose Parquet for read-heavy, analytical queries (e.g., Spark, Hive, Data Lakes) because it offers superior compression and column-specific retrieval.

[Back to Contents](#contents)

---

<a id="kag-knowledge-augmented-generation"></a>

### KAG: Knowledge Augmented Generation

**Knowledge-Augmented Generation (KAG)** is an advanced framework designed to improve the performance of Large Language Models (LLMs) in professional and specialized domains. While similar to Retrieval-Augmented Generation (RAG), KAG focuses on integrating structured knowledge graphs (KGs) and logical reasoning rather than just retrieving unstructured text snippets.

**Core Architecture**

KAG typically operates through three primary layers, as seen in the KAG-Framework on GitHub:

- **KAG-Builder**: Acts as a "knowledge architect" by creating bidirectional links between knowledge graphs and original text chunks.
- **KAG-Solver**: A logic-guided reasoning engine that breaks complex queries into manageable, multi-step sub-tasks using "symbolic operators" like planning and reasoning.
- **KAG-Model**: A fine-tuned layer that optimizes the LLM’s ability to understand, infer, and generate responses aligned with both professional terminology and human language.

**Key Differences from RAG**

| **Feature** | **Traditional RAG** | **Knowledge-Augmented Generation (KAG)** |
| --- | --- | --- |
| **Data Source** | Primarily unstructured text blocks. | Unified graph of text, data, and expert rules. |
| **Search Method** | Vector similarity (keyword/pattern matching). | Semantic alignment and graph walks. |
| **Reasoning** | Limited; struggles with multi-hop questions. | High; uses multi-step logical reasoning. |
| **Accuracy** | Prone to "hallucinations" due to fragmented context. | High accuracy; facts are anchored to knowledge nodes. |

**Professional Use Cases**

KAG has been successfully implemented in high-stakes fields where accuracy is non-negotiable:

- **Healthcare**: Used in medical knowledge bases to manage millions of entities and relationships, achieving over 93% accuracy in interpreting medical indicators.
- **E-Government**: Implemented by Ant Group to help citizens access administrative info, showing a significant improvement in precision (91.6%) over traditional RAG.
- **Finance & Law**: Enables deep contextual understanding of ownership structures, transactions, or legal precedents that simple text retrieval might miss.

**GPTCache**

**What it is**: An open-source semantic cache library for speeding up and reducing cost in LLM applications by storing and reusing previous model responses.

**Key idea**: Instead of always calling the LLM API, cache responses and return them (or similar responses) on repeat or semantically similar requests — improving performance.

<u>**Important notes**</u>:

- Works with popular LLM interfaces (e.g., OpenAI, LangChain).
- Customizable cache behavior (embedding function, similarity measure, storage, eviction).
- Saves latency and cost by reusing previous outputs.

https://github.com/OpenSPG/KAG

📌 **KAG — Knowledge Augmented Generation**

**What it is**: A framework that enhances large language models (LLMs) with structured, logical knowledge from knowledge graphs for better reasoning and factual Q&A, especially in professional domain systems.

**Key idea**: Improves on Retrieval-Augmented Generation (RAG) by integrating structured knowledge and logical reasoning rather than relying on simple vector similarity retrieval.

<u>**Core features (for notes)**</u>:

- Uses a knowledge + text mutual indexing structure.
- Applies semantic reasoning and alignment to reduce noise.
- Supports multi-hop reasoning and more rigorous logical answers.

(**Framework built on the OpenSPG engine + large language models.)**

[Back to Contents](#contents)

---

<a id="gptcache-a-library-for-creating-semantic-cache-for-llm-queries"></a>

### GPTCache: A Library for Creating Semantic Cache for LLM Queries

**GPTCache** is an open-source library designed to create a semantic cache for Large Language Model (LLM) queries. It acts as a middle layer between your application and LLM services like OpenAI, reducing costs and response times by storing and reusing previously generated answers.

https://github.com/zilliztech/GPTCache

**Key Concepts & Features**

- **Semantic Caching**: Unlike traditional caches that require an exact text match, GPTCache uses embedding algorithms to convert queries into vectors. This allows it to identify and retrieve answers for queries that are meaningfully similar but not identical.
- **Modular Architecture**: The system is highly customizable, allowing you to choose your own:
  - **Embedding Functions**: To transform text into searchable vectors.
  - **Vector Stores**: Supports options like Milvus, FAISS, and Chroma.
  - **Similarity Evaluators**: To decide if a cached response is a "good enough" match for a new query.
- **Integration**: It offers native support for popular frameworks like LangChain and mirrors the OpenAI API interface for easy adoption.

**Primary Benefits**

- **Cost Reduction**: It minimizes the number of tokens sent to expensive LLM APIs.
- **Speed**: Cached responses are fetched in milliseconds, providing a 2-10x speedup over real-time LLM generation.
- **Reliability**: Helps bypass API rate limits by serving many requests locally.
- **Mocking Support**: Facilitates development and testing by allowing you to store mocked data without needing an active network connection.

**Potential Risks**

- **Semantic Inaccuracy**: There is a risk of "false positives" where the cache provides an answer that is semantically close but contextually incorrect.
- **Data Freshness**: Caching may return stale information for queries requiring real-time data.

For a deep dive into its implementation, you can explore the GPTCache GitHub repository or the official documentation.

📌 **GPTCache**

**What it is**: An open-source semantic cache library for speeding up and reducing cost in LLM applications by storing and reusing previous model responses.

**Key idea**: Instead of always calling the LLM API, cache responses and return them (or similar responses) on repeat or semantically similar requests — improving performance.

<u>**Important notes**</u>:

- Works with popular LLM interfaces (e.g., OpenAI, LangChain).
- Customizable cache behavior (embedding function, similarity measure, storage, eviction).
- Saves latency and cost by reusing previous outputs.

[Back to Contents](#contents)

---

<a id="neo4j"></a>

### Neo4j

**Neo4j** is a graph database—which means it stores data as a network of connected things instead of rows and columns.

**Think of it like this**:

```text
instead of tables, you have nodes (things) and relationships (how those things connect). This makes Neo4j especially good at answering questions about connections.
```

**The basics**

**Neo4j uses the property graph model**:

- **Nodes → entities (people, products, places, accounts)**
- **Relationships → how nodes connect (FRIENDS\_WITH, BOUGHT, FOLLOWS)**
- **Properties → key-value data on both nodes and relationships**

**Example**:

```text
(Alice)-[:FRIENDS_WITH]->(Bob)
```

**Why people use Neo4j**

**Neo4j shines when relationships are the star of the show**:

- 🧑‍🤝‍🧑 **Social networks (who knows whom)**
- 🛒 **Recommendations (“users who bought this also bought…”)**
- 🕵️ **Fraud detection (suspicious connection patterns)**
- 🧠 **Knowledge graphs (linked facts and concepts)**
- 🧬 **Network analysis (paths, influence, dependencies)**

Queries like “find friends of friends” or “shortest path between two users” are fast and natural in Neo4j.

**Querying Neo4j**

Neo4j uses its own query language called Cypher.

**It’s designed to be readable and visual**:

```cypher
MATCH (a:Person {name: "Alice"})-[:FRIENDS_WITH]->(b)
RETURN b.name
```

That literally matches the shape of the graph. Very satisfying.

**How it compares to relational databases**

| **Traditional SQL DB** | **Neo4j** |
| --- | --- |
| **Tables & rows** | **Nodes & relationships** |
| **JOINs** | **Direct relationships** |
| **Slower for deep connections** | **Extremely fast for graph traversals** |

Neo4j isn’t meant to replace all databases—just the ones where relationships matter a lot.

[Back to Contents](#contents)

---

<a id="acid-atomicity-consistency-isolation-durability"></a>

### ACID (Atomicity, Consistency, Isolation, Durability)

An ACID database ensures high data integrity and reliability for transactions through four key properties: Atomicity (all-or-nothing execution), Consistency (valid state transitions), Isolation (independent concurrent operations), and Durability (permanent, recorded changes). Primarily associated with relational databases (RDBMS) like MySQL, PostgreSQL, and Oracle, ACID compliance is critical for applications requiring strict accuracy, such as banking or inventory systems.

**The Four ACID Properties**

- **Atomicity**: Treats a transaction as a single "unit," meaning it either succeeds completely or fails entirely and is rolled back.
- **Consistency**: Guarantees a transaction brings the database from one valid state to another, maintaining all predefined rules and constraints.
- **Isolation**: Ensures concurrent execution of transactions leaves the database in the same state as if they were executed sequentially.
- **Durability**: Guarantees that once a transaction is committed, it remains committed, even in the event of a power loss, crash, or error.

**ACID Database Examples & Characteristics**

- **SQL Databases**: Almost all traditional relational databases are ACID-compliant, including Oracle, MySQL, PostgreSQL, and Microsoft SQL Server.
- **NoSQL/Distributed**: While historically non-ACID, some modern databases and data platforms like Neo4j (graph) and Delta Lake (data lake) offer ACID transaction support.
- **Use Cases**: Essential for financial applications, online shopping, and any system where data integrity is paramount.
- **Trade-offs**: ACID-compliant databases can be more challenging to scale and may have lower performance compared to BASE (Basically Available, Soft state, Eventual consistency) systems.

[Back to Contents](#contents)

---

<a id="master-data-management-mdm"></a>

### Master Data Management (MDM)

**Master data management (MDM)** is a discipline in which business and information technology collaborate to ensure the uniformity, accuracy, stewardship, semantic consistency, and accountability of the enterprise's official shared master data assets.

(MDM) is a technology-enabled discipline that creates a single, trusted, and authoritative "golden record" for core business entities—such as customers, products, and suppliers. By integrating, cleansing, and de-duplicating data from multiple sources, MDM ensures consistent, high-quality information for reporting, analytics, and operational efficiency across an enterprise.

**Key Aspects of Master Data Management**

- **Definition**: MDM establishes a "single source of truth" for critical, slow-changing data (master data), separating it from rapidly changing transactional data.
- **Core Entities**: It covers "people" (customers, employees), "things" (products, assets), and "places" (offices, locations).
- **Data Governance**: MDM relies on strict data governance policies to manage data quality, security, and compliance with regulations like GDPR.
- **Benefits**: Key advantages include reduced data errors, improved regulatory compliance, enhanced customer experience (via accurate CRM data), and better, faster business decision-making.
- **Technology/Process**: MDM involves data integration, modeling, cleansing, and matching tools to merge data into a consolidated, reliable repository.

**Common Use Cases**

- **Customer 360**: Creating a unified view of customers across CRM, billing, and support systems.
- **Product Information Management (PIM)**: Ensuring uniform product descriptions and specifications.
- **Supplier Management**: Managing supplier data to improve supply chain efficiency and compliance.

[Back to Contents](#contents)

---

<a id="time-to-live-ttl"></a>

### Time-to-Live (TTL)

https://www.cloudflare.com/learning/cdn/glossary/time-to-live-ttl/

**What is time-to-live (TTL)? | TTL definition**

- Time to live (TTL) refers to the amount of time or “hops” that a packet is set to exist inside a network before being discarded by a router. TTL is also used in other contexts including CDN caching and DNS caching.

**How does TTL work?**

- When a packet of information is created and sent out across the Internet, there is a risk that it will continue to pass from router to router indefinitely. To mitigate this possibility, packets are designed with an expiration called a time-to-live or hop limit. Packet TTL can also be useful in determining how long a packet has been in circulation, and allow the sender to receive information about a packet’s path through the Internet.
- Each packet has a place where it stores a numerical value determining how much longer it should continue to move through the network. Every time a router receives a packet, it subtracts one from the TTL count and then passes it onto the next location in the network. If at any point the TTL count is equal to zero after the subtraction, the router will discard the packet and send an ICMP message back to the originating host.
- The commonly used network commands ping and traceroute both utilize TTL. When using the traceroute command, a stream of packets with increasingly higher sequential TTLs are sent across the Internet towards a destination. Because each step along the connection is the last stop for one of the packets, each location will return an ICMP message to the sender after discarding the packet. The time it takes for the ICMP message to return to the sender is then used to determine how long it takes to get to each successive hop along the network.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-065.png)

**In what other circumstances is TTL used?**

- Apart from tracing the route packets take across the Internet, time-to-live is used in the context of caching information for a set period. Instead of measuring time in hops between routers, each of which can take a variable amount of time, some networking use cases operate in a more traditional fashion.
- CDN's like Cloudflare's CDN commonly use a TTL to determine how long cached content should be served from a CDN edge server before a new copy will be fetched from an origin server. By properly setting the amount of time between origin server pulls, a CDN is able to serve updated content without requests continuously propagating back to the origin. This optimization allows a CDN to efficiently serve content closer to a user while reducing the bandwidth required from the origin.
- In the context of a DNS record, TTL is a numerical value that determines how long a DNS cache server can serve a DNS record before reaching out to the authoritative DNS server and getting a new copy of the record.

[Back to Contents](#contents)

---

<a id="observability-distributed-operations"></a>

## Observability & Distributed Operations

<a id="grafana-widgets"></a>

### Grafana Widgets

**Grafana widgets (commonly referred to as panels)** are the modular, interactive visualization building blocks used to display data on a dashboard. They transform data from various sources (SQL, API, Prometheus) into visual formats like graphs, gauges, tables, and maps to monitor metrics. These customizable elements allow for real-time data analysis, alerting, and trend tracking.

**Key Aspects of Grafana Widgets (Panels)**:

**Types of Visualizations**:

- **Time Series**: Line graphs showing trends over time.
- **Stat/Gauge**: Displays single metric values or progress against thresholds.
- **Bar/Pie Charts**: Used for categorical or proportional data.
- **Heatmaps/Geomaps**: Visualizes two-dimensional or geographical data.
- **Table**: Displays raw data in a structured format.
- **Text/News**: Displays static text, markdown, or alerts.

**Key Features**:

- **Customization**: Extensive options to adjust axes, colors, legends, and styling.
- **Data Transformation**: Capabilities to modify data before visualization.
- **Interactivity**: Allows filtering, zooming, and navigating between dashboards.
- **Independent Operation**: Some plugins (like text or news) can operate without a data source.
- **Purpose**: They allow users to create a, at-a-glance view of related information to troubleshoot faster and reduce monitoring costs.

[Back to Contents](#contents)

---

<a id="grafana-spans"></a>

### Grafana Spans

https://grafana.com/docs/grafana/latest/datasources/tempo/tracing-best-practices/

📊 **Spans in Grafana**

**Definition**: A Span represents one operation or step inside a request in distributed tracing. It shows how long that specific part took and what happened during it.

🔹 **Key Idea**

- A Trace = the entire request journey through services.

- A Span = one piece of work inside that trace.

👉 **Think of it like**:

```text
Trace = whole trip
Spans = each stop on the trip
```

🔹 **What a Span Contains**

**Typical information inside a span**:

- **Operation Name – what the service/function is doing**

- **Start Time – when it started**

- **Duration – how long it took**

- **Service Name – which service handled it**

- **Tags / Attributes – metadata (HTTP method, DB query, etc.)**

- **Logs / Events – events during execution**

🔹 **Span Types (Common)**

- **Root Span – first span of a trace**

- **Child Span – created by another span**

- **Sibling Spans – spans with the same parent**

**Example hierarchy**:

```text
Trace (User request)
 └── Span: API Gateway
       ├── Span: Auth Service
       ├── Span: Product Service
       │      └── Span: Database Query
       └── Span: Cache Check
```

🔹 **Why Spans Are Useful**

**Spans help you**:

- 🔍 **Find slow services**

- 🐞 **Debug failures**

- 📈 **Analyze request latency**

- 🧩 **Understand microservice interactions**

🔹 **In Grafana**

**In Grafana’s tracing UI (Tempo/Jaeger)**:

- **Spans appear as bars in a timeline**

- **Each bar shows duration and dependency**

- **You can expand spans to inspect metadata**

✅ **One-line summary**:

```text
A Span in Grafana is a single timed operation within a distributed trace that shows what a service did and how long it took.
```

[Back to Contents](#contents)

---

<a id="prometheus-grafana"></a>

### Prometheus & Grafana

**Prometheus and Grafana** are two widely used, open-source, and highly compatible tools designed for monitoring and visualizing the health of systems and applications.

Prometheus is the data collector and storage engine. It pulls metrics from systems, stores them as time-series data, and handles alerting.

Grafana is the visualization and analysis platform. It creates, queries, and shares customizable dashboards based on data provided by Prometheus (or other sources).

Together, they form a complete "observability" stack—Prometheus tracks the what and when, while Grafana displays the how.

**Prometheus**: The Data Collector & Database

Prometheus, a CNCF graduated project, is a time-series database designed for reliability and scalability in dynamic environments, especially Kubernetes.

**Key Concepts**:

- **Metrics**: Numerical data representing system performance (e.g., CPU usage, memory, request counts).

- **Time Series Data**: Data points, each with a timestamp, allowing for tracking changes over time.

- **Scraping (Pull Model)**: Prometheus scrapes (pulls) metrics from applications/servers by hitting specific HTTP endpoints, usually every few seconds.

- **Exporters**: Specialized tools (like Node Exporter for servers or cAdvisor for containers) that translate native system metrics into a format Prometheus can understand.

- **PromQL (Prometheus Query Language)**: A powerful, flexible language used to query, aggregate, and manipulate the collected time-series data.

- **Alertmanager**: A separate, connected tool that processes alerts sent by Prometheus, routing them to platforms like Slack, PagerDuty, or Email.

**Grafana**: The Visualization Platform

Grafana is a dashboarding tool that turns raw Prometheus data into visual, actionable insights. While it can work with many databases, its pairing with Prometheus is standard in the industry.

**Key Features**:

- **Dashboards**: Customizable, interactive dashboards with panels for graphs, charts, tables, and gauges.

- **Data Source Integration**: It acts as a "single pane of glass" able to query data not just from Prometheus, but also from SQL databases, Elasticsearch, or Loki.

- **Alerting & Annotations**: It can set alert thresholds based on visual data and display annotations to correlate events (like a code deployment) with system performance dips.

- **Templating & Variables**: Allows for dynamic, reusable dashboards (e.g., one dashboard to monitor multiple, different microservices).

**How They Work Together**

- **Instrument**: An application is instrumented to expose metrics at a /metrics endpoint (often using Prometheus client libraries).

- **Collect**: Prometheus scrapes these endpoints at regular intervals and stores the data in its time-series database.

- **Visualize**: Grafana is configured to connect to the Prometheus server as a "Data Source".

- **Query & Display**: Users write PromQL queries within Grafana panels to display metrics in real-time graphs.

- **Alert**: Prometheus evaluates alerting rules. When a threshold is met (e.g., CPU &gt; 90%), it sends a notification to the Alertmanager, which sends a notification to the team.

**Summary of Differences**

| **Feature** | **Prometheus** | **Grafana** |
| --- | --- | --- |
| **Primary Goal** | **Data Acquisition & Storage** | **Data Visualization & Analysis** |
| **Type** | **Time-series Database** | **Monitoring & Observability Platform** |
| **Data Handling** | **Pulls and stores metrics** | **Queries and displays metrics** |
| **Interface** | **Basic ("Expression Browser")** | **Rich, Customizable Dashboards** |
| **Alerting** | **Alertmanager (Rule evaluation)** | **Alerting & Notification Management** |
| **Query Language** | **PromQL** | **Uses PromQL (and SQL/others)** |

**Why use both?**

Prometheus alone is not a full-fledged dashboarding solution; it is designed for storage and alerts. Grafana fills the void by providing the necessary visualization, UI, and sharing capabilities needed to make sense of that data, making them the "perfect combo".

[Back to Contents](#contents)

---

<a id="event-bus"></a>

### Event Bus

**An event bus** is a communication mechanism that allows different components of a system to exchange messages or events without needing to know about each other, facilitating decoupled interactions.

**Definition and Functionality**

- An event bus serves as a centralized communication hub that enables various components (or services) to communicate through events. It operates on the publish-subscribe pattern, where components can publish events to the bus without knowing who will consume them, and subscribers can listen for events without knowing who published them. This decoupling allows for greater flexibility and scalability in software architecture.

**How It Works**

- **Event Publishing**: When a significant change occurs in a system (e.g., a user action, a data update), an event is published to the event bus. This event contains all necessary information for subscribers to process it.
- **Event Subscription**: Components that are interested in specific events subscribe to the event bus. When an event matching their criteria is published, they receive it and can take appropriate actions, such as updating a database or triggering another process.
- **Decoupling**: The event bus allows for loose coupling between components, meaning that changes in one component do not directly affect others. This is particularly useful in microservices architectures, where services can evolve independently.

**Use Cases**

- **Microservices Communication**: In microservices architectures, event buses facilitate communication between services, allowing them to react to events without direct dependencies.
- **Real-Time Data Processing**: Event buses are used in systems that require real-time processing of events, such as financial transactions or user interactions on websites.
- **Event-Driven Architectures**: They are a core component of event-driven architectures (EDA), enabling systems to respond quickly to changes and events in the environment.

**Conclusion**

- In summary, an event bus is a powerful tool for enabling asynchronous communication between components in a software system. By decoupling event producers and consumers, it enhances flexibility, scalability, and maintainability in complex applications. Whether in cloud services like AWS EventBridge or custom implementations, event buses play a crucial role in modern software design.

[Back to Contents](#contents)

---

<a id="messaging-systems-message-queue-mq-event-bus-message-broker"></a>

### Messaging Systems. Message Queue (MQ), Event Bus & Message Broker

```text
Messaging Systems
│
├── Message Queue
├── Pub/Sub
│   └── Event Bus
├── Message Broker (infrastructure)
├── Event Streaming Platform
├── Service Bus / ESB(Enterprise Service Bus)
│
└── Architectural Patterns
    ├── Event Sourcing
    ├── CQRS(Command Query Responsibility Segregation)
    ├── Saga
```

**1️⃣ Message Queue**

**What it is**

- **A Message Queue (MQ) is a mechanism for sending messages between systems where**:
  - **A producer sends a message to a queue**
  - **A consumer reads from that queue**
  - **Each message is typically processed by one consumer**

**Key Characteristics**

- **Point-to-point**

- **Usually one consumer per message**

- **Messages are removed after processing**

- **Often used for background jobs**

**Typical Use Cases**

- **Order processing**

- **Email sending**

- **Payment processing**

- **Task scheduling**

**Examples**

- **RabbitMQ**

- **Amazon SQS**

- **ActiveMQ**

**2️⃣ Message Broker**

**What it is**

A Message Broker is a broader system that manages message exchange between applications.

**It can**:

- **Host queues**

- **Route messages**

- **Transform messages**

- **Implement pub/sub**

- **Handle retries, persistence, dead-letter queues**

👉 Think of a message queue as a feature, and a message broker as the full system that provides messaging infrastructure.

**Key Characteristics**

- **Manages multiple communication patterns**

- **Handles routing logic**

- **Provides durability, delivery guarantees, scaling**

**Examples**

- **RabbitMQ**

- **Apache Kafka**

- **Azure Service Bus**

**3️⃣ Event Bus**

**What it is**

An Event Bus is an architectural pattern (or system) that distributes events to multiple subscribers using publish–subscribe (pub/sub).

**Unlike a queue**:

- **One event can be delivered to many subscribers**

- **Consumers react independently**

- **Producers and consumers don’t know about each other**

**Key Characteristics**

- **Broadcast model**

- **Enables event-driven architecture (EDA)**

- **Strong decoupling**

- **Often used in microservices**

**Examples**

- **AWS EventBridge**

- **Google Cloud Pub/Sub**

- **Apache Kafka**

🎯 **Core Differences (Simple Table)**

| **Feature** | **Message Queue** | **Message Broker** | **Event Bus** |
| --- | --- | --- | --- |
| **Communication model** | **Point-to-point** | **Infrastructure** | **Publish–Subscribe** |
| **Message consumers** | **Usually, 1** | **1 or many** | **Many** |
| **Message removal** | **After consumption** | **Depends** | **Often retained (event streaming systems)** |
| **Purpose** | **Task distribution** | **Message management** | **Event distribution** |
| **Architecture style** | **Asynchronous processing** | **Messaging middleware** | **Event-driven architecture** |

🔎 **Relationship Between Them**

**Think of it like this**:

- **Message Queue → a mailbox**

- **Event Bus → a radio station**

- **Message Broker → the postal system + radio tower infrastructure**

**A broker can**:

- **Host queues**

- **Implement event buses**

- **Support multiple messaging patterns**

💡 **When to Use What**

**Use a Message Queue when**:

- **You need reliable background processing**

- **Only one service should process a task**

- **You want work distribution among workers**

**Use an Event Bus when**:

- **Many services need to react to an event**

- **You’re building microservices**

- **You want loose coupling and scalability**

**Use a Message Broker when**:

- **You need full messaging infrastructure**

- **You want routing, retries, monitoring, scaling**

- **You’re building a distributed system**

🚀 **Final Answer**

**No — they are not the same**:

- A message queue is a messaging pattern/mechanism.

- A message broker is the system that manages message exchange.

- An event bus is a pub/sub pattern for distributing events.

- They overlap — but they serve different architectural roles.

[Back to Contents](#contents)

---

<a id="security-identity-compliance"></a>

## Security, Identity & Compliance

<a id="vpn-server"></a>

### VPN Server

https://www.wireguard.com/
https://openvpn.net/
https://developers.cloudflare.com/cloudflare-one/team-and-resources/devices/warp/download-warp/

🌐 <u>**VPN Server — Quick Notes**</u>

**Definition**: A VPN server is a remote computer that receives encrypted traffic from a VPN client and forwards it to the internet.

**Main role**:

- **Acts as an intermediary between your device and the internet**
- **Decrypts your traffic coming from the VPN tunnel**
- **Sends requests to websites on your behalf**

**What it provides**:

- 🔐 **Encryption endpoint (end of the secure tunnel)**
- 🕵️ **IP address masking (websites see the server’s IP, not yours)**
- 🌍 **Virtual location (server’s country/location)**

**How it works (simple flow)**:

```text
Your device → encrypted tunnel → VPN server → internet
```

**Where it runs**:

- **Cloud servers (AWS, DigitalOcean, etc.)**
- **Company infrastructure**
- **VPN provider data centers**
- **Self-hosted personal servers**

**What it is NOT**:

- **Not the same as a VPN client (client runs on your device)**
- **Not just a protocol (protocol = rules; server = machine)**

**Common software used**:

- **WireGuard server**
- **OpenVPN server**
- **IPsec/L2TP servers**

<u>**Key point to remember**</u>:

```text
👉 No VPN server = no real VPN, only encrypted networking at best.
```

[Back to Contents](#contents)

---

<a id="payment-card-industry-data-security-standard-pci-dss"></a>

### Payment Card Industry Data Security Standard (PCI DSS)

**The Payment Card Industry Data Security Standard (PCI DSS)** is a global data security standard that regulates how entities store, process, and transmit cardholder data (CHD) and/or sensitive authentication data (SAD). PCI DSS includes guidelines regarding components of organizations' technical and operational system that are related to such data. Cardholder Data refers to information including Primary Account Numbers (PAN), cardholder names, expiration dates, and service codes. Sensitive authentication data refers to information including "full track data (magnetic-stripe data or equivalent on a chip)," card verification codes, and PINs/PIN blocks. This standard is administered by the Payment Card Industry Security Standards Council, and its use is enforced by the major payment card brands. PCI DSS was created to improve and streamline the security controls organizations use when handling cardholder data and reduce credit card fraud. These organizations, including merchants and service providers, must prove compliance to the PCI DSS through an assessment and validation process. The payment card brands issue fines and other penalties when merchants or service providers fail to prove compliance. Validation of compliance is performed annually or quarterly with a method suited to the organization's volume of transactions:

- **Self-assessment questionnaire (SAQ)**
- **Firm-specific Internal Security Assessor (ISA)**
- **External Qualified Security Assessor (QSA)**

PCI DSS scope defines the specific people, processes, and technologies that store, process, or transmit cardholder data (CHD), or can impact the security of the Cardholder Data Environment (CDE). It includes connected systems, network components, and personnel, requiring all 12 PCI DSS requirements to be met for these assets to ensure secure payment processing.

**Components in Scope**

- **Technology**: Servers, workstations, databases, routers, switches, and security appliances (firewalls) connected to or within the CDE.
- **Data**: Primary Account Numbers (PAN), cardholder name, expiration date, and Sensitive Authentication Data (SAD).
- **People**: Staff with administrative access to the CDE or access to cardholder data.
- **Processes**: Procedures for handling payment data and managing security.

**Scoping Key Principles**

- **Connected Systems**: Any system that connects to the CDE—even if it does not directly handle data—is considered in scope.
- **Impact**: Systems that can affect the security of the CDE (e.g., security monitoring tools) are in scope.
- **Segmentation**: Utilizing network segmentation (e.g., firewalls) to isolate the CDE can significantly reduce the scope.
- **Consequences**: Incorrect scoping leads to unnecessary costs (too broad) or security breaches (too narrow).

**Reducing Scope**

- Reducing the scope reduces compliance efforts, risks, and costs. Effective methods include using PCI-compliant third-party payment processors, implementing tokenization, or strict network segmentation.

[Back to Contents](#contents)

---

<a id="write-once-read-many-worm"></a>

### Write once read many (WORM)

Write once read many (WORM) describes a data storage device in which information, once written, cannot be modified. This write protection affords the assurance that the data cannot be tampered with once it is written to the device, excluding the possibility of data loss from human error, computer bugs, or malware.

On ordinary (non-WORM) data storage devices, the number of times data can be modified is limited only by the lifespan of the device, as modification involves physical changes that may cause wear to the device. The "read many" aspect is unremarkable, as modern storage devices permit unlimited reading of data once written. Historical exceptions include time-limited discs such as Flexplay, designed for short-term rental of movies; and early non-volatile memory technologies such as magnetic-core memory and bubble memory, from which reading data also erased it.

WORM prevents important data being deleted or modified, helping to preserve its authenticity.

[Back to Contents](#contents)

---

<a id="role-based-access-control-rbac"></a>

### Role-Based Access Control (RBAC)

**In computer systems security, role-based access control (RBAC) or role-based security** is an approach to restricting system access to authorized users, and to implementing mandatory access control (MAC) or discretionary access control (DAC).

Role-based access control is a policy-neutral access control mechanism defined around roles and privileges. The components of RBAC such as role-permissions, user-role and role-role relationships make it simple to perform user assignments. A study by NIST has demonstrated that RBAC addresses many needs of commercial and government organizations. RBAC can be used to facilitate administration of security in large organizations with hundreds of users and thousands of permissions. Although RBAC is different from MAC and DAC access control frameworks, it can enforce these policies without any complication.

[Back to Contents](#contents)

---

<a id="security-information-and-event-management-siem"></a>

### Security Information and Event Management (SIEM)

**Security information and event management (SIEM)** is a field within computer security that combines security information management (SIM) and security event management (SEM) to enable real-time analysis of security alerts generated by applications and network hardware. SIEM systems are central to security operations centers (SOCs), where they are employed to detect, investigate, and respond to security incidents. SIEM technology collects and aggregates data from various systems, allowing organizations to meet compliance requirements while safeguarding against threats. National Institute of Standards and Technology (NIST) definition for SIEM tool is application that provides the ability to gather security data from information system components and present that data as actionable information via a single interface.

SIEM tools can be implemented as software, hardware, or managed services. SIEM systems log security events and generating reports to meet regulatory frameworks such as the Health Insurance Portability and Accountability Act (HIPAA) and the Payment Card Industry Data Security Standard (PCI DSS). The integration of SIM and SEM within SIEM provides organizations with a centralized approach for monitoring security events and responding to threats in real-time.

First introduced by Gartner analysts Mark Nicolett and Amrit Williams in 2005, the term SIEM has evolved to incorporate advanced features such as threat intelligence and behavioral analytics, which allow SIEM solutions to manage complex cybersecurity threats, including zero-day vulnerabilities and polymorphic malware.

In recent years, SIEM has become increasingly incorporated into national cybersecurity initiatives. For instance, Executive Order 14028 signed in 2021 by U.S. President Joseph Biden mandates the use of SIEM technologies to improve incident detection and reporting in federal systems. Compliance with these mandates is further reinforced by frameworks such as NIST SP 800-92, which outlines best practices for managing computer security logs.

Modern SIEM platforms are aggregating and normalizing data not only from various Information Technology (IT) sources, but from production and manufacturing Operational Technology (OT) environments as well.

[Back to Contents](#contents)

---

<a id="general-data-protection-regulation-gdpr"></a>

### General Data Protection Regulation (GDPR)

**The General Data Protection Regulation (Regulation (EU) 2016/679), abbreviated GDPR,** is a European Union regulation on information privacy in the European Union (EU) and the European Economic Area (EEA). The GDPR is an important component of EU privacy law and human rights law, in particular Article 8(1) of the Charter of Fundamental Rights of the European Union. It also governs the transfer of personal data outside the EU and EEA. The GDPR's goals are to enhance individuals' control and rights over their personal information and to simplify the regulations for international business. It supersedes the Data Protection Directive 95/46/EC and, among other things, simplifies the terminology.

The European Parliament and Council of the European Union adopted the GDPR on 14 April 2016, to become effective on 25 May 2018. As an EU regulation (instead of a directive), the GDPR has direct legal effect and does not require transposition into national law. However, it also provides flexibility for individual member states to modify (derogate from) some of its provisions.

As an example of the Brussels effect, the regulation became a model for many other laws around the world, including in Brazil, Japan, Singapore, South Africa, South Korea, Sri Lanka, and Thailand. After leaving the European Union the United Kingdom enacted its "UK GDPR", identical to the GDPR. The California Consumer Privacy Act (CCPA), adopted on 28 June 2018, has many similarities with the GDPR.

[Back to Contents](#contents)

---

<a id="self-assessment-questionnaires-saqs"></a>

### Self-Assessment Questionnaires (SAQs)

**Self-Assessment Questionnaires (SAQs)** are compliance validation tools used in the Payment Card Industry Data Security Standard (PCI DSS) framework. They allow merchants and service providers to evaluate their own compliance with PCI DSS requirements when handling cardholder data.

🔹 **SAQ-A**

**Who is it for?**

**Merchants who**:

- Fully outsource all cardholder data processing to a PCI DSS–validated third party.
- Do NOT store, process, or transmit cardholder data on their own systems.
- Have no electronic storage of cardholder data.
- Use payment pages hosted entirely by a third-party provider (e.g., redirect or iframe fully controlled by provider).

**Typical Example**:

- **A small online store that redirects customers to a hosted payment page like**:
  - **Stripe**
  - **PayPal**
  - **Square**

The merchant’s website never touches card data directly.

**Compliance Scope**:

- ✅ **Very limited**
- ✅ **Fewer PCI DSS requirements (around 20–30 controls depending on version)**
- ✅ **Simplest SAQ type**

🔹 **SAQ-A-EP (E-Commerce Payment)**

**Who is it for?**

**E-commerce merchants who**:

- **Outsource payment processing, BUT**

Their website can impact the security of the payment transaction.

**This includes merchants who**:

- **Use embedded payment forms (e.g., JavaScript payment fields)**
- **Control the page that loads the payment form**
- **Host scripts that influence the checkout process**

**Key Difference**:

Even though card data may go directly to a third party, your website can affect payment security, so more PCI controls apply.

**Typical Example**:

- **An online store that**:
  - **Hosts its own checkout page**
  - **Embeds Stripe or another provider’s payment field using JavaScript**
  - **Controls the webpage where customers enter card details**

**Compliance Scope**:

- ⚠️ **Larger scope than SAQ-A**
- ⚠️ **More PCI DSS controls (roughly 150+ requirements)**
- ⚠️ Stronger security controls required (web security, vulnerability scanning, segmentation, etc.)

📊 **Side-by-Side Comparison**

| **Feature** | **SAQ-A** | **SAQ-A-EP** |
| --- | --- | --- |
| **Card data touches merchant system?** | ❌ **No** | ❌ **No (but site can affect security)** |
| **Merchant hosts checkout page?** | ❌ **No** | ✅ **Yes** |
| **Redirect to PSP payment page?** | ✅ **Yes** | ❌ **Usually embedded** |
| **PCI requirements** | **Minimal** | **Extensive** |
| **Security validation complexity** | **Low** | **Moderate to High** |

🔎 **Simple Rule of Thumb**

👉 **If customers leave your website to pay → SAQ-A**

👉 If payment happens on your website (even if processed by someone else) → SAQ-A-EP

[Back to Contents](#contents)

---

<a id="tls-1-2-data-in-transit-aes-256-data-at-rest"></a>

### TLS 1.2+ (Data in Transit) & AES-256 (Data at Rest)

🔐 **TLS 1.2+ (Data in Transit)**

**What it protects**:

Data while moving between systems (browser ↔ server, API ↔ API).

**What it is**: Transport Layer Security (TLS) version 1.2 or higher.

**Why required**:

- **Prevents eavesdropping**
- **Prevents tampering**
- **Verifies server identity (certificates)**

**Key points**:

- **Always use TLS 1.2 or 1.3**
- **Disable TLS 1.0 / 1.1**
- **Use strong cipher suites**
- **Certificates from trusted CA**

👉 **Think**: “Data on the wire.”

🔐 **AES-256 (Data at Rest)**

**What it protects**:

Stored data (databases, disks, backups, files).

**What it is**:

- **AES = Advanced Encryption Standard**
- **256 = 256-bit key (very strong)**

**Why used**:

- **Industry standard**
- **NIST approved**
- **Required for many compliance frameworks (PCI, HIPAA, etc.)**

**Where applied**:

- **Full disk encryption**
- **Database encryption**
- **Backup encryption**
- **Cloud storage encryption**

👉 **Think**: “Data sitting still.”

🔐 **Application-Level PII Encryption**

**What it protects**:

- **Sensitive personal data (PII) like**:
  - **SSN**
  - **Passport number**
  - **Card number**
  - **Health data**

**Where encryption happens**:

- **Inside the application before storing in DB**

<u>**Why important**</u>:

- **DB admins can’t see raw data**
- **Even if DB is breached → data still protected**
- **Adds extra layer beyond disk encryption**

**Flow Example**:

- **User enters SSN →**
- **App encrypts SSN →**
- **Encrypted value stored in DB**

👉 **Think**: “Encrypt before saving.”

🔑 **Key Management**

Encryption is only as strong as how keys are protected.

🏢 **HSM (Hardware Security Module)**

**What it is**:

Physical device that securely generates and stores encryption keys.

**Pros**:

- **Very high security**
- **Keys never leave hardware**
- **FIPS-certified**
- **Required for high-security environments (banks, PCI Level 1)**

**Cons**:

- **Expensive**
- **Complex setup**

👉 **Best for**: Financial institutions, large enterprises

💻 **Software Vault (e.g., cloud KMS, secrets manager)**

**What it is**:

Software-based key management system.

**Examples**:

- **AWS KMS**
- **Azure Key Vault**
- **HashiCorp Vault**

**Pros**:

- **Easier to manage**
- **Scalable**
- **Cheaper than HSM**
- **Good security if configured properly**

**Cons**:

- **Not physically isolated**
- **Depends on cloud/provider security**

👉 **Best for**: Most modern SaaS and cloud applications

⚖️ **HSM vs Software Vault (Quick Comparison)**

| **Feature** | **HSM** | **Software Vault** |
| --- | --- | --- |
| **Security level** | **Very high** | **High** |
| **Cost** | **Expensive** | **Moderate** |
| **Hardware-based** | **Yes** | **No** |
| **Compliance strength** | **Strongest** | **Strong** |
| **Ease of use** | **Complex** | **Easier** |

🧠 <u>**Simple Mental Model**</u>

- **TLS 1.2+ → Protect data while moving**
- **AES-256 → Protect data while stored**
- **App-level encryption → Protect sensitive fields before saving**
- **HSM / Vault → Protect the keys that protect the data**

[Back to Contents](#contents)

---

<a id="identity-provider-idp"></a>

### Identity Provider (IdP)

https://www.cloudflare.com/learning/access-management/what-is-an-identity-provider/

**What is an identity provider (IdP)?**

- An identity provider (IdP) is a service that stores and verifies user identity. IdPs are typically cloud-hosted services, and they often work with single sign-on (SSO) providers to authenticate users.
- An identity provider (IdP) stores and manages users' digital identities. Think of an IdP as being like a guest list, but for digital and cloud-hosted applications instead of an event. An IdP may check user identities via username-password combinations and other factors, or it may simply provide a list of user identities that another service provider (like an SSO) checks.
- IdPs are not limited to verifying human users. Technically, an IdP can authenticate any entity connected to a network or a system, including computers and other devices. Any entity stored by an IdP is known as a "principal" (instead of a "user"). However, IdPs are most often used in cloud computing to manage user identities.

**What is user identity?**

Digital user identity is associated with quantifiable factors that can be verified by a computer system. These factors are called "authentication factors." The three authentication factors are:

- **Knowledge**: something you know, such as a username and password
- **Possession**: something you have, such as a smartphone
- **Intrinsic qualities**: something you are, such as your fingerprint or a retina scan

An IdP may use one or more of these factors to identify a user. Using multiple factors to verify user identify is called multi-factor authentication (MFA).

**Why are IdPs necessary?**

- Digital identity must be tracked somewhere, especially for cloud computing, where user identity determines whether someone can access sensitive data. Cloud services need to know exactly where and how to retrieve and verify user identity.
- Records of user identities also need to be stored in a secured fashion to ensure that attackers cannot use them to impersonate users. A cloud identity provider will typically take extra precautions to protect user data, whereas a service not dedicated solely to storing identity may store it in an unsecured location, such as a server open to the Internet.

**How do IdPs work with SSO services?**

- A single sign-on service, often called an 'SSO,' is a unified place for users to sign in to all their cloud services at once. In addition to being more convenient for users, implementing SSO often makes user logins more secure.
- For the most part, SSOs and IdPs are separate. An SSO service uses an IdP to check user identity, but it does not actually store user identity. An SSO provider is more of a go-between than a one-stop shop; think of it as being like a security guard firm that is hired to keep a company secure but is not actually part of that company.
- Even though they are separate, IdPs are an essential part of the SSO login process. SSO providers check user identity with the IdP when users log in. Once that is done, the SSO can verify user identity with any number of connected cloud applications.
- However, this is not always the case. An SSO and IdP can theoretically be one and the same. But this setup is much more open to on-path attacks in which an attacker forges a SAML assertion\* to gain access to an application. For this reason, IdP and SSO are typically separated.

\*A SAML assertion is a specialized message sent from SSO services to any cloud application that confirms user authentication, allowing the user to access and use the application.

**How does all this look in practice?**

- Suppose Alice is using her work laptop at her employer's office. Alice needs to log in to the company's live chat application to better coordinate with her coworkers. She opens a tab on her browser and loads the chat application. Assuming her company uses an SSO service, the following steps take place behind the scenes:
  - The chat app asks the SSO for Alice's identity verification.
  - The SSO sees that Alice has not signed in yet.
  - The SSO prompts Alice to log in.
- At this point Alice's browser redirects her to the SSO login page. The page has fields for Alice to enter her username and password. Since her company requires two-factor authentication, Alice also must enter a short code that the SSO automatically texts to her smartphone. After this is done, she clicks "Log in." Now, the following things happen:
  - The SSO sends a SAML request to the IdP used by Alice's company.
  - The IdP sends a SAML response to the SSO confirming Alice's identity.
  - The SSO sends a SAML assertion to the chat application Alice originally wanted to use.
- Alice is redirected back to her chat application. Now she can chat with her coworkers. The whole process only took seconds.

**How does Cloudflare integrate with identity providers?**

- Cloudflare Zero Trust helps keep internal teams secure by integrating with SSOs and IdPs to manage user access.

**Types of identity providers**

- **OpenID provider**
  OpenID Connect (OIDC) is an identity layer on top of OAuth. In the domain model associated with OIDC, an identity provider is a special type of OAuth 2.0 authorization server. Specifically, a system entity called an OpenID Provider issues JSON-formatted identity tokens to OIDC relying parties via a RESTful HTTP API.
- **SAML identity provider**
  The Security Assertion Markup Language (SAML) is a set of profiles for exchanging authentication and authorization data across security domains. In the SAML domain model, an identity provider is a special type of authentication authority. Specifically, a SAML identity provider is a system entity that issues authentication assertions in conjunction with an SSO profile of SAML. A relying party that consumes these authentication assertions is called a SAML service provider.

[Back to Contents](#contents)

---

<a id="oauth"></a>

### OAuth

**OAuth (short for open authorization)** is an open standard for access delegation, commonly used as a way for internet users to grant websites or applications access to their information on other websites but without giving them the passwords. This mechanism is used by companies such as Amazon, Google, Meta Platforms, Microsoft, and Twitter to permit users to share information about their accounts with third-party applications or websites.

Generally, the OAuth protocol provides a way for resource owners to provide a client application with secure delegated access to server resources. It specifies a process for resource owners to authorize third-party access to their server resources without providing credentials. Designed specifically to work with Hypertext Transfer Protocol (HTTP), OAuth essentially allows access tokens to be issued to third-party clients by an authorization server, with the approval of the resource owner. The third party then uses the access token to access the protected resources hosted by the resource server.

[Back to Contents](#contents)

---

<a id="sso-mfa"></a>

### SSO & MFA

SSO simplifies access by letting users log in once for multiple applications, while MFA strengthens security by requiring multiple verification factors.

**What is Single Sign-On (SSO)?**

Single Sign-On (SSO) allows users to log in once and gain access to multiple applications or services without reentering credentials each time. It reduces password fatigue, improves productivity, and centralizes authentication management. Popular examples include logging in with Google, Microsoft, or Apple accounts to access multiple services like email, cloud storage, and collaboration tools. SSO works by issuing an authentication token after the initial login, which is then used to verify access across connected applications without repeated logins.

**What is Multi-Factor Authentication (MFA)?**

Multi-Factor Authentication (MFA) requires users to provide two or more verification factors to confirm their identity. These factors typically fall into three categories: something you know (password or PIN), something you have (security token, smartphone), and something you are (biometrics like fingerprint or facial recognition). MFA significantly enhances security because even if a password is compromised, unauthorized access is prevented without the additional factor.

**Key Differences Between SSO and MFA**

- **Purpose**: SSO focuses on convenience and streamlined access, while MFA focuses on verifying identity and preventing unauthorized access.
- **Security**: MFA provides stronger protection against account breaches, whereas SSO can create a single point of failure if credentials are compromised.
- **User Experience**: SSO improves workflow efficiency by reducing repeated logins, while MFA may add extra steps but ensures higher security.

**Using SSO and MFA Together**

- **Combining SSO and MFA offers the best of both worlds**: users enjoy seamless access to multiple applications while organizations maintain strong security. For example, an employee logs in once via SSO, and MFA verifies their identity at critical access points. This approach reduces password-related vulnerabilities, simplifies IT management, and helps monitor network activity effectively.

**Practical Benefits**

- **Reduces password fatigue and IT helpdesk tickets**
- **Strengthens security against stolen credentials and cyberattacks**
- **Centralizes identity management and access control**
- **Enhances user experience without compromising security**

**In summary, SSO and MFA are complementary**: SSO streamlines access, and MFA ensures that access is secure. Implementing both together is considered a best practice for modern cybersecurity strategies.

[Back to Contents](#contents)

---

<a id="security-information-and-event-management-siem-2"></a>

### Security Information and Event Management (SIEM)

https://en.wikipedia.org/wiki/Security\_information\_and\_event\_management

**Security information and event management (SIEM)** is a field within computer security that combines security information management (SIM) and security event management (SEM) to enable real-time analysis of security alerts generated by applications and network hardware. SIEM systems are central to security operations centers (SOCs), where they are employed to detect, investigate, and respond to security incidents. SIEM technology collects and aggregates data from various systems, allowing organizations to meet compliance requirements while safeguarding against threats. National Institute of Standards and Technology (NIST) definition for SIEM tool is application that provides the ability to gather security data from information system components and present that data as actionable information via a single interface.

Wazuh is the open-source security platform - unified XDR and SIEM protection for endpoints and cloud workloads.

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-066.png)

[Back to Contents](#contents)

---

<a id="mutual-tls-mtls"></a>

### Mutual TLS (mTLS)

https://www.cloudflare.com/learning/access-management/what-is-mutual-tls/

**Mutual TLS (mTLS)** is a type of authentication in which the two parties in a connection authenticate each other using the TLS protocol.

mTLS ensures that the parties at each end of a network connection are who they claim to be by verifying that they both have the correct private key. The information within their respective TLS certificates provides additional verification.

mTLS is often used in a Zero Trust security framework\* to verify users, devices, and servers within an organization. It can also help keep APIs secure&lt;.

\*Zero Trust means that no user, device, or network traffic is trusted by default, an approach that helps eliminate many security vulnerabilities.

**What is TLS?**

Transport Layer Security (TLS) is an encryption protocol in wide use on the Internet. TLS, which was formerly called SSL, authenticates the server in a client-server connection and encrypts communications between client and server so that external parties cannot spy on the communications.

<u>**There are three important things to understand about how TLS works**</u>:

**1. Public key and private key**

- TLS works using a technique called public key cryptography, which relies on a pair of keys — a public key and a private key. Anything encrypted with the public key can be decrypted only with the private key.
- Therefore, a server that decrypts a message that was encrypted with the public key proves that it possesses the private key. Anyone can view the public key by looking at the domain's or server's TLS certificate.

**2. TLS certificate**

- A TLS certificate is a data file that contains important information for verifying a server's or device's identity, including the public key, a statement of who issued the certificate (TLS certificates are issued by a certificate authority), and the certificate's expiration date.

**3. TLS handshake**

- The TLS handshake is the process for verifying the TLS certificate and the server's possession of the private key. The TLS handshake also establishes how encryption will take place once the handshake is finished.

**How does mTLS work?**

Normally in TLS, the server has a TLS certificate and a public/private key pair, while the client does not. The typical TLS process works like this:

- **Client connects to server**
- **Server presents its TLS certificate**
- **Client verifies the server's certificate**
- **Client and server exchange information over encrypted TLS connection**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-067.png)

In mTLS, however, both the client and server have a certificate, and both sides authenticate using their public/private key pair. Compared to regular TLS, there are additional steps in mTLS to verify both parties (additional steps in bold):

- **Client connects to server**
- **Server presents its TLS certificate**
- **Client verifies the server's certificate**
- **Client presents its TLS certificate**
- **Server verifies the client's certificate**
- **Server grants access**
- **Client and server exchange information over encrypted TLS connection**

![Embedded note image 1](./General-Active-Recall-Notes.assets/image-068.png)

**Certificate authorities in mTLS**

- The organization implementing mTLS acts as its own certificate authority. This contrasts with standard TLS, in which the certificate authority is an external organization that checks if the certificate owner legitimately owns the associated domain (learn about TLS certificate validation).
- A "root" TLS certificate is necessary for mTLS; this enables an organization to be their own certificate authority. The certificates used by authorized clients and servers must correspond to this root certificate. The root certificate is self-signed, meaning that the organization creates it themselves. (This approach does not work for one-way TLS on the public Internet because an external certificate authority must issue those certificates.)

**Why use mTLS?**

mTLS helps ensure that traffic is secure and trusted in both directions between a client and server. This provides an additional layer of security for users who log in to an organization's network or applications. It also verifies connections with client devices that do not follow a login process, such as Internet of Things (IoT) devices.

**mTLS prevents various kinds of attacks, including**:

- **On-path attacks**: On-path attackers place themselves between a client and a server and intercept or modify communications between the two. When mTLS is used, on-path attackers cannot authenticate to either the client or the server, making this attack almost impossible to carry out.
- **Spoofing attacks**: Attackers can attempt to "spoof" (imitate) a web server to a user, or vice versa. Spoofing attacks are far more difficult when both sides must authenticate with TLS certificates.
- **Credential stuffing**: Attackers use leaked sets of credentials from a data breach to try to log in as a legitimate user. Without a legitimately issued TLS certificate, credential stuffing attacks cannot be successful against organizations that use mTLS.
- **Brute force attacks**: Typically carried out with bots, a brute force attack is when an attacker uses rapid trial and error to guess a user's password. mTLS ensures that a password is not enough to gain access to an organization's network. (Rate limiting is another way to deal with this type of bot attack.)
- **Phishing attacks**: The goal of a phishing attack is often to steal user credentials, then use those credentials to compromise a network or an application. Even if a user falls for such an attack, the attacker still needs a TLS certificate and a corresponding private key to use those credentials.
- **Malicious API requests**: When used for API security, mTLS ensures that API requests come from legitimate, authenticated users only. This stops attackers from sending malicious API requests that aim to exploit a vulnerability or subvert the way the API is supposed to function.

[Back to Contents](#contents)

---

<a id="dlp-data-loss-prevention-filter"></a>

### DLP (Data Loss Prevention) Filter

**A DLP (Data Loss Prevention) filter** is a security mechanism used to detect and prevent unauthorized transmission of sensitive data—such as Personally Identifiable Information (PII), Protected Health Information (PHI), or intellectual property—across networks, endpoints, and cloud systems. These filters analyze data in motion, at rest, or in use, allowing administrators to block, log, or quarantine content based on predefined rules, file types, or regular expressions (regex).

**Key Aspects of DLP Filters**

- **Functionality**: They inspect data for specific patterns (e.g., credit card numbers, confidential keywords) and apply actions like blocking or monitoring.
- **Application Areas**: Common in network traffic (email, HTTP/FTP) and endpoint security (USB, printing).
- **Types of Filters**:
  - **File Type/Size Filters**: Block, shadow, or archive specific file types (PDF, Office docs) or files exceeding defined sizes.
  - **Expression/Keyword Filters**: Use Perl Compatible Regular Expressions (PCRE) to detect patterns within documents.
  - **Protocol Filters**: Monitor and control data leaving via web, mail, or file transfer protocols.
- **Use Cases**: Essential for compliance with regulations like GDPR, HIPAA, and CCPA.

**Alternatives (Context Dependent)**

- **DLP (Defects Liability Period)**: A construction term for a post-completion defect fixing period.
- **Projector Filter**: A UV filter used to manage light from a halogen lamp in a projector.

**Common Implementations**

- **Fortinet/FortiGate**: Uses "sensors" to inspect traffic.
- **Microsoft Purview**: Uses activity explorers to monitor sensitive data.
- **Ivanti**: Uses device control for endpoint security.

[Back to Contents](#contents)

---

<a id="short-lived-leases-in-vault"></a>

### Short-Lived Leases in Vault

Фраза «получение во время выполнения с short-lived leases» относится к тому, как сервис получает и использует секреты из HashiCorp Vault.

**Коротко**:

**Что это означает**

- **Секрет не хранится постоянно**
  Он не лежит в коде, config-файлах или environment variables.
  Сервис запрашивает его у Vault во время работы (runtime), когда он действительно нужен.
- **Vault выдает временный доступ**
  Вместо постоянного ключа Vault выдает временный secret / credential.
  У него есть TTL (time-to-live) — например 1 час.
- **Это и есть lease**
  Vault возвращает secret + lease ID.
  **Lease означает**: этот секрет действителен только в течение заданного времени.
- **После истечения TTL**
  **секрет автоматически становится недействительным**
  **сервис должен**:
  - **renew lease (продлить), или**
  - **получить новый secret**

**Упрощенный поток**

```text
Service → Vault: "дай мне LLM API key"
Vault → Service:
    secret = api_key_xxx
    lease_id = abc123
    ttl = 1 hour
```

**Через 1 час**:

```text
secret автоматически инвалидируется
```

**Сервис делает**:

```text
renew lease
или
получает новый secret
```

**Почему это важно для безопасности**

- **Это снижает риск утечки**:

| **Без leases** | **С short-lived leases** |
| --- | --- |
| **ключ может жить месяцами** | **ключ живет ~1 час** |
| **если украли — долго работает** | **быстро перестает работать** |
| **сложно отозвать** | **Vault может revoke instantly** |

**Пример в вашем pipeline**

**В шаге LLM Call**:

```text
LLM Call
   │
   ▼
Service → Vault: request Azure OpenAI key
Vault → temporary key (TTL = 1h)
   │
   ▼
Call Azure OpenAI
```

Если сервис скомпрометирован — ключ очень быстро перестанет быть валидным.

💡 **В одном предложении**:

```text
Short-lived leases означают, что сервис получает временные секреты из Vault во время выполнения, и эти секреты автоматически истекают через короткое время (например 1 час), уменьшая риск их компрометации.
```

[Back to Contents](#contents)

---

<a id="json-web-token-jwt"></a>

### JSON Web Token (JWT)

**JSON Web Token (JWT, suggested pronunciation /dʒɒt/, same as the word "jot")** is a proposed Internet standard for creating data with optional signature and/or optional encryption whose payload holds JSON that asserts some number of claims. The tokens are signed either using a private secret or a public/private key.

- For example, a server could generate a token that has the claim "logged in as administrator" and provide that to a client. The client could then use that token to prove that it is logged in as admin. The tokens can be signed by one party's private key (usually the server's) so that any party can subsequently verify whether the token is legitimate. If the other party, by some suitable and trustworthy means, is in possession of the corresponding public key, they too can verify the token's legitimacy. The tokens are designed to be compact, URL-safe, and usable, especially in a web-browser single-sign-on (SSO) context. JWT claims can typically be used to pass identity of authenticated users between an identity provider and a service provider, or any other type of claims as required by business processes.
- **JWT relies on other JSON-based standards**: JSON Web Signature and JSON Web Encryption.

**Structure**

**Header**

- Identifies which algorithm is used to generate the signature. In the below example, HS256 indicates that this token is signed using HMAC-SHA256.
- Typical cryptographic algorithms used are HMAC with SHA-256 (HS256) and RSA signature with SHA-256 (RS256). JWA (JSON Web Algorithms) RFC 7518 introduces many more for both authentication and encryption.

```json
    {
      "alg": "HS256",
      "typ": "JWT"
    }
```

**Payload**

- Contains a set of claims. The JWT specification defines seven Registered Claim Names, which are the standard fields commonly included in tokens. Custom claims are usually also included, depending on the purpose of the token.
- This example has the standard Issued At Time claim (iat) and a custom claim (loggedInAs).

```json
    {
      "loggedInAs": "admin",
      "iat": 1422779638
    }
```

**Signature**

- Securely validates the token. The signature is calculated by encoding the header and payload using Base64url Encoding RFC 4648 and concatenating the two together with a period separator. That string is then run through the cryptographic algorithm specified in the header. This example uses HMAC-SHA256 with a shared secret (public key algorithms are also defined). The Base64url Encoding is like base64, but uses different non-alphanumeric characters and omits padding.

```text
    HMAC_SHA256(
      secret,
      base64urlEncoding(header) + '.' +
      base64urlEncoding(payload)
    )
```

The three are encoded separately using Base64url Encoding RFC 4648, and concatenated using periods to produce the JWT:

```typescript
const token: string = base64urlEncoding(header) + '.' + base64urlEncoding(payload) + '.' + base64urlEncoding(signature)
```

**The above data and the secret of "secretkey" creates the token**:

```text
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJsb2dnZWRJbkFzIjoiYWRtaW4iLCJpYXQiOjE0MjI3Nzk2Mzh9.gzSraSYS8EXBxLN
_oWnFSRgCzcmJmMjLiuyu5CSpyHI
```

(The above json strings are formatted without newlines or spaces, into utf-8-byte arrays. This is important as even slight changes in the data will affect the resulting token)

This resulting token can be easily passed into HTML and HTTP.

**Use**

In authentication, when a user successfully logs in, a JSON Web Token (JWT) is often returned. This token should be sent to the client using a secure mechanism like an HTTP-only cookie. Storing the JWT locally in browser storage mechanisms like local or session storage is discouraged. This is because JavaScript running on the client-side (including browser extensions) can access these storage mechanisms, exposing the JWT and compromising security. To make use of the HTTP-only cookie, as you might need it to authenticate with cross-origin APIs, the best approach is to use the credentials property to tell the browser to automatically send the cookies to the external APIs via a Fetch call like so:

```javascript
fetch('https://api.example.com/data', {
  method: 'GET',
  credentials: 'include' // This tells the browser to include cookies, etc.
})
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error('Error:', error));
```

By using this method, the JWT is never exposed to client-side JavaScript; this is the best approach to make use of your JWT while maintaining security best practices. For unattended processes, the client may also authenticate directly by generating and signing its own JWT with a pre-shared secret and passing it to an OAuth compliant service like so:

```http
POST /oauth2/token
Content-type: application/x-www-form-urlencoded
grant_type=urn:ietf:params:oauth:grant-type:jwt-bearer&assertion=eyJhb...
```

If the client passes a valid JWT assertion the server will generate an access\_token valid for making calls to the application and pass it back to the client:

```json
{
  "access_token": "eyJhb...",
  "token_type": "Bearer",
  "expires_in": 3600
}
```

When the client wants to access a protected route or resource, the user agent should send the JWT, typically in the Authorization HTTP header using the Bearer schema. The content of the header might look like the following:

```text
Authorization: Bearer eyJhbGci...<snip>...yu5CSpyHI
```

This is a stateless authentication mechanism as the user state is never saved in server memory. The server's protected routes will check for a valid JWT in the Authorization header, and if it is present, the user will be allowed to access protected resources. As JWTs are self-contained, all the necessary information is there, reducing the need to query the database multiple times.

[Back to Contents](#contents)

---

<a id="static-application-security-testing-sast"></a>

### Static Application Security Testing (SAST)

**Static Application Security Testing (SAST)** is an automated white-box testing method that analyzes source code, byte code, or binaries for security vulnerabilities without executing the application. It is used early in the SDLC to identify flaws like SQL injection and XSS, enabling early, cost-effective remediation.

**Key Aspects of SAST**:

**Early Detection**: Scans code during development (often on commit) to find vulnerabilities before deployment.

**Methodology**: Analyzes the application "from the inside" by inspecting the code, making it effective for finding the root cause of issues.

**Benefits**: Covers 100% of the codebase, provides rapid feedback, and allows for automated, consistent enforcement of coding standards.

**Limitations**: Generally, it does not analyze the application in a running state and can produce false positives.

**SAST vs. DAST**:

**SAST**: Analyzes code without running it (Static).

**DAST**: Tests the running application from the outside (Dynamic).

[Back to Contents](#contents)

---
