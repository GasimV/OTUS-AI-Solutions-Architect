# Solution Architecture Document (SAD)

<a id="contents"></a>

## Contents

1. [Solution Architecture Document](#1-solution-architecture-document)
2. [Purpose of the SAD](#2-purpose-of-the-sad)
3. [Views of the SAD](#3-views-of-the-sad)
   - [3.1 SAD Views Figure](#31-sad-views-figure)
   - [3.2 Business View](#32-business-view)
   - [3.3 Logical View](#33-logical-view)
   - [3.4 Process View](#34-process-view)
   - [3.5 Deployment View](#35-deployment-view)
   - [3.6 Implementation View](#36-implementation-view)
   - [3.7 Data View](#37-data-view)
   - [3.8 Operational View](#38-operational-view)
4. [Structure of the SAD](#4-structure-of-the-sad)
   - [4.1 Solution Overview](#41-solution-overview)
   - [4.2 Business Context](#42-business-context)
   - [4.3 Conceptual Solution Overview](#43-conceptual-solution-overview)
   - [4.4 Solution Architecture](#44-solution-architecture)
     - [4.4.1 Information Architecture](#441-information-architecture)
     - [4.4.2 Application Architecture](#442-application-architecture)
     - [4.4.3 Data Architecture](#443-data-architecture)
     - [4.4.4 Integration Architecture](#444-integration-architecture)
     - [4.4.5 Infrastructure Architecture](#445-infrastructure-architecture)
     - [4.4.6 Security Architecture](#446-security-architecture)
   - [4.5 Solution Implementation](#45-solution-implementation)
   - [4.6 Solution Management](#46-solution-management)
   - [4.7 Appendix](#47-appendix)
5. [Life Cycle of the SAD](#5-life-cycle-of-the-sad)
6. [SAD Best Practices and Common Pitfalls](#6-sad-best-practices-and-common-pitfalls)
7. [IT Procurement Documentation for a Solution Architecture](#7-it-procurement-documentation-for-a-solution-architecture)
   - [7.1 RFx Document Types](#71-rfx-document-types)
   - [7.2 Solutions Architect Role in RFP Work](#72-solutions-architect-role-in-rfp-work)
   - [7.3 Cloud CRM RFP Example](#73-cloud-crm-rfp-example)
8. [Summary](#8-summary)

---

## 1. Solution Architecture Document

During **solution architecture design and optimization**, the solutions architect must maintain consistent communication with stakeholders for successful application delivery.

> The architecture design must be understandable for both technical and non-technical stakeholders.

The **Solution Architecture Document (SAD)** provides an **end-to-end view of the application** and helps everyone stay on the same page. It addresses the needs of all stakeholders associated with the development of the application.

### SAD documentation areas

- **Purpose of the SAD**
- **Views of the SAD**
- **Structure of the SAD**
- **Life cycle of the SAD**
- **SAD best practices and common pitfalls**
- **IT procurement documentation for a solution architecture**
  - Request for Proposal (**RFP**)
  - Request for Information (**RFI**)
  - Request for Quotation (**RFQ**)

Solutions architects also participate in procurement-related documentation by providing input and feedback that support strategic decisions.

[Back to Contents](#contents)

---

## 2. Purpose of the SAD

Architecture documentation is often skipped or delayed, and teams may start implementation without understanding the overall architecture. The SAD provides a **broad view of the overall solution design** and keeps all stakeholders informed.

### Stakeholder usage

- **Project managers** rely on the SAD to oversee project coordination and progress.
- **Business analysts** use it to align the project with business requirements.
- **Technical teams**, including developers and IT professionals, refer to it while implementing and maintaining the proposed solution.
- **Senior management** uses the document to make informed strategic decisions.
- **Clients and end users** depend on the document to confirm that the project outcome meets their needs and expectations.

### SAD goals

The SAD helps to achieve the following goals:

- Communicate the **end-to-end application solution** to all stakeholders.
- Provide a **high-level overview of the architecture** and different views of the application design.
- Address service-quality requirements such as:
  - **Reliability**
  - **Security**
  - **Performance**
  - **Scalability**
- Provide traceability from the solution back to the business requirements.
- Show how the application meets **functional** and **non-functional requirements** (**NFRs**).
- Provide the solution views required for:
  - Design
  - Build
  - Testing
  - Implementation
- Define solution impacts for:
  - Estimation
  - Planning
  - Delivery
- Define the business process, continuation, and operations needed for uninterrupted work after production launch.

### Important SAD content

SADs define the **purpose and goal of the solution** and address critical components that implementation teams may otherwise overlook:

- **Solution constraints**
- **Assumptions**
- **Risks**

The solutions architect should write the document in clear language that business users can understand, while still connecting the business context with the technical design.

> Documentation helps retain knowledge during resource changes and makes the overall design process less dependent on specific people.

### Existing applications and modernization

For existing applications where modernization is needed, the SAD presents:

- An abstract view of the **current architecture**
- An abstract view of the **future architecture**
- A transition plan
- Existing system dependencies
- Potential risks discovered in advance
- Migration tools and technologies
- Resource planning needs for the new system

### Assessments and change record

The solutions architect may conduct assessments during solution design through:

- Proof of concept (**POC**)
- Market research
- Technology evaluation
- Architecture trade-off analysis

A SAD should list architecture assessments, their impact, and the selected technology choices. It should also present the conceptual view of the solution's **current state** and **target state**, while maintaining a change record.

[Back to Contents](#contents)

---

## 3. Views of the SAD

The solutions architect needs to create a SAD that both business and technical users can understand.

The SAD bridges the communication gap between business users and the development team by explaining the function of the overall application. A practical way to capture stakeholder input is to look at the problem from each stakeholder's perspective.

The solutions architect evaluates both **business** and **technical** aspects of architecture design to account for technical and non-technical user requirements.

### 3.1 SAD Views Figure

As illustrated below, a holistic overview of the SAD includes several views that cover different aspects derived from the business requirements.

![Solution Architecture Document Views](assets/sad-views.svg)

**Figure: SAD views**

### Diagram note

Solutions architects can choose standard diagrams to represent the views, for example:

- Unified Modeling Language (**UML**) diagrams
- Block diagrams
- Microsoft Visio diagrams

The diagram should be easy to read and understandable for both business and technical stakeholders.

### 3.2 Business View

The **Business View** addresses business concerns and business purpose.

- Shows the value proposition of the overall solution and product.
- Can present high-level business scenarios as a use case diagram.
- Describes stakeholders and required resources to execute the project.
- Can also be treated as a **use case view**.

### 3.3 Logical View

The **Logical View** presents the packages or logical components of the system so that business users and designers can understand the system structure.

- Shows the logical components of the system.
- Explains how multiple packages are connected.
- Shows how the user interacts with the system.
- Can show the order in which parts of the system should be built.

Example for a banking application:

```text
Security package
  -> Account package
  -> Loan package
  -> Other business modules
```

Each package represents a module and can be built as a microservice.

### 3.4 Process View

The **Process View** presents more detail about how critical processes in the system work together.

- Can be reflected with a **state diagram**.
- Can be expanded with a **sequence diagram**.
- In a banking application, it can show processes such as:
  - Loan approval
  - Account approval

### 3.5 Deployment View

The **Deployment View** presents how the application will work in production.

- Shows how connected system components work together, such as:
  - Network firewall
  - Load balancer
  - Application servers
  - Database
- Should be simple enough for business users to understand.
- Can include more detail in a UML deployment diagram for technical users.
- Helps development and DevOps teams understand node components and dependencies.
- Represents the **physical layout of the system**.

### 3.6 Implementation View

The **Implementation View** is the core of the SAD and represents architecture and technology choices.

- Includes the architecture diagram and reasoning.
- Examples:
  - **3-tier architecture**
  - **N-tier architecture**
  - **Event-driven architecture**
- Details technology choices and trade-offs, for example:
  - Java vs. Node.js
  - Pros and cons of each option
- Justifies the resources and skills required to execute the project.

The development team uses the implementation view to create detailed design artifacts, such as class diagrams. Those detailed design artifacts do not always need to be part of the SAD.

### 3.7 Data View

Most applications are data-driven, so the **Data View** is essential.

- Represents how data flows between components.
- Explains how data is stored.
- Can explain data security and data integrity.
- Can use an entity-relationship (**ER**) diagram to show relationships between tables and schemas.
- Also explains reporting and analytics needs.

### 3.8 Operational View

The **Operational View** explains how the system will be maintained after launch.

- Defines service-level agreements (**SLAs**).
- Describes alerting and monitoring.
- Includes the disaster recovery plan.
- Includes the support plan.
- Details how maintenance will be carried out, such as:
  - Deploying bug fixes
  - Patching
  - Backup and recovery
  - Handling security incidents

### Additional views

The listed views help ensure the SAD covers the system and stakeholder concerns. Additional views may be added depending on stakeholder requirements:

- **Physical architecture view**
- **Network architecture view**
- **Security / controls architecture view**

[Back to Contents](#contents)

---

## 4. Structure of the SAD

The structure of the SAD can differ from project to project depending on stakeholder requirements and the nature of the project.

Examples:

- Creating a new product from the ground up.
- Modernizing a legacy application.
- Moving an entire system to the cloud.

For each project, the SAD may differ, but overall it should consider the views of different stakeholders and include a set of sections that cover multiple solution architecture and design aspects.

![Structure of a SAD](<assets/Structure of a SAD.png>)

**Figure: Structure of a SAD**

> The solutions architect may add subsections or remove sections depending on project requirements.

Examples of structure changes:

- Add an **Introduction** section to discuss the document purpose and provide a summary.
- For a transition project, add a subsection that presents the **existing architecture**, compares it with the **target architecture**, and explains the transition.

### 4.1 Solution Overview

The **Solution Overview** section briefly introduces the solution in a few paragraphs. It describes the function of the solution and its different components at a very high level.

A high-level block diagram can be added to show the major components in one place.

![Solution overview of an e-commerce platform](<assets/Solution overview of an e-commerce platform.png>)

**Figure: Solution overview of an e-commerce platform**

The component descriptions should use simplified language so that business users can understand the overall workings of the solution.

Example: a typical e-commerce order and supply chain management workflow can be explained as follows.

#### Customer Order

- The process begins when a customer places an order through a website or an online marketplace.
- Customer service assists customers with orders, handles inquiries, and resolves issues that may appear during the order process.

#### Supply Chain and Order Management

- **Sales Order:** once an order is placed, it is recorded as a sales order, which triggers the order fulfillment process.
- **Inventory Data:** the system checks inventory data to confirm that the ordered items are in stock.
- **Ship Notification:** after the order is processed and ready for shipment, a notification is sent, possibly including tracking information for the customer.

#### Order Processing

- **Payment:** processes customer payment, including taxes and promotional codes.
- **Tax:** calculates and applies the correct sales tax based on customer location and purchased items.
- **Transport:** arranges the logistics for delivering the order to the customer's location.
- **Order Fulfillment:** includes picking, packing, and preparing the ordered items for shipment.
- **Shipment:** sends the order out for delivery.
- **Promotion:** applies discounts or special offers that are part of the sales order.
- **Notification:** keeps the customer informed about order status.
- **Return:** handles returns if the customer is not satisfied or if there are order issues.
- **Cancellation:** processes cancellations if the customer cancels the order before shipment.

These elements allow customer orders to be handled efficiently from placement through delivery.

#### Solution Purpose

The **solution purpose** provides a brief outline of the business concern that the solution solves and the justification for building it.

Example:

- The purpose of an order management solution is to streamline and automate order management within a supply chain.
- It addresses business concerns around:
  - Efficiency
  - Accuracy
  - Customer satisfaction during order fulfillment

#### Solution Scope

The **solution scope** states the business scope of the proposed solution and clearly describes items that are out of scope.

Example:

- Scope includes end-to-end automation of the order management system, from customer order placement to shipment notification.
- Scope does not include post-delivery customer engagement, such as:
  - Feedback collection
  - Returns processing

#### Solution Assumptions

The **solution assumptions** subsection lists assumptions made by the solutions architect while creating the solution.

Example assumptions:

- Minimum network bandwidth is available to support real-time data processing.
- Integration is possible with marketplaces and shipping carriers.
- Customers have access to digital payment methods.

#### Solution Constraints

The **solution constraints** subsection lists technical, business, and resource constraints.

Constraints often come from industry and government compliance requirements. This section can also highlight risks and mitigation plans.

Example constraints:

- The solution must comply with data protection regulations such as **GDPR** for customer data privacy in the EU.
- The solution must comply with **PCI-DSS** when storing customer payment information in the USA.
- Resource constraints may include a fixed budget and deployment timeline.
- Technical constraints may include integration with legacy systems.

#### Solution Dependencies

The **solution dependencies** subsection lists upstream and downstream dependencies.

Example dependencies:

- An e-commerce website must communicate with a shipping system such as **UPS** or **FedEx** to ship packages to customers.
- The solution depends on real-time inventory data from the inventory management system.
- The solution requires integration with payment gateways for financial transactions.

#### Key Architecture Decisions

The **key architecture decisions** subsection lists significant problem statements and proposed solutions.

For each major decision, describe:

- Problem statement.
- Possible options.
- Pros and cons of each option.
- Selected decision.
- Rationale behind the decision.

Example decision: **use a cloud-based platform for scalability**

- Chosen because it can handle varying order volumes.
- Reduces the need for upfront capital investment.
- Trade-off: continuous operational expense.

Example decision: **adopt an API-first integration approach**

- Chosen to support flexibility and easier integration with partners and services.
- Trade-off: increased complexity of API management.

[Back to Contents](#contents)

### 4.2 Business Context

After giving the solution overview, the SAD should relate the solution to the **business context**.

In the **Business Context** section, the solutions architect provides a high-level overview of the business capabilities and requirements the solution will address.

This section contains only an abstract view of requirements. Detailed requirements should be documented in a separate requirements document, with an external link or reference provided here.

#### Business Capabilities

- Briefly describe the business capabilities for which the solution is being designed.
- Include the benefits of the capabilities.
- Explain how those capabilities address customer needs.

#### Key Business Requirements

- List the key business concerns the solution will address.
- Provide a high-level view of key requirements.
- Add a reference to the detailed requirements document.

#### Key Business Processes

Solutions architects should show key processes with a business process document.

![Business process diagram of an e-commerce platform](<assets/Business process diagram of an e-commerce platform.png>)

**Figure: Business process diagram of an e-commerce platform**

#### Business Stakeholders

List stakeholders directly or indirectly impacted by the project.

Examples:

- Sponsors
- Developers
- End users
- Vendors
- Partners

#### Non-Functional Requirements (NFRs)

Solutions architects must pay special attention to **NFRs**, because these are often missed by business users and development teams.

At a high level, NFRs should include:

- **Scalability:** how the application scales as workloads fluctuate.
  - Example: scale from 1,000 to 10,000 transactions per second, per day, or per month.
- **Availability and reliability:** acceptable downtime for system availability.
  - Example: 99.99% availability, or 45 minutes of downtime per month.
- **Performance:** performance requirements and the ability to handle load increases without affecting end-user experience.
  - Example: the catalog page must load within 3 seconds.
- **Portability:** whether the application can run on multiple platforms without additional work.
  - Example: a mobile app must run on iOS and Android.
- **Capacity:** the maximum workload the application can handle.
  - Example: maximum number of users, number of requests, expected response time, and expected application load.

[Back to Contents](#contents)

### 4.3 Conceptual Solution Overview

The **Conceptual Solution Overview** section provides an abstract-level diagram that captures the big-picture view of the whole solution, including both business and technical aspects.

> The conceptual view is a good middle point: it gives both business and technical stakeholders a useful system overview.

The conceptual solution overview provides a basis for:

- Analysis
- Trade-off studies
- Architecture refinement
- Architecture optimization
- Solution design
- Implementation planning

![Conceptual architecture diagram of an e-commerce platform](<assets/Conceptual architecture diagram of an e-commerce platform.png>)

**Figure: Conceptual architecture diagram of an e-commerce platform**

The conceptual architecture diagram should show:

- Significant modules.
- Information flowing between modules.
- A high-level understanding of the overall architecture.

The conceptual view is useful for both business and technical users, but technical users usually need deeper architecture details in the solution architecture section.

[Back to Contents](#contents)

### 4.4 Solution Architecture

The **Solution Architecture** section dives deeper into each part of the architecture.

It provides different views that technical teams can use to create detailed designs and work on implementation.

These views may target different groups:

- Developers
- Infrastructure engineers
- DevOps engineers
- Security engineers
- User experience (**UX**) designers

### 4.4.1 Information Architecture

The **Information Architecture** section provides the user navigation flow for the application.

The solutions architect should include the application navigation structure at a high level.

![Informational architecture diagram of an e-commerce platform](<assets/Informational architecture diagram of an e-commerce platform.png>)

**Figure: Informational architecture diagram of an e-commerce platform**

For example, in an e-commerce website, the information architecture can show that it takes three clicks for the user to navigate to the desired page.

Solutions architects can add more detail, such as:

- Website navigation
- Taxonomy
- High-level wireframe

UX designers can use this information to generate a detailed wireframe.

### 4.4.2 Application Architecture

The **Application Architecture** section targets the development team.

It provides implementation details that a software architect or development team can use to build a detailed design.

![Application architecture diagram of an e-commerce platform](<assets/Application architecture diagram of an e-commerce platform.png>)

**Figure: Application architecture diagram of an e-commerce platform**

For a cloud-based e-commerce platform, the application architecture may list technology building blocks such as:

- Caching
- Networking
- Content distribution
- Data storage

Example application architecture components:

- **User interaction:** customers interact with the e-commerce platform through a web interface, beginning with a secure purchase request over **SSL** for encrypted communication.
- **Content delivery:** **Amazon CloudFront**, a content delivery network (**CDN**), serves static content such as images, stylesheets, and client-side scripts. It reduces latency by caching content closer to the user's location.
- **Domain Name System (DNS):** **Amazon Route 53** manages DNS and directs user requests to appropriate endpoints, such as a CloudFront distribution or application load balancer.
- **Application processing:** inside the **Virtual Private Cloud (VPC)**, the e-commerce application service processes dynamic requests, such as page rendering based on user profiles and shopping history. A product recommendation service can provide personalized suggestions based on behavior and preferences.
- **Caching mechanism:** **Amazon ElastiCache** speeds up data retrieval by caching frequently accessed data, such as session state and frequently viewed product information. This reduces database load and improves response time.
- **Data storage and processing:** a cart checkout service manages cart interactions and transactions. Catalog and session cache data is stored for quick access. A search engine built with **Amazon Elasticsearch** provides search capabilities across the product catalog.
- **User profile and transaction data:** user profile information and transaction data are stored in **Amazon DynamoDB**, which provides scalable NoSQL database capabilities.
- **Data logging:** **Amazon S3** stores logging data such as clickstream data, product interactions, and system logs, enabling analysis of user behavior and system performance.

For application modernization, this section should also list application modules that need to be:

- Retired
- Retained
- Replatformed
- Transformed

### 4.4.3 Data Architecture

The **Data Architecture** section is primarily used by database administrators and development teams.

It helps them understand database schemas and how tables are related.

This section often includes an entity-relationship diagram (**ERD**) that shows relationships between entity sets stored in a database.

![ERD of an e-commerce platform](<assets/ERD of an e-commerce platform.png>)

**Figure: ERD of an e-commerce platform**

An ERD is a visual representation of entities, usually corresponding to database tables, and the relationships between them. It is used in database design to illustrate the logical structure of databases.

Example ERD components for an order processing system:

- **Event entity:** represents an occurrence or action in the system.
  - Attributes can include `Event_ID` as the primary key, `Event_Type`, `Event_Name`, and `Event_Loc`.
  - These attributes describe what the event is, where it happened, and other event characteristics.
- **Order entity:** represents a customer order.
  - Attributes can include `Order_ID` as the primary key, `Order_Number`, `Order_Type`, `Order_Quantity`, `Order_Date`, and `Ship_Address`.
  - These attributes store order-specific information such as amount ordered, shipping details, and order date.
- **Order_Processing entity:** acts as an associative entity or junction table that connects events to orders.
  - It indicates that an event leads to order processing.
  - It has its own primary key, `Processing_EventID`.
  - It includes foreign keys such as `Event_ID` and `Order_ID`.
  - The `Order_Event_date` attribute records when an event resulted in an order being processed.

Relationship lines between entities show cardinality.

- Crow's foot notation indicates **many**.
- A single line indicates **one**.

The data architecture section should also list all data objects that must be considered during application development.

### 4.4.4 Integration Architecture

**Integration Architecture** describes the framework that allows different software applications, systems, and services to communicate and work together effectively.

It includes the design and implementation of methods and middleware that support the exchange of data and processes across diverse systems:

- Within an organization.
- Between an organization and external parties.

This section mainly targets:

- Vendors
- Partners
- Other teams

![Integration architecture diagram of an e-commerce platform](<assets/Integration architecture diagram of an e-commerce platform.png>)

**Figure: Integration architecture diagram of an e-commerce platform**

The integration architecture section should list:

- Upstream systems that provide data to the application.
- Platforms, services, or databases from which the application receives data.
- The nature of the data flows.
- Downstream systems to which the application sends data.
- Applications, databases, or services that rely on data produced or processed by the application.
- All system dependencies related to the application.

### 4.4.5 Infrastructure Architecture

The **Infrastructure Architecture** section is primarily targeted at the infrastructure team and system engineers.

The solutions architect should include a deployment diagram that outlines logical server locations and dependencies.

![Deployment diagram of an e-commerce platform](<assets/Deployment diagram of an e-commerce platform.png>)

**Figure: Deployment diagram of an e-commerce platform**

Separate diagrams can be created for other environments, such as:

- Development (**dev**)
- Quality assurance (**QA**)
- User Acceptance Testing (**UAT**)
- Production (**prod**)

This section lists the infrastructure needed to deploy the application:

- Server configurations
- Databases
- Networks
- Switches

### 4.4.6 Security Architecture

The **Security Architecture** section includes the application's security and compliance aspects.

It should cover:

- **Identity and access management (IAM):**
  - Active Directory (**AD**)
  - User authentication
  - Authorization management
- **Infrastructure security:**
  - Firewall configuration
  - Intrusion Prevention System (**IPS**)
  - Intrusion Detection System (**IDS**)
  - Antivirus software
- **Application security:**
  - Web Application Firewall (**WAF**)
  - Distributed Denial of Service (**DDoS**) protection
- **Data security at rest and in transit:**
  - SSL
  - Encryption algorithms
  - Key management

The solutions architect can include an application security threat model to identify potential vulnerabilities and plan protections.

Examples:

- Cross-site scripting (**XSS**)
- SQL injection (**SQLi**)

[Back to Contents](#contents)

### 4.5 Solution Implementation

The **Solution Implementation** section includes essential considerations for developing and deploying the solution.

It can include the following major subsections.

#### Development

This section is essential for the development team.

It discusses:

- Development tools
- Programming language
- Code repository
- Code versioning
- Branching strategy
- Rationale behind these choices

#### Deployment

This section mainly focuses on DevOps engineers.

It discusses:

- Deployment approach
- Deployment tools
- Deployment components
- Deployment checklist
- Rationale behind these choices

#### Data Migration

This section helps the team understand:

- Data migration approach
- Data ingestion approach
- Scope of data migration
- Data objects
- Data ingestion tools
- Data sources
- Data formats

#### Application Decommissioning

This section lists:

- Existing systems that need to be decommissioned.
- Exit strategy for the current system if the return on investment (**ROI**) is not realized.
- Approach and timeline for decommissioning the old system.
- Overall impact of decommissioning.

The SAD includes development approach and tools, but it does not include detailed application-level design such as:

- Class diagrams
- Pseudocode

Those details should be handled by the software architect or senior developer in the relevant software application detailed design document.

[Back to Contents](#contents)

### 4.6 Solution Management

After the solution is deployed, it must be managed in production.

The **Solution Management** section focuses on production support and ongoing maintenance across product and non-product environments.

This section covers the operational aspects of the solution, including:

- Monitoring
- Incident management
- User onboarding
- Support processes
- Recovery processes

The solution management section is primarily targeted at the operations management team.

It should address:

- Operational management, such as system patching and upgrades of:
  - Dev environments
  - Test environments
  - Staging environments
  - Production environments
- Tools to manage application upgrades and new releases.
- Tools to manage system infrastructure.
- System monitoring and alerts.
- Operations dashboard.
- Production support.
- Service-level agreements (**SLAs**).
- Incident management.
- Disaster recovery.
- Business process continuation (**BPC**).

[Back to Contents](#contents)

### 4.7 Appendix

Like a business proposal document, the SAD can include an open **Appendix** section containing data that supports the overall architecture and solution choices.

The appendix can include:

- Open issues
- Research data
- Proof of concept (**POC**) outcomes
- Tool comparison data
- Vendor data
- Partner data

The structure of the SAD gives an overview of major sections that are usually included. The solutions architect may exclude some sections or include others to meet the requirements of the organization or project.

> SADs should continue to be iterated and improved. More robust SADs create better-defined implementation guidelines and reduce risk of failure.

A SAD is a running document created during the initial stages and updated over the years based on changes throughout the application life cycle.

[Back to Contents](#contents)

---

## 5. Life Cycle of the SAD

The life cycle of a SAD aligns with different phases of a project's life cycle. Different sections of the SAD are created at distinct stages.

The life cycle of a SAD typically includes the following phases:

![SAD life cycle](<assets/SAD life cycle.png>)

**Figure: SAD life cycle**

### Initiation

The **initiation** phase is where the need for a SAD is recognized, usually at the project's conception.

This phase defines objectives, such as outlining the software architecture for a new enterprise application. It sets the direction and scope for the document and helps ensure that the SAD aligns with project goals and stakeholder expectations.

### Gathering Requirements

The **gathering requirements** phase collects detailed requirements from key stakeholders.

Example:

- For a retail e-commerce platform, this may involve gathering insights on:
  - User experience
  - Payment processing
  - Inventory management

This phase ensures that the SAD addresses all critical aspects of the project.

### Drafting

The **drafting** phase creates the initial version of the SAD.

For example, a SAD for a cloud-based system may detail:

- AWS services to be used.
- Database schemas.
- Security protocols.
- Technical implementation blueprint.

The draft acts as the blueprint for the project's technical implementation.

### Review and Feedback

After the draft is prepared, it is shared with stakeholders for review.

Example:

- For a healthcare management system, reviewers may include:
  - Clinicians
  - IT staff
  - Administrative personnel

They may provide feedback on topics such as patient data flow and compliance with health regulations.

### Finalization

The **finalization** phase incorporates feedback and completes the SAD.

Example:

- A network architecture may be adjusted to improve data flow efficiency based on stakeholder input.

### Implementation

The finalized SAD guides project implementation.

For example, developers and project managers use the SAD to ensure alignment with:

- Planned architecture
- Technology stack
- Design decisions
- Delivery expectations

### Maintenance

Over time, the SAD is revisited and updated to reflect:

- Technology changes
- Business objective changes
- External factors
- Regulatory changes

Example:

- New data privacy laws may require updates to the SAD to maintain compliance.

[Back to Contents](#contents)

---

## 6. SAD Best Practices and Common Pitfalls

Effective management of SADs involves certain best practices and an active effort to avoid common pitfalls.

### Best Practices

- Keep the document **clear and concise**.
- Make it understandable while avoiding unnecessary technical jargon.
- Involve stakeholders regularly throughout the process.
- Ensure the SAD aligns with both business and technical requirements.
- Keep the SAD up to date with the latest project changes and developments.
- Align the architecture in the SAD with the organization's broader business goals.
- Make sure proposed technical solutions support and enhance business objectives.

### Common Pitfalls

- **Overcomplicating the architecture:** can create implementation and maintenance challenges.
- **Lack of flexibility:** makes the SAD harder to adapt when project scope or objectives change.
- **Insufficient stakeholder engagement:** can cause misalignment with real business needs and requirements.
- **Poor documentation practices:** can lead to misunderstandings, implementation errors, and project execution challenges.

> These pitfalls should be addressed proactively to improve the success of the solution architecture.

[Back to Contents](#contents)

---

## 7. IT Procurement Documentation for a Solution Architecture

In addition to the SAD, solution architecture often involves procurement proposal documents known as **RFx documents**.

**RFx** is a general term for formal request documents used at different stages of the procurement process.

RFx documents include:

- Request for Information (**RFI**)
- Request for Proposal (**RFP**)
- Request for Quotation (**RFQ**)

Solutions architects are often involved in procurement, either leading the process or providing input. Procurement may involve:

- Outsourcing
- Contracting
- Procuring software such as databases or development tools
- Buying SaaS solutions

Because these documents can be highly technical and can have broad, long-term impact, the solutions architect should provide input by responding to procurement requirements.

### 7.1 RFx Document Types

#### Request for Information (RFI)

An **RFI** comes early in the procurement process.

Buyers invite information from different vendors to support a later procurement decision. An RFI collects information about supplier capabilities so the buyer can compare suppliers using similar parameters and shortlist candidates for the next proposal stages.

Example:

- A company explores the market for learning management systems (**LMSs**) that meet training needs.
- The company issues an RFI to collect information about:
  - Functionalities
  - Integration capabilities
  - User experiences of available LMS platforms

#### Request for Proposal (RFP)

An **RFP** is used when shortlisted suppliers receive more information about the project's desired outcome.

An RFP is more open-ended than an RFI. Suppliers can propose the best way to deliver the solution and may include multiple options with pros and cons.

Example:

- A government agency wants to upgrade its IT infrastructure.
- The RFP describes:
  - Current system
  - Required improvements
  - Performance criteria
- Vendors respond with:
  - Technical solutions
  - Project timelines
  - Cost estimates

#### Request for Quotation (RFQ)

An **RFQ** is used when buyers have narrowed down the requirements and can list exact requirements for work, equipment, or supplies.

Suppliers provide costs for the listed requirements, and the buyer can choose the best quote to award the contract.

Example:

- A technology startup is scaling IT infrastructure to support a growing user base.
- The startup has already determined requirements for:
  - Compute instances
  - Memory
  - Storage
  - Bandwidth
- The RFQ asks infrastructure providers to submit:
  - Monthly pricing
  - Yearly pricing
  - Long-term commitment discounts
  - Support options
  - Monitoring options
  - Security features included in the quoted price

### RFP as the Common Shortcut

The **RFP** is often the most popular choice because buyer organizations may ask directly for RFP documents from potential vendors to speed up procurement.

In this situation, the RFP document needs a clear structure so the buyer can compare vendors by:

- Capabilities
- Solution approaches
- Costs
- Delivery confidence

### 7.2 Solutions Architect Role in RFP Work

Due to the technical nature of procurement in IT organizations, solutions architects play an important role in:

- Evaluating vendor capabilities and approaches from the buyer's perspective.
- Responding to RFP documents from the supplier side.

During an RFP process for an IT workload, a solutions architect may contribute in the following ways.

#### Understanding Requirements

The solutions architect thoroughly understands the technical and business requirements.

Examples:

- Enhancing existing systems.
- Migrating to the cloud.
- Integrating new technologies.

#### Designing the Solution

The solutions architect drafts a preliminary IT architecture design that addresses the identified requirements.

This includes:

- Selecting appropriate technologies.
- Designing the infrastructure layout.
- Considering integration with existing systems.

#### Collaborative Contribution

The architect collaborates with cross-functional teams to ensure that the proposal aligns with business objectives and technical feasibility.

Common collaborators:

- Business analysts
- Project managers
- Technical leads

#### Estimating Resources

The architect estimates resources required for the project, such as:

- Hardware
- Software
- Cloud services
- Personnel hours

This helps keep the proposal competitive and realistic.

#### Risk Assessment

Solutions architects identify potential project risks and propose mitigation strategies for the RFP response.

#### Documentation

Solutions architects help create detailed technical documentation explaining how the proposed solution meets the RFP requirements.

This documentation may include:

- System diagrams
- Data flow diagrams
- Detailed descriptions of the proposed environment

#### Pricing Strategy

Solutions architects may work with the finance team to develop a pricing strategy for the proposal, ensuring the costs are aligned with the value delivered.

#### Presentation

Solutions architects may be part of the team that presents the proposal to the potential client.

They explain technical aspects and answer technical questions from the client.

> The role of a solutions architect is to tailor a solution that meets client requirements while ensuring technological viability and cost-effectiveness.

### 7.3 Cloud CRM RFP Example

For a company soliciting proposals for a new cloud-based CRM system, the solutions architect would first analyze the existing IT infrastructure and assess required features such as:

- Scalability
- Data security

The architect would design a cloud solution that:

- Integrates with current systems such as ERP.
- Aligns with marketing strategies.
- Fits the company's specific needs.
- Evaluates compatibility of potential CRM platforms.
- Meets compliance and data management requirements in collaboration with legal teams.

A key task is creating a migration strategy to move data from old systems to the new CRM with minimal business disruption.

This also involves estimating the **total cost of ownership (TCO)**, including:

- Subscription costs
- Customization
- Data transfer
- Training needs

The solutions architect helps draft the technical sections of the RFP response, including:

- Proposed architecture
- Data strategy
- Security measures
- Implementation roadmap
- Scalability plan
- Post-deployment support structure

The solutions architect may also present and defend the technical strategy to client decision-makers, showing the benefits and practicality of the proposed solution.

The goal is to help the client invest in a reliable, secure, and scalable system that supports business growth and adapts to evolving market demands.

[Back to Contents](#contents)

---

## 8. Summary

The SAD keeps stakeholders on the same page and supports formal agreement on solution design and requirements.

Because stakeholders include both business and technical users, the SAD should include views that address different needs:

- For non-technical users:
  - Business view
  - Process view
  - Logical view
- For technical users:
  - Application view
  - Development view
  - Deployment view
  - Operational view

The detailed SAD structure includes major sections and subsections covering:

- Solution overview
- Business context
- Conceptual architecture
- Application architecture
- Data architecture
- Infrastructure architecture
- Integration architecture
- Security architecture
- Solution delivery considerations
- Operations management

The completed SAD should provide clear implementation guidance, reduce delivery risk, and remain a living document throughout the solution life cycle.

[Back to Contents](#contents)
