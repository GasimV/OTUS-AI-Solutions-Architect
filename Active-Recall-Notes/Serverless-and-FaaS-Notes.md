# Serverless & FaaS - Active Recall Notes

## Contents

1. [What Is Serverless?](#1-what-is-serverless)
2. [Function as a Service (FaaS)](#2-function-as-a-service-faas)
3. [Backend as a Service (BaaS)](#3-backend-as-a-service-baas)
4. [Serverless vs Traditional Architecture](#4-serverless-vs-traditional-architecture)
5. [Cold Starts](#5-cold-starts)
6. [Event-Driven Execution Model](#6-event-driven-execution-model)
7. [Major Serverless Platforms](#7-major-serverless-platforms)
8. [Serverless Design Patterns](#8-serverless-design-patterns)
9. [Serverless for AI/ML Workloads](#9-serverless-for-aiml-workloads)
10. [Security in Serverless](#10-security-in-serverless)
11. [Monitoring & Observability](#11-monitoring--observability)
12. [Limitations & Trade-offs](#12-limitations--trade-offs)
13. [Final Recall Checklist](#final-recall-checklist)

---

## 1. What Is Serverless?

A **cloud execution model** where the cloud provider dynamically manages the allocation and provisioning of servers. The developer writes code; the provider handles infrastructure, scaling, and availability.

**Key idea:** "Serverless" does not mean no servers — it means servers are **invisible to the developer**

### Two Pillars of Serverless

1. **FaaS (Function as a Service)** — run individual functions in response to events
2. **BaaS (Backend as a Service)** — consume fully managed third-party services (auth, database, storage) via APIs

### Core Properties

| Property | What It Means |
|----------|---------------|
| **No server management** | No patching, provisioning, or capacity planning |
| **Pay-per-use** | Billed only for actual execution time (often per 100ms) |
| **Auto-scaling** | Scales from zero to thousands of instances automatically |
| **Event-driven** | Functions execute in response to triggers (HTTP, queue, schedule, file upload) |
| **Stateless** | Each invocation is independent — no guaranteed local state between calls |
| **Ephemeral** | Runtime containers are created and destroyed on demand |

### Why It Matters

- Eliminates **undifferentiated heavy lifting** — teams focus on business logic, not infrastructure
- Enables **rapid prototyping** — deploy a working API in minutes without setting up servers
- Cost-effective for **bursty or unpredictable** workloads — no paying for idle capacity

[Back to Contents](#contents)

---

## 2. Function as a Service (FaaS)

The compute layer of serverless — **deploy individual functions** that run in short-lived, isolated containers triggered by events.

### How FaaS Works

```
Event Source (HTTP / Queue / Timer / File Upload)
        |
        v
  +-----------+
  |  Gateway   |    ← Routes event to the correct function
  +-----------+
        |
        v
  +-----------+
  |  Runtime   |    ← Spins up container (or reuses warm one)
  |  Container |
  +-----------+
        |
        v
  Function Code Executes
        |
        v
  Response / Side Effect (write to DB, send message, return HTTP)
```

### Function Lifecycle

1. **Deploy** — upload code (zip, container image, or inline)
2. **Trigger** — event fires (e.g., HTTP request arrives)
3. **Init** — platform provisions a container and loads your code (cold start)
4. **Execute** — function handler runs with event payload
5. **Response** — result returned to caller or pushed to output
6. **Idle / Destroy** — container kept warm for a short time, then destroyed

### Key Constraints

| Constraint | Typical Limit | Why It Exists |
|-----------|---------------|---------------|
| **Execution timeout** | 15 min (AWS Lambda) | Prevents runaway costs and resource hogging |
| **Memory** | 128 MB - 10 GB | Bounds the container resource footprint |
| **Payload size** | 6 MB sync / 256 KB async (Lambda) | Keeps invocations lightweight |
| **Concurrency** | 1000 default (account-level) | Protects downstream services and shared infra |
| **Package size** | 50 MB zipped / 250 MB unzipped | Fast cold start requires small deployments |
| **Stateless** | No local persistence | Each invocation may run on a different host |

### Recall: FaaS Essentials

- Functions are **short-lived**, **stateless**, **event-triggered**
- Platform handles **scaling**, **provisioning**, **fault tolerance**
- Constraints exist to keep execution **fast, cheap, and isolated**

[Back to Contents](#contents)

---

## 3. Backend as a Service (BaaS)

The other half of serverless — **fully managed third-party services** consumed via APIs, removing the need to build and maintain backend components.

### Common BaaS Categories

| Category | Examples | What It Replaces |
|----------|----------|-----------------|
| **Authentication** | Firebase Auth, AWS Cognito, Auth0 | Custom user/session management |
| **Database** | DynamoDB, Firestore, Supabase | Self-managed database servers |
| **Storage** | S3, Cloud Storage, Azure Blob | File servers, NAS |
| **Messaging** | SNS, SQS, Pub/Sub | Self-hosted message brokers |
| **Search** | Algolia, Elasticsearch Service | Self-managed search clusters |
| **Push Notifications** | Firebase Cloud Messaging, OneSignal | Custom notification infrastructure |

### FaaS + BaaS Together

A typical serverless application combines both:

```
Client App
    |
    ├── FaaS (business logic, data transformation, orchestration)
    |       |
    |       ├── DynamoDB (BaaS - data)
    |       ├── S3 (BaaS - storage)
    |       └── SQS (BaaS - messaging)
    |
    └── Firebase Auth (BaaS - authentication) ← direct from client
```

**Key insight:** BaaS moves complexity from your code to the provider's managed service — you trade control for speed and reduced operational burden

[Back to Contents](#contents)

---

## 4. Serverless vs Traditional Architecture

| Dimension | Traditional (Server-based) | Serverless |
|-----------|---------------------------|------------|
| **Infrastructure** | You provision, patch, scale | Provider manages everything |
| **Scaling** | Manual or auto-scale groups (minutes) | Instant, per-request (milliseconds) |
| **Cost model** | Pay for allocated capacity (even idle) | Pay only for execution time |
| **State** | Servers maintain local state, sessions | Stateless — external stores required |
| **Startup** | Always running | On-demand (cold start latency) |
| **Execution duration** | Unlimited | Capped (e.g., 15 min) |
| **Vendor lock-in** | Lower (portable VMs/containers) | Higher (proprietary triggers, APIs) |
| **Debugging** | SSH into server, inspect logs locally | Distributed tracing, cloud-native logging |
| **Best for** | Long-running, stateful, predictable load | Event-driven, bursty, microservices |

### When Serverless Fits Well

- API backends with variable traffic
- Event processing (file uploads, webhooks, IoT data)
- Scheduled tasks (cron jobs, report generation)
- Chatbots and lightweight AI inference endpoints

### When Serverless Does NOT Fit

- Long-running computations (model training, video encoding > 15 min)
- Workloads requiring persistent WebSocket connections
- High-throughput, steady-state services (cheaper to reserve capacity)
- Applications requiring low, predictable latency (cold starts unacceptable)

[Back to Contents](#contents)

---

## 5. Cold Starts

The **delay** experienced when a serverless function is invoked after being idle — the platform must provision a new container, load the runtime, and initialize the code.

### Cold Start Breakdown

```
|<------------ Cold Start Latency ------------>|<-- Execution -->|
|                                               |                 |
[ Container Provision ][ Runtime Init ][ Code Init ][ Handler Run ]
       ~100-300ms          ~50-200ms     ~10-500ms     your code
```

### Factors That Affect Cold Start Duration

| Factor | Impact | Mitigation |
|--------|--------|------------|
| **Language runtime** | JVM (Java, Scala) = slower; Python, Node.js, Go = faster | Choose lightweight runtimes for latency-sensitive functions |
| **Package size** | Larger deployment = longer load time | Minimize dependencies, use layers, tree-shake |
| **VPC attachment** | ENI creation adds 1-10s (historically) | Use VPC improvements (AWS Hyperplane) or avoid VPC if possible |
| **Memory allocation** | More memory = more CPU = faster init | Allocate more memory even if function doesn't need it |
| **Provisioned concurrency** | Pre-warms N instances | Use for latency-critical paths (costs more) |

### Warm vs Cold Invocation

- **Cold** — no existing container; full initialization (~200ms to 5s+)
- **Warm** — reusing an existing container from recent invocation (~1-10ms overhead)
- **Provisioned** — pre-warmed containers always ready (~0ms cold start, constant cost)

### Mitigation Strategies

1. **Provisioned concurrency** — keep N containers warm (AWS Lambda, Azure Functions Premium)
2. **Keep-alive pings** — scheduled invocations to prevent container teardown (hack, not recommended at scale)
3. **Smaller packages** — fewer dependencies = faster load
4. **Lazy initialization** — defer heavy setup (DB connections) to first actual use, not module load
5. **Choose fast runtimes** — Go, Rust, Python generally cold-start faster than Java, .NET

[Back to Contents](#contents)

---

## 6. Event-Driven Execution Model

Serverless functions are **reactive** — they do nothing until an event arrives. The event source determines **when** and **how** the function is invoked.

### Common Event Sources

| Source Type | Examples | Invocation |
|------------|---------|------------|
| **HTTP** | API Gateway, ALB | Synchronous — caller waits for response |
| **Queue / Stream** | SQS, Kafka, Kinesis | Asynchronous — batch of messages processed |
| **Storage** | S3 put, Blob upload | Asynchronous — file event triggers function |
| **Schedule** | CloudWatch Events, Cron | Asynchronous — timer-based |
| **Database** | DynamoDB Streams, Change Data Capture | Asynchronous — row change triggers function |
| **IoT** | MQTT messages, device events | Asynchronous — sensor data triggers function |
| **Custom** | SNS, EventBridge, Pub/Sub | Async or sync depending on configuration |

### Sync vs Async Invocation

```
Synchronous:                        Asynchronous:
Client ──req──> Function            Client ──event──> Queue ──> Function
Client <──res── Function            Client <──202 Accepted──   (result later)
```

- **Sync** — caller blocks until function returns (API calls)
- **Async** — event is queued; function processes it independently; retries on failure

### Event Payload

Every invocation receives an **event object** containing the trigger data:

- HTTP trigger → method, headers, body, path parameters
- S3 trigger → bucket name, object key, event type
- SQS trigger → array of message records

**Key insight:** The function's logic should be **event-source agnostic** where possible — parse the event at the handler boundary, then call pure business logic underneath

[Back to Contents](#contents)

---

## 7. Major Serverless Platforms

| Platform | Provider | Language Support | Key Differentiator |
|----------|----------|-----------------|-------------------|
| **AWS Lambda** | Amazon | Python, Node.js, Java, Go, .NET, Ruby, Rust (custom) | Largest ecosystem, deepest AWS integration |
| **Azure Functions** | Microsoft | C#, Java, JavaScript, Python, PowerShell, TypeScript | Tight integration with Azure services and VS Code |
| **Google Cloud Functions** | Google | Node.js, Python, Go, Java, .NET, Ruby, PHP | Simple, integrates with Firebase and GCP |
| **Google Cloud Run** | Google | Any (container-based) | Runs containers serverlessly — no language/runtime lock-in |
| **AWS Fargate** | Amazon | Any (container-based) | Serverless containers on ECS/EKS — no EC2 management |
| **Cloudflare Workers** | Cloudflare | JavaScript, TypeScript, Rust, WASM | Edge-deployed, ultra-low latency (~0ms cold start with V8 isolates) |
| **Vercel Functions** | Vercel | Node.js, Go, Python, Ruby | Optimized for frontend/full-stack web apps |
| **OpenFaaS** | Open-source | Any (container-based) | Self-hosted, Kubernetes-native, vendor-neutral |
| **Knative** | Open-source (Google) | Any (container-based) | Kubernetes-based serverless building blocks |

### Containers-as-Serverless Trend

Traditional FaaS (Lambda-style) has strict constraints. Newer platforms like **Cloud Run**, **Fargate**, and **Knative** blur the line — they run **arbitrary containers** with serverless properties (scale to zero, pay-per-use, no server management).

---

### Quick-Start Examples

#### AWS Lambda (Python)

**1. Write the function** — `lambda_function.py`:

```python
import json

def lambda_handler(event, context):
    """Simple HTTP handler that echoes back the request body."""
    body = json.loads(event.get("body", "{}"))
    name = body.get("name", "World")
    return {
        "statusCode": 200,
        "body": json.dumps({"message": f"Hello, {name}!"})
    }
```

**2. Package and deploy** (AWS CLI):

```bash
# Zip the function
zip function.zip lambda_function.py

# Create the Lambda function
aws lambda create-function \
  --function-name hello-serverless \
  --runtime python3.12 \
  --role arn:aws:iam::123456789012:role/lambda-exec-role \
  --handler lambda_function.lambda_handler \
  --zip-file fileb://function.zip

# Invoke it directly (for testing)
aws lambda invoke \
  --function-name hello-serverless \
  --payload '{"body": "{\"name\": \"Alice\"}"}' \
  response.json
```

**3. Expose via API Gateway** — attach an HTTP API trigger in the AWS console or via CLI, mapping `POST /hello` → `hello-serverless` Lambda.

**Key takeaway:** You write only the handler. AWS manages the runtime, scaling, OS, and networking.

---

#### OpenFaaS (Kubernetes, any language)

OpenFaaS is **self-hosted** on any Kubernetes cluster — no cloud vendor required. Functions are packaged as **Docker containers**.

**1. Install OpenFaaS on a K8s cluster:**

```bash
# Install faas-cli (the OpenFaaS CLI)
curl -sL https://cli.openfaas.com | sudo sh

# Deploy OpenFaaS into the cluster using arkade (or Helm)
arkade install openfaas

# Port-forward the gateway to localhost
kubectl port-forward svc/gateway -n openfaas 8080:8080

# Log in
PASSWORD=$(kubectl get secret -n openfaas basic-auth -o jsonpath="{.data.basic-auth-password}" | base64 --decode)
echo $PASSWORD | faas-cli login --username admin --password-stdin
```

**2. Create a function from a template:**

```bash
# Scaffold a new Python function
faas-cli new hello-openfaas --lang python3

# This creates:
# hello-openfaas/
#   handler.py        ← your function code
#   requirements.txt  ← Python dependencies
# hello-openfaas.yml  ← deployment config
```

**3. Write the handler** — `hello-openfaas/handler.py`:

```python
import json

def handle(req):
    """Receives raw request body, returns response string."""
    body = json.loads(req) if req else {}
    name = body.get("name", "World")
    return json.dumps({"message": f"Hello, {name}!"})
```

**4. Build, push, and deploy:**

```bash
# Build the container image, push to registry, deploy to cluster
faas-cli up -f hello-openfaas.yml

# Invoke the function
curl http://127.0.0.1:8080/function/hello-openfaas \
  -d '{"name": "Alice"}'
# → {"message": "Hello, Alice!"}
```

**5. Auto-scaling** — OpenFaaS scales based on requests per second by default (configurable):

```yaml
# In hello-openfaas.yml
functions:
  hello-openfaas:
    labels:
      com.openfaas.scale.min: "1"    # minimum replicas
      com.openfaas.scale.max: "10"   # maximum replicas
      com.openfaas.scale.zero: "true" # scale to zero when idle
```

**Key takeaway:** OpenFaaS gives you serverless **on your own infrastructure** — vendor-neutral, runs anywhere Kubernetes runs.

---

#### Knative (Kubernetes-native serverless)

Knative provides **building blocks** for serverless on Kubernetes. Unlike OpenFaaS (which is a complete FaaS platform), Knative gives you two components:

- **Knative Serving** — deploy and auto-scale containers (including scale to zero)
- **Knative Eventing** — connect event sources to services (pub/sub, channels, brokers)

**1. Install Knative on a K8s cluster:**

```bash
# Install Knative Serving (CRDs + core)
kubectl apply -f https://github.com/knative/serving/releases/download/knative-v1.13.0/serving-crds.yaml
kubectl apply -f https://github.com/knative/serving/releases/download/knative-v1.13.0/serving-core.yaml

# Install a networking layer (e.g., Kourier)
kubectl apply -f https://github.com/knative/net-kourier/releases/download/knative-v1.13.0/kourier.yaml

# Install Knative CLI (optional, but convenient)
brew install knative/client/kn
```

**2. Write a simple web server** (any language — Knative runs containers, not functions):

```python
# app.py — a regular Flask app
from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/", methods=["POST"])
def hello():
    body = request.get_json(silent=True) or {}
    name = body.get("name", "World")
    return jsonify({"message": f"Hello, {name}!"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
```

**3. Containerize it** — `Dockerfile`:

```dockerfile
FROM python:3.12-slim
WORKDIR /app
COPY app.py .
RUN pip install flask
CMD ["python", "app.py"]
```

```bash
# Build and push
docker build -t docker.io/youruser/hello-knative:latest .
docker push docker.io/youruser/hello-knative:latest
```

**4. Deploy as a Knative Service** — `service.yaml`:

```yaml
apiVersion: serving.knative.dev/v1
kind: Service
metadata:
  name: hello-knative
spec:
  template:
    metadata:
      annotations:
        autoscaling.knative.dev/min-scale: "0"   # scale to zero
        autoscaling.knative.dev/max-scale: "10"
    spec:
      containers:
        - image: docker.io/youruser/hello-knative:latest
          ports:
            - containerPort: 8080
```

```bash
kubectl apply -f service.yaml

# Or using the Knative CLI:
kn service create hello-knative \
  --image docker.io/youruser/hello-knative:latest \
  --port 8080

# Get the auto-generated URL
kn service describe hello-knative -o url
# → http://hello-knative.default.example.com

# Invoke it
curl -X POST http://hello-knative.default.example.com \
  -H "Content-Type: application/json" \
  -d '{"name": "Alice"}'
# → {"message": "Hello, Alice!"}
```

**5. Knative Eventing** — wire an event source to the service:

```yaml
apiVersion: sources.knative.dev/v1
kind: PingSource
metadata:
  name: every-minute
spec:
  schedule: "*/1 * * * *"
  contentType: "application/json"
  data: '{"name": "Scheduled User"}'
  sink:
    ref:
      apiVersion: serving.knative.dev/v1
      kind: Service
      name: hello-knative
```

This fires a JSON event to `hello-knative` every minute — demonstrating how Knative Eventing decouples **event sources** from **services**.

**Key takeaway:** Knative turns any container into a serverless workload on Kubernetes — no function format required, no vendor lock-in. It is the foundation under Google Cloud Run.

---

### Platform Comparison at a Glance

| Aspect | AWS Lambda | OpenFaaS | Knative |
|--------|-----------|----------|---------|
| **Hosting** | Fully managed (AWS) | Self-hosted (any K8s) | Self-hosted (any K8s) |
| **Unit of deployment** | Function (handler) | Function (Docker container) | Container (any web server) |
| **Scale to zero** | Yes (default) | Yes (opt-in) | Yes (default) |
| **Language lock-in** | Runtime-specific | None (container) | None (container) |
| **Max timeout** | 15 min | Configurable (unlimited) | Configurable (unlimited) |
| **Event system** | AWS triggers (S3, SQS, etc.) | HTTP + connectors | Knative Eventing (pluggable) |
| **Vendor lock-in** | High (AWS-specific) | None | None |
| **Setup complexity** | Low (managed) | Medium (need K8s cluster) | Medium-High (need K8s + CRDs) |

[Back to Contents](#contents)

---

## 8. Serverless Design Patterns

### 8.1 Simple Function (Single-Purpose)

One function = one task. The most basic and recommended pattern.

```
API Gateway ──> Lambda (process order) ──> DynamoDB
```

**When to use:** CRUD operations, webhooks, simple transformations

---

### 8.2 Fan-Out / Fan-In

Distribute work across many parallel functions, then aggregate results.

```
                    ┌── Function A ──┐
Trigger ── SNS/SQS ─┤── Function B ──├── Aggregator Function ──> Result
                    └── Function C ──┘
```

**When to use:** Image processing, batch data transformation, parallel API calls

---

### 8.3 Function Chaining (Pipeline)

Output of one function feeds as input to the next.

```
Event ──> Function 1 ──> Function 2 ──> Function 3 ──> Output
```

**Risk:** Tight coupling, cascading failures, difficult to debug
**Better alternative:** Use Step Functions / Durable Functions for orchestration

---

### 8.4 Orchestrator (State Machine)

A central coordinator manages the workflow steps, handling branching, retries, and error handling.

```
              ┌── Validate ──> Process ──> Notify (success)
Step Functions ┤
              └── Validate ──> Error Handler ──> Notify (failure)
```

**Tools:** AWS Step Functions, Azure Durable Functions, Google Workflows
**When to use:** Multi-step business processes, approval flows, saga pattern

---

### 8.5 Event Sourcing

Store every state change as an immutable event. Replay events to reconstruct state.

```
User Action ──> Lambda ──> Event Store (append only)
                                |
                                v
                    Materialized View (read model)
```

**When to use:** Audit trails, financial transactions, undo/replay capability

---

### 8.6 Strangler Fig (Migration)

Gradually replace a monolith by routing specific endpoints to serverless functions.

```
API Gateway
    ├── /old-endpoint  ──> Legacy Monolith
    ├── /new-endpoint  ──> Lambda Function
    └── /migrated      ──> Lambda Function
```

**When to use:** Incremental modernization without rewriting everything at once

---

### 8.7 CQRS (Command Query Responsibility Segregation)

Separate write (command) and read (query) paths, each optimized independently.

```
Write ──> Lambda ──> DynamoDB (write-optimized)
                        |
                  DynamoDB Stream
                        |
                        v
              Lambda ──> Elasticsearch (read-optimized)
                              ^
                        Read ──┘
```

**When to use:** High read-to-write ratio, complex query requirements, event-driven systems

[Back to Contents](#contents)

---

## 9. Serverless for AI/ML Workloads

### What Fits

| Use Case | Why Serverless Works |
|----------|---------------------|
| **Model inference (lightweight)** | Short execution, bursty traffic, scale-to-zero between requests |
| **Data preprocessing** | Event-triggered ETL (e.g., new file in S3 triggers transformation) |
| **Feature engineering pipelines** | Chain of small transformations on incoming data |
| **Chatbot backends** | Stateless request/response, integrates with API Gateway |
| **A/B testing routing** | Lightweight function decides which model version to call |

### What Does NOT Fit

| Use Case | Why Serverless Fails |
|----------|---------------------|
| **Model training** | Exceeds timeout limits, needs GPUs, long-running |
| **Large model inference** | Package size limits, cold start with large models, memory constraints |
| **Real-time streaming** | Persistent connections needed, not event-per-invocation |

### Hybrid Pattern for AI

```
Training (EC2/SageMaker with GPU)
        |
        v
  Model Artifact ──> S3
        |
        v
  Lambda loads model ──> Inference endpoint (lightweight models < 250 MB)
        or
  Lambda calls ──> SageMaker Endpoint (heavy models, GPU inference)
```

**Key insight:** Use serverless as the **glue and routing layer** — trigger preprocessing, route requests, post-process results — while heavy compute stays on dedicated infrastructure

[Back to Contents](#contents)

---

## 10. Security in Serverless

### Shared Responsibility Shifts

```
Traditional:     You manage [OS + Runtime + App + Data + Network]
Serverless:      Provider manages [OS + Runtime + Scaling + Patching]
                 You manage [App code + Data + IAM + Configuration]
```

The attack surface **shrinks** (no OS to patch) but **shifts** (more focus on IAM, input validation, dependency security).

### Key Security Concerns

| Concern | Risk | Mitigation |
|---------|------|------------|
| **Over-permissive IAM** | Function has access to resources it shouldn't | Least-privilege per function, not per application |
| **Injection attacks** | Event data contains malicious input | Validate and sanitize all event inputs |
| **Dependency vulnerabilities** | Third-party packages with known CVEs | Regular dependency scanning (Snyk, Dependabot) |
| **Secrets in environment variables** | Credentials exposed in config | Use secrets manager (AWS Secrets Manager, Vault) |
| **Function event injection** | Crafted events exploit parsing logic | Validate event schema at handler boundary |
| **Insecure deserialization** | Untrusted data deserialized into objects | Use safe parsers, avoid pickle/eval |
| **Denial of wallet** | Attacker triggers massive scale-up to inflate costs | Set concurrency limits, budget alerts, rate limiting |

### Serverless-Specific Best Practices

1. **One IAM role per function** — never share roles across functions
2. **Encrypt at rest and in transit** — use KMS, TLS everywhere
3. **Set concurrency limits** — protect against denial-of-wallet attacks
4. **Minimize package dependencies** — smaller surface = fewer vulnerabilities
5. **Enable function-level logging** — every invocation should be auditable

[Back to Contents](#contents)

---

## 11. Monitoring & Observability

Serverless makes monitoring **harder** because there are no persistent servers to instrument — everything is distributed and ephemeral.

### Three Pillars

| Pillar | What It Captures | Tools |
|--------|-----------------|-------|
| **Logs** | Function output, errors, debug info | CloudWatch Logs, Datadog, ELK |
| **Metrics** | Invocation count, duration, errors, throttles, cold starts | CloudWatch Metrics, Prometheus |
| **Traces** | End-to-end request path across services | AWS X-Ray, Jaeger, Zipkin |

### Key Metrics to Watch

| Metric | Why It Matters |
|--------|---------------|
| **Invocation count** | Understand traffic patterns, detect anomalies |
| **Duration (p50, p95, p99)** | Identify slow functions, approaching timeout |
| **Error rate** | Detect failures early |
| **Throttle count** | Hitting concurrency limits — need to request increase |
| **Cold start frequency** | If high, consider provisioned concurrency |
| **Concurrent executions** | Approaching account limits |
| **Iterator age** (for streams) | Growing lag = function can't keep up with events |

### Distributed Tracing

In a serverless architecture, a single user request may traverse:

```
API Gateway ──> Lambda A ──> SQS ──> Lambda B ──> DynamoDB
                                                    |
                                              Lambda C (stream)
```

Without tracing, debugging failures across this chain is nearly impossible. **Correlation IDs** and **X-Ray traces** make the invisible visible.

[Back to Contents](#contents)

---

## 12. Limitations & Trade-offs

| Limitation | Impact | Workaround |
|-----------|--------|------------|
| **Cold starts** | Latency spikes for infrequent functions | Provisioned concurrency, keep-alive |
| **Execution timeout** | Cannot run > 15 min (Lambda) | Break into steps, use Step Functions |
| **Vendor lock-in** | Proprietary triggers, APIs, packaging | Use containers (Cloud Run), abstraction layers (Serverless Framework) |
| **Debugging difficulty** | No SSH, ephemeral logs, distributed systems | Structured logging, tracing, local emulators (SAM, LocalStack) |
| **State management** | No local persistence between invocations | External stores (DynamoDB, Redis, S3) |
| **Cost at scale** | At sustained high throughput, per-invocation billing > reserved instances | Evaluate break-even point; switch to containers for steady workloads |
| **Testing complexity** | Hard to replicate cloud triggers locally | Use emulators (LocalStack), integration test in staging |
| **Concurrency limits** | Account-level caps can throttle under load | Request limit increases, implement queuing |
| **Package size limits** | Large ML models or dependencies won't fit | Use container images (up to 10 GB), or call external inference endpoints |

### The Break-Even Question

Serverless is cheaper when:
- Traffic is **bursty** and **unpredictable**
- Functions are **idle most of the time**
- Development speed matters more than per-request cost

Traditional infra is cheaper when:
- Load is **steady and predictable**
- Functions run **near-constantly** at high concurrency
- Workloads are **long-running** or **resource-heavy**

**Rule of thumb:** If your function runs > 50% of the time at high concurrency, compare the cost against a reserved container or EC2 instance

[Back to Contents](#contents)

---

## Final Recall Checklist

Try to recall from memory:

- [ ] Define serverless — what does the developer manage vs the provider?
- [ ] FaaS vs BaaS — explain the difference with examples
- [ ] What causes cold starts and name 3 mitigation strategies
- [ ] Sync vs async invocation — when is each used?
- [ ] Name 4 event sources that can trigger a serverless function
- [ ] When does serverless NOT fit? Give 3 scenarios
- [ ] Explain the Fan-Out / Fan-In pattern
- [ ] What is "denial of wallet" and how to prevent it?
- [ ] Why is one IAM role per function important?
- [ ] Name the 3 pillars of observability
- [ ] What is the break-even question for serverless vs traditional?
- [ ] How would you use serverless for AI/ML inference vs training?

[Back to Contents](#contents)
