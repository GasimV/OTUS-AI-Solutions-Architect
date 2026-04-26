# Networking Math, DevOps Networking, and VoIP Active Recall Notes

<a id="contents"></a>

## Contents

- [Elaborative Encoding Framework for Networking Math](#elaborative-encoding-framework-for-networking-math)
  - [1. Core Idea](#networking-math-core-idea)
  - [2. Main Encoding Pattern](#main-encoding-pattern)
  - [3. Core Networking Math Concepts](#core-networking-math-concepts)
  - [4. Bits, Bytes, and Unit Conversion](#bits-bytes-and-unit-conversion)
  - [5. Transfer Time](#transfer-time)
  - [6. Bandwidth vs. Throughput](#bandwidth-vs-throughput)
  - [7. Latency](#latency)
  - [8. Round Trip Time (RTT)](#round-trip-time-rtt)
  - [9. Propagation Delay](#propagation-delay)
  - [10. Transmission Delay](#transmission-delay)
  - [11. Total Delay](#total-delay)
  - [12. Bandwidth-Delay Product](#bandwidth-delay-product)
  - [13. Packet Overhead](#packet-overhead)
  - [14. Packet Loss](#packet-loss)
  - [15. Jitter](#jitter)
  - [16. Subnetting](#subnetting)
  - [17. Number of Hosts in a Subnet](#number-of-hosts-in-a-subnet)
  - [18. CIDR Prefixes](#cidr-prefixes)
  - [19. Subnet Mask](#subnet-mask)
  - [20. Block Size](#block-size)
  - [21. Network Address and Broadcast Address](#network-address-and-broadcast-address)
  - [22. Binary Encoding for IPv4](#binary-encoding-for-ipv4)
  - [23. Powers of Two](#powers-of-two)
  - [24. VLSM](#vlsm)
  - [25. TCP Window and Throughput](#tcp-window-and-throughput)
  - [26. Common Networking Math Mistakes](#common-networking-math-mistakes)
  - [27. Study Framework for Elaborative Encoding](#study-framework-for-elaborative-encoding)
  - [28. Networking Math Concept Map](#networking-math-concept-map)
  - [29. Master Mental Model](#networking-math-master-mental-model)
  - [30. Practice Encoding Template](#practice-encoding-template)
  - [31. Best Way to Study Networking Math](#best-way-to-study-networking-math)
  - [32. Networking Math Final Summary](#networking-math-final-summary)
- [Networking for DevOps](#networking-for-devops)
  - [1. What Networking for DevOps Means](#what-networking-for-devops-means)
  - [2. Core DevOps Networking Mental Model](#core-devops-networking-mental-model)
  - [3. Most Important Concepts](#most-important-concepts)
  - [4. TCP vs. UDP](#tcp-vs-udp)
  - [5. DNS](#dns)
  - [6. HTTP and HTTPS](#http-and-https)
  - [7. TLS and Certificates](#tls-and-certificates)
  - [8. Firewalls and Security Rules](#firewalls-and-security-rules)
  - [9. NAT](#nat)
  - [10. CIDR and Subnets](#cidr-and-subnets)
  - [11. VPC / Virtual Network](#vpc-virtual-network)
  - [12. Public and Private Subnets](#public-and-private-subnets)
  - [13. Routing](#routing)
  - [14. Load Balancers](#load-balancers)
  - [15. Reverse Proxy](#reverse-proxy)
  - [16. Health Checks](#health-checks)
  - [17. Containers and Networking](#containers-and-networking)
  - [18. Kubernetes Networking](#kubernetes-networking)
  - [19. Service Discovery](#service-discovery)
  - [20. Service Mesh](#service-mesh)
  - [21. VPN, Peering, and Private Connectivity](#vpn-peering-and-private-connectivity)
  - [22. Observability for Networking](#observability-for-networking)
  - [23. Common DevOps Networking Tools](#common-devops-networking-tools)
  - [24. Troubleshooting Framework](#troubleshooting-framework)
  - [25. Common Real-World Problems](#common-real-world-problems)
  - [26. Network Security for DevOps](#network-security-for-devops)
  - [27. Cloud Networking Patterns](#cloud-networking-patterns)
  - [28. Timeouts, Retries, and Circuit Breakers](#timeouts-retries-and-circuit-breakers)
  - [29. Important Linux Networking Files](#important-linux-networking-files)
  - [30. Important Commands Summary](#important-commands-summary)
  - [31. DevOps Networking Debug Flow](#devops-networking-debug-flow)
  - [32. What DevOps Should Memorize](#what-devops-should-memorize)
  - [33. Simple DevOps Networking Checklist](#simple-devops-networking-checklist)
  - [34. DevOps Networking Final Mental Model](#devops-networking-final-mental-model)
- [VoIP / SIP for DevOps](#voip-sip-for-devops)
  - [1. VoIP](#voip)
  - [2. SIP](#sip)
  - [3. RTP](#rtp)
  - [4. Important SIP Messages](#important-sip-messages)
  - [5. Important SIP Response Codes](#important-sip-response-codes)
  - [6. PBX](#pbx)
  - [7. SIP Trunk](#sip-trunk)
  - [8. Asterisk](#asterisk)
  - [9. FreePBX](#freepbx)
  - [10. 3CX](#3cx)
  - [11. FreeSWITCH](#freeswitch)
  - [12. Main Platforms in This Category](#main-platforms-in-this-category)
  - [13. Typical VoIP Architecture](#typical-voip-architecture)
  - [14. VoIP Ports](#voip-ports)
  - [15. NAT Problems in VoIP](#nat-problems-in-voip)
  - [16. SIP ALG](#sip-alg)
  - [17. Codecs](#codecs)
  - [18. VoIP Quality Metrics](#voip-quality-metrics)
  - [19. Asterisk vs. FreePBX vs. 3CX vs. FreeSWITCH](#asterisk-vs-freepbx-vs-3cx-vs-freeswitch)
  - [20. DevOps Troubleshooting Flow for VoIP](#devops-troubleshooting-flow-for-voip)
  - [21. Useful VoIP Commands and Tools](#useful-voip-commands-and-tools)
  - [22. Simple VoIP Final Model](#simple-voip-final-model)
  - [References](#references)

---

<a id="elaborative-encoding-framework-for-networking-math"></a>

## Elaborative Encoding Framework for Networking Math

**Elaborative encoding** means you do not memorize formulas in isolation. You connect each formula to meaning, examples, visuals, real network situations, similar concepts, and common mistakes.

For networking math, this is especially useful because many topics look similar: bandwidth, throughput, latency, packet size, subnetting, CIDR, binary, and data transfer time.

[Back to Contents](#contents)

---

<a id="networking-math-core-idea"></a>

### 1. Core Idea

Instead of learning networking math as "use this formula when asked," learn it as:

> What is happening in the network, and which numbers describe it?

A useful mental pattern:

> Data moves through a network.

Networking math usually asks:

1. How much data?
2. How fast can it move?
3. How long will it take?
4. How is the address space divided?
5. How much overhead or loss affects the result?

[Back to Contents](#contents)

---

<a id="main-encoding-pattern"></a>

### 2. Main Encoding Pattern

For every networking math topic, encode it using this structure:

| Step | Question | Example |
| --- | --- | --- |
| Meaning | What does this value represent in real life? | Bandwidth = maximum possible data rate, or "pipe size." |
| Formula | How is it calculated? | `Transfer Time = File Size / Transfer Rate` |
| Units | Do the units cancel correctly? | `seconds = bits / (bits/second)` |
| Real-world story | Where would this happen? | Sending a 100 MB video over a 10 Mbps link. |
| Similar concepts | What can it be confused with? | Bandwidth, throughput, latency, jitter, packet loss. |

Related terms:

| Concept | Simple meaning |
| --- | --- |
| Bandwidth | Maximum possible rate |
| Throughput | Actual achieved rate |
| Latency | Delay before data arrives |
| Jitter | Variation in delay |
| Packet loss | Data that fails to arrive |

[Back to Contents](#contents)

---

<a id="core-networking-math-concepts"></a>

### 3. Core Networking Math Concepts

The main networking math areas are:

- Bits, bytes, and unit conversion.
- Transfer time.
- Bandwidth vs. throughput.
- Latency, RTT, propagation delay, and transmission delay.
- Packet overhead, packet loss, and jitter.
- Subnetting, CIDR, subnet masks, block size, and VLSM.
- TCP window size, RTT, and throughput.

[Back to Contents](#contents)

---

<a id="bits-bytes-and-unit-conversion"></a>

### 4. Bits, Bytes, and Unit Conversion

This is the foundation of almost all networking math.

**Key idea:** networking speeds are usually measured in **bits per second**, while file sizes are often measured in **bytes**.

```text
1 byte = 8 bits
1 MB = 8 Mb
```

Elaborative encoding:

> Networks count tiny signals called bits. Files are usually counted in bytes. I often need to multiply by 8.

Common units:

| Unit | Meaning |
| --- | --- |
| b | bit |
| B | byte |
| Kb | kilobit |
| KB | kilobyte |
| Mb | megabit |
| MB | megabyte |
| Gb | gigabit |
| GB | gigabyte |

Be careful: **Mb is not the same as MB.**

Example:

```text
50 MB * 8 = 400 Mb
```

So a 50 MB file is 400 megabits.

[Back to Contents](#contents)

---

<a id="transfer-time"></a>

### 5. Transfer Time

Key formula:

```text
Transfer Time = Data Size / Rate
seconds = bits / (bits/second)
```

Elaborative encoding:

> Time equals how much stuff divided by how fast it moves.

This is the same logic as:

```text
time = distance / speed
```

Example:

```text
100 MB = 800 Mb
800 Mb / 20 Mbps = 40 seconds
```

Memory hook:

> Big file = more time. Faster link = less time.

[Back to Contents](#contents)

---

<a id="bandwidth-vs-throughput"></a>

### 6. Bandwidth vs. Throughput

**Bandwidth** is the theoretical maximum data rate.

Example:

> This link supports 100 Mbps.

**Throughput** is the actual useful data rate achieved.

Example:

> I am only getting 72 Mbps.

Elaborative encoding:

> Bandwidth is the speed limit. Throughput is the actual speed.

Efficiency formula:

```text
Efficiency = (Throughput / Bandwidth) * 100%
```

Example:

```text
Bandwidth = 100 Mbps
Throughput = 80 Mbps
Efficiency = (80 / 100) * 100 = 80%
```

[Back to Contents](#contents)

---

<a id="latency"></a>

### 7. Latency

Latency is delay: the time it takes for data to travel from source to destination.

Elaborative encoding:

> Latency is waiting time.

Bandwidth is about **how much per second**. Latency is about **how long before anything arrives**.

Example: a video call may have high bandwidth but still feel bad if latency is high.

[Back to Contents](#contents)

---

<a id="round-trip-time-rtt"></a>

### 8. Round Trip Time (RTT)

RTT is the time for a signal to go to a destination and come back.

Formula:

```text
RTT = 2 * One-way Delay
```

Elaborative encoding:

> RTT is a ping going out and returning.

Example:

```text
One-way delay = 30 ms
RTT = 2 * 30 = 60 ms
```

[Back to Contents](#contents)

---

<a id="propagation-delay"></a>

### 9. Propagation Delay

Propagation delay is the time it takes a signal to physically travel through a medium.

Formula:

```text
Propagation Delay = Distance / Signal Speed
```

Elaborative encoding:

> This is travel time through the cable, fiber, or air.

Example:

```text
Distance = 2,000 km
Signal speed = 200,000 km/s
2,000 / 200,000 = 0.01 seconds = 10 ms
```

[Back to Contents](#contents)

---

<a id="transmission-delay"></a>

### 10. Transmission Delay

Transmission delay is the time needed to push all bits of a packet onto the link.

Formula:

```text
Transmission Delay = Packet Size / Link Bandwidth
```

Elaborative encoding:

> How long does it take to place the entire packet onto the wire?

Difference from propagation delay:

| Type | Simple question |
| --- | --- |
| Transmission delay | How long to push the bits out? |
| Propagation delay | How long for the bits to travel? |

Example:

```text
Packet size = 1,000 bits
Bandwidth = 1,000,000 bits/s
1,000 / 1,000,000 = 0.001 seconds = 1 ms
```

[Back to Contents](#contents)

---

<a id="total-delay"></a>

### 11. Total Delay

Networking delay often has multiple parts:

```text
Total Delay = Processing + Queuing + Transmission + Propagation
```

Elaborative encoding: imagine a packet like a person going through an airport.

| Network delay | Airport analogy |
| --- | --- |
| Processing delay | Checking the ticket |
| Queuing delay | Waiting in line |
| Transmission delay | Boarding everyone onto the plane |
| Propagation delay | Plane flying to destination |

[Back to Contents](#contents)

---

<a id="bandwidth-delay-product"></a>

### 12. Bandwidth-Delay Product

Bandwidth-delay product (BDP) tells you how much data can be **in flight** on a network path.

Formula:

```text
BDP = Bandwidth * RTT
```

Elaborative encoding:

> How much data fits inside the network pipe at once?

Example:

```text
Bandwidth = 100 Mbps = 100,000,000 bits/s
RTT = 50 ms = 0.05 seconds
BDP = 100,000,000 * 0.05 = 5,000,000 bits
5,000,000 / 8 = 625,000 bytes
```

So about 625 KB can be in flight.

[Back to Contents](#contents)

---

<a id="packet-overhead"></a>

### 13. Packet Overhead

Packets contain user data plus headers.

- **Payload** = useful user data.
- **Headers** = overhead.

Formula:

```text
Efficiency = (Payload / Total Packet Size) * 100%
```

Elaborative encoding:

> A package contains the item plus the box and label.

Example:

```text
Payload = 1,460 bytes
Total packet size = 1,500 bytes
Efficiency = (1,460 / 1,500) * 100 = 97.3%
```

[Back to Contents](#contents)

---

<a id="packet-loss"></a>

### 14. Packet Loss

Packet loss means some packets fail to reach the destination.

Formula:

```text
Packet Loss Rate = (Lost Packets / Sent Packets) * 100%
```

Example:

```text
Sent packets = 1,000
Lost packets = 20
Loss = (20 / 1,000) * 100 = 2%
```

Elaborative encoding:

> Packet loss is missing mail.

[Back to Contents](#contents)

---

<a id="jitter"></a>

### 15. Jitter

Jitter is variation in delay.

Elaborative encoding:

> Latency is the delay. Jitter is how uneven the delay is.

Example:

| Packet | Delay |
| --- | --- |
| 1 | 20 ms |
| 2 | 21 ms |
| 3 | 80 ms |
| 4 | 19 ms |

The average delay might not look terrible, but variation causes problems for voice and video.

[Back to Contents](#contents)

---

<a id="subnetting"></a>

### 16. Subnetting

Subnetting divides one IP network into smaller networks.

An IP address has two parts:

1. **Network part**
2. **Host part**

Example:

```text
192.168.1.10/24
```

The `/24` means the first 24 bits are the network part. IPv4 has 32 bits total:

```text
32 - 24 = 8 host bits
```

[Back to Contents](#contents)

---

<a id="number-of-hosts-in-a-subnet"></a>

### 17. Number of Hosts in a Subnet

Formula:

```text
Usable Hosts = 2^h - 2
h = number of host bits
```

The minus 2 exists because:

1. One address is the network address.
2. One address is the broadcast address.

Example for `/24`:

```text
32 - 24 = 8 host bits
2^8 - 2 = 256 - 2 = 254 usable hosts
```

Elaborative encoding:

> The host bits are the seats available for devices.

[Back to Contents](#contents)

---

<a id="cidr-prefixes"></a>

### 18. CIDR Prefixes

CIDR notation tells how many bits are used for the network.

Example:

```text
/24 = 24 network bits
Host Bits = 32 - Prefix Length
```

Common examples:

| CIDR | Host bits | Usable hosts |
| --- | ---: | ---: |
| /24 | 8 | 254 |
| /25 | 7 | 126 |
| /26 | 6 | 62 |
| /27 | 5 | 30 |
| /28 | 4 | 14 |
| /29 | 3 | 6 |
| /30 | 2 | 2 |

Memory hook:

> As the prefix gets larger, the subnet gets smaller. `/24` is bigger than `/28`.

[Back to Contents](#contents)

---

<a id="subnet-mask"></a>

### 19. Subnet Mask

A subnet mask shows which bits belong to the network.

Example:

```text
/24 = 255.255.255.0
```

Elaborative encoding:

> 255 means locked network bits. 0 means flexible host bits.

Common masks:

| CIDR | Subnet mask |
| --- | --- |
| /24 | 255.255.255.0 |
| /25 | 255.255.255.128 |
| /26 | 255.255.255.192 |
| /27 | 255.255.255.224 |
| /28 | 255.255.255.240 |
| /29 | 255.255.255.248 |
| /30 | 255.255.255.252 |

[Back to Contents](#contents)

---

<a id="block-size"></a>

### 20. Block Size

Block size helps find subnet ranges.

Formula:

```text
Block Size = 256 - Interesting Octet Mask Value
```

Example:

```text
Mask = 255.255.255.192
Interesting octet value = 192
Block size = 256 - 192 = 64
```

Subnet ranges increase by 64:

| Subnet | Range |
| --- | --- |
| 192.168.1.0/26 | .0 to .63 |
| 192.168.1.64/26 | .64 to .127 |
| 192.168.1.128/26 | .128 to .191 |
| 192.168.1.192/26 | .192 to .255 |

[Back to Contents](#contents)

---

<a id="network-address-and-broadcast-address"></a>

### 21. Network Address and Broadcast Address

For each subnet:

| Address type | Meaning |
| --- | --- |
| Network address | First address in subnet |
| Broadcast address | Last address in subnet |
| Usable host range | Addresses between them |

Example:

```text
Subnet = 192.168.1.64/26
Block size = 64
Range = 192.168.1.64 to 192.168.1.127
```

| Type | Address |
| --- | --- |
| Network address | 192.168.1.64 |
| First usable | 192.168.1.65 |
| Last usable | 192.168.1.126 |
| Broadcast | 192.168.1.127 |

[Back to Contents](#contents)

---

<a id="binary-encoding-for-ipv4"></a>

### 22. Binary Encoding for IPv4

IPv4 addresses are made of 32 bits and split into four octets.

Example:

```text
192.168.1.1
```

Each octet has 8 bits.

| Bit value | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |

Example: convert 192 to binary.

```text
192 = 128 + 64
192 = 11000000
```

Elaborative encoding:

> Binary is just choosing which place values are turned on.

[Back to Contents](#contents)

---

<a id="powers-of-two"></a>

### 23. Powers of Two

Subnetting depends heavily on powers of two.

| Power | Value |
| --- | ---: |
| 2^1 | 2 |
| 2^2 | 4 |
| 2^3 | 8 |
| 2^4 | 16 |
| 2^5 | 32 |
| 2^6 | 64 |
| 2^7 | 128 |
| 2^8 | 256 |
| 2^10 | 1024 |

Connect them to subnet sizes:

| Host bits | Total addresses |
| ---: | ---: |
| 8 | 256 |
| 7 | 128 |
| 6 | 64 |
| 5 | 32 |
| 4 | 16 |
| 3 | 8 |
| 2 | 4 |

[Back to Contents](#contents)

---

<a id="vlsm"></a>

### 24. VLSM

VLSM means **Variable Length Subnet Masking**. It allows different subnet sizes inside the same larger network.

Key idea:

> Give bigger subnets to groups that need more hosts, and smaller subnets to groups that need fewer hosts.

Elaborative encoding:

> VLSM is cutting a cake into unequal pieces based on need.

Simple process:

1. List host requirements from largest to smallest.
2. Choose the smallest subnet that fits each group.
3. Assign ranges in order.
4. Avoid overlap.

Example:

| Group | Hosts needed | Subnet |
| --- | ---: | --- |
| A | 100 | /25, 126 usable |
| B | 50 | /26, 62 usable |
| C | 20 | /27, 30 usable |
| D | 10 | /28, 14 usable |

[Back to Contents](#contents)

---

<a id="tcp-window-and-throughput"></a>

### 25. TCP Window and Throughput

TCP throughput can be limited by window size and RTT.

Formula:

```text
Throughput ~= TCP Window Size / RTT
```

Elaborative encoding:

> TCP can only send a window of data before waiting for acknowledgment.

A larger window allows more data in flight. A larger RTT makes waiting longer.

[Back to Contents](#contents)

---

<a id="common-networking-math-mistakes"></a>

### 26. Common Networking Math Mistakes

**Mistake 1: Mixing bits and bytes**

Wrong:

```text
100 MB / 10 Mbps = 10 seconds
```

Correct:

```text
100 MB = 800 Mb
800 / 10 = 80 seconds
```

**Mistake 2: Forgetting milliseconds**

```text
1 ms = 0.001 seconds
50 ms = 0.05 seconds
```

**Mistake 3: Thinking bigger CIDR means bigger network**

`/28` is smaller than `/24` because `/28` leaves fewer host bits.

**Mistake 4: Forgetting network and broadcast addresses**

For normal IPv4 host calculations:

```text
2^h - 2
```

not just:

```text
2^h
```

[Back to Contents](#contents)

---

<a id="study-framework-for-elaborative-encoding"></a>

### 27. Study Framework for Elaborative Encoding

Use this for every networking math topic:

| Step | Action | Example |
| --- | --- | --- |
| 1 | Name the concept | Bandwidth-delay product |
| 2 | Say it simply | How much data can be in the network path at once |
| 3 | Draw or imagine it | A long pipe filled with bits |
| 4 | Write the formula | `BDP = Bandwidth * RTT` |
| 5 | Explain variables | Bandwidth = send rate; RTT = round-trip delay |
| 6 | Check units | `bits/second * seconds = bits` |
| 7 | Attach an example | 100 Mbps with 50 ms RTT holds 5,000,000 bits |
| 8 | Compare | BDP is related to TCP window size |
| 9 | Create a hook | BDP is how much data fits in the pipe |

[Back to Contents](#contents)

---

<a id="networking-math-concept-map"></a>

### 28. Networking Math Concept Map

| Concept | Main question |
| --- | --- |
| Bits/bytes | Am I using the right unit? |
| Transfer time | How long does sending data take? |
| Bandwidth | What is the maximum possible rate? |
| Throughput | What is the actual rate? |
| Latency | How long is the delay? |
| RTT | How long for a round trip? |
| Propagation delay | How long to travel across distance? |
| Transmission delay | How long to push bits onto the link? |
| Packet loss | What percentage failed? |
| Jitter | How uneven is the delay? |
| Subnetting | How do I divide IP space? |
| CIDR | How many bits are network bits? |
| VLSM | How do I make unequal subnet sizes? |
| BDP | How much data is in flight? |

[Back to Contents](#contents)

---

<a id="networking-math-master-mental-model"></a>

### 29. Master Mental Model

Most networking math is based on this idea:

> Data has size. Networks have speed. Paths have delay. Addresses have structure.

When solving a problem, ask:

1. Is this about data size?
2. Is this about speed or rate?
3. Is this about time or delay?
4. Is this about IP address space?
5. Is this about efficiency, overhead, or loss?

[Back to Contents](#contents)

---

<a id="practice-encoding-template"></a>

### 30. Practice Encoding Template

Use this template for each formula:

```text
Concept:
Simple meaning:
Formula:
Units:
Real-world example:
Common mistake:
Memory hook:
Related concepts:
Practice problem:
```

Example:

```text
Concept: Transfer time
Simple meaning: How long it takes to send data.
Formula: Time = Data size / Rate
Units: seconds = bits / bits per second
Real-world example: Sending a 500 MB file over a 100 Mbps link.
Common mistake: Forgetting to convert MB to Mb.
Memory hook: Time = stuff / speed.
Related concepts: Bandwidth, throughput, file size, bits, bytes.
Practice problem: How long does a 1 GB file take over 50 Mbps?
```

[Back to Contents](#contents)

---

<a id="best-way-to-study-networking-math"></a>

### 31. Best Way to Study Networking Math

Use a three-pass method.

**Pass 1: Understand**

Say the concept in plain language.

> Subnetting is dividing one network into smaller networks.

**Pass 2: Connect**

Relate it to something familiar.

> Subnetting is like dividing a building into rooms.

**Pass 3: Calculate**

Use the formula with units.

```text
/26 -> 32 - 26 = 6 host bits
2^6 - 2 = 62 usable hosts
```

[Back to Contents](#contents)

---

<a id="networking-math-final-summary"></a>

### 32. Networking Math Final Summary

Elaborative encoding helps you learn networking math by connecting formulas to meaning.

Do not memorize formulas alone. For each topic, connect:

| Layer | Question |
| --- | --- |
| Meaning | What does this represent? |
| Formula | How is it calculated? |
| Units | Do the units cancel correctly? |
| Story | What real network situation uses it? |
| Comparison | What is it similar to or different from? |
| Mistake | What trap should I avoid? |

Strongest memory pattern:

> Meaning first, formula second, units always, example immediately.

[Back to Contents](#contents)

---

<a id="networking-for-devops"></a>

## Networking for DevOps

[Back to Contents](#contents)

---

<a id="what-networking-for-devops-means"></a>

### 1. What Networking for DevOps Means

Networking for DevOps is not about becoming a full network engineer. It means understanding enough networking to:

- Deploy applications correctly.
- Debug connection problems.
- Configure cloud infrastructure.
- Secure traffic.
- Troubleshoot DNS, load balancers, firewalls, containers, and Kubernetes.
- Understand how services talk to each other.

A DevOps engineer often sits between:

| Area | What you need to understand |
| --- | --- |
| Developers | How the app communicates |
| Cloud/Infrastructure | Where the app runs |
| Security | Who can access what |
| Networking | How traffic flows |
| Operations | How to troubleshoot failures |

Key question:

> How does traffic get from the user to the service, and back again?

[Back to Contents](#contents)

---

<a id="core-devops-networking-mental-model"></a>

### 2. Core DevOps Networking Mental Model

Most networking problems can be understood as a path:

```text
User
  -> DNS
  -> Internet
  -> Firewall / Security Rules
  -> Load Balancer
  -> Reverse Proxy / Ingress
  -> Application Service
  -> Container / Pod / VM
  -> Database / Cache / Internal Service
```

When something breaks, ask:

> Where in the path did the request fail?

[Back to Contents](#contents)

---

<a id="most-important-concepts"></a>

### 3. Most Important Concepts

**IP address** identifies a device or service on a network.

Examples:

```text
192.168.1.10
10.0.2.15
172.16.5.20
```

Simple meaning:

> Where is this machine or service?

**Port** identifies a specific service running on a machine.

| Port | Common use |
| ---: | --- |
| 22 | SSH |
| 53 | DNS |
| 80 | HTTP |
| 443 | HTTPS |
| 5432 | PostgreSQL |
| 3306 | MySQL |
| 6379 | Redis |
| 9092 | Kafka |

Simple meaning:

> Which door on the machine should traffic enter?

Example:

```text
10.0.1.5:5432
```

Means: go to machine `10.0.1.5` and connect to PostgreSQL on port `5432`.

**Protocol** is a rule system for communication.

| Protocol | Meaning |
| --- | --- |
| HTTP | Web traffic |
| HTTPS | Secure web traffic |
| TCP | Reliable connection-based transport |
| UDP | Fast connectionless transport |
| SSH | Secure remote shell |
| DNS | Name lookup |
| ICMP | Ping and network messages |

Simple meaning:

> What language are these systems speaking?

[Back to Contents](#contents)

---

<a id="tcp-vs-udp"></a>

### 4. TCP vs. UDP

**TCP** is reliable and connection-based. It checks that data arrives correctly.

Used by:

- HTTP and HTTPS.
- SSH.
- Databases.
- Most APIs.

Simple analogy:

> TCP is like a phone call. Both sides connect and confirm communication.

**UDP** is faster but does not guarantee delivery.

Used by:

- DNS.
- Streaming.
- VoIP.
- Some gaming traffic.
- Some observability protocols.

Simple analogy:

> UDP is like sending postcards. Fast, but no guarantee each one arrives.

DevOps importance: know whether a service uses TCP or UDP. DNS often uses UDP port 53, but can also use TCP port 53. A firewall rule allowing TCP 53 but blocking UDP 53 may cause DNS issues.

[Back to Contents](#contents)

---

<a id="dns"></a>

### 5. DNS

DNS translates names into IP addresses.

```text
api.example.com -> 34.120.80.10
```

Simple meaning:

> DNS is the internet's phonebook.

DevOps uses DNS for domain names, service discovery, load balancers, Kubernetes services, internal cloud services, database endpoints, blue-green deployments, and failover.

DNS record types:

| Record | Purpose |
| --- | --- |
| A | Name to IPv4 address |
| AAAA | Name to IPv6 address |
| CNAME | Alias to another name |
| MX | Mail server |
| TXT | Text verification, SPF, DKIM, etc. |
| NS | Nameserver |
| SRV | Service location |

Example:

```text
app.example.com CNAME my-load-balancer.cloudprovider.com
```

Common DNS problems:

| Problem | Explanation |
| --- | --- |
| Wrong DNS record | Domain points to wrong place |
| DNS propagation delay | Changes take time to spread |
| Bad TTL | Old records are cached |
| Internal vs. external DNS confusion | Works inside cloud, fails outside |
| Split-horizon DNS | Same name resolves differently depending on network |

Useful commands:

```bash
dig app.example.com
nslookup app.example.com
```

[Back to Contents](#contents)

---

<a id="http-and-https"></a>

### 6. HTTP and HTTPS

HTTP is the protocol most web applications use. HTTPS is HTTP encrypted with TLS.

Common DevOps tasks:

- Configure HTTPS certificates.
- Set reverse proxies.
- Route paths.
- Debug API failures.
- Check headers.
- Manage redirects.
- Configure health checks.

HTTP request flow:

```text
Client sends request
  -> Server processes it
  -> Server sends response
```

Example request:

```http
GET /api/users HTTP/1.1
Host: example.com
```

Example response:

```http
HTTP/1.1 200 OK
Content-Type: application/json
```

Common HTTP status codes:

| Code | Meaning | DevOps interpretation |
| ---: | --- | --- |
| 200 | OK | Request succeeded |
| 301 / 302 | Redirect | Check routing or HTTPS redirect |
| 400 | Bad request | Client sent invalid data |
| 401 | Unauthorized | Authentication missing or wrong |
| 403 | Forbidden | Authenticated but not allowed |
| 404 | Not found | Wrong path, route, or service |
| 408 | Timeout | Request took too long |
| 429 | Too many requests | Rate limit hit |
| 500 | Server error | App crashed or failed |
| 502 | Bad gateway | Proxy/load balancer cannot reach backend properly |
| 503 | Service unavailable | Backend unavailable or overloaded |
| 504 | Gateway timeout | Backend did not respond in time |

Debugging shortcut:

```text
4xx usually means client/request/auth/routing issue.
5xx usually means server/backend/proxy/infrastructure issue.
```

[Back to Contents](#contents)

---

<a id="tls-and-certificates"></a>

### 7. TLS and Certificates

TLS encrypts traffic. HTTPS uses TLS. A certificate proves that a domain belongs to the server.

For `https://example.com`, the browser checks:

1. Is the certificate valid?
2. Is it expired?
3. Does it match the domain?
4. Was it issued by a trusted certificate authority?

Common TLS problems:

| Problem | Meaning |
| --- | --- |
| Expired certificate | Certificate needs renewal |
| Wrong hostname | Certificate does not match domain |
| Missing intermediate certificate | Chain is incomplete |
| Self-signed certificate | Client may not trust it |
| TLS version mismatch | Client/server support different versions |

Useful command:

```bash
openssl s_client -connect example.com:443 -servername example.com
```

[Back to Contents](#contents)

---

<a id="firewalls-and-security-rules"></a>

### 8. Firewalls and Security Rules

A firewall controls traffic. It usually answers:

```text
Who can connect?
To what?
On which port?
Using which protocol?
```

Examples:

```text
Allow TCP 443 from 0.0.0.0/0 to web load balancer
Allow TCP 5432 from app servers to database
```

DevOps examples:

| Task | Networking idea |
| --- | --- |
| App cannot connect to DB | Check security group/firewall |
| SSH fails | Check port 22 and source IP |
| API works locally but not in cloud | Check inbound/outbound rules |
| Kubernetes pod cannot reach service | Check NetworkPolicy |
| Load balancer health check fails | Check backend port and allowed sources |

[Back to Contents](#contents)

---

<a id="nat"></a>

### 9. NAT

NAT means **Network Address Translation**. It changes private IPs into public IPs, or maps one address to another.

Simple meaning:

> NAT lets private machines access the internet using shared public addresses.

Common private IP ranges:

```text
10.0.0.0/8
172.16.0.0/12
192.168.0.0/16
```

Common NAT use:

```text
Private server -> NAT Gateway -> Internet
```

The private server can download updates without needing a public IP.

[Back to Contents](#contents)

---

<a id="cidr-and-subnets"></a>

### 10. CIDR and Subnets

CIDR describes IP ranges.

Example:

```text
10.0.1.0/24
```

This means:

- Network starts at `10.0.1.0`.
- `/24` means 24 network bits.
- Leaves 8 host bits.
- Gives 256 total addresses.
- Usually 254 usable addresses in traditional IPv4 subnetting.

DevOps use cases:

- VPC design.
- Subnet creation.
- Firewall rules.
- Kubernetes pod networks.
- VPN routing.
- Allowlists.
- Database access rules.

Simple subnet model:

```text
VPC:      10.0.0.0/16
Subnet A: 10.0.1.0/24
Subnet B: 10.0.2.0/24
Subnet C: 10.0.3.0/24
```

[Back to Contents](#contents)

---

<a id="vpc-virtual-network"></a>

### 11. VPC / Virtual Network

A VPC is a private network in the cloud.

| Provider | Name |
| --- | --- |
| AWS | VPC |
| Azure | Virtual Network / VNet |
| Google Cloud | VPC Network |

Simple meaning:

> A VPC is your private network area in the cloud.

Inside a VPC, you usually have subnets, route tables, internet gateways, NAT gateways, firewalls/security groups, private IPs, load balancers, VPNs, or peering connections.

[Back to Contents](#contents)

---

<a id="public-and-private-subnets"></a>

### 12. Public and Private Subnets

**Public subnet:** can route directly to the internet.

Usually contains:

- Public load balancers.
- Bastion hosts.
- NAT gateways.

**Private subnet:** does not receive direct inbound internet traffic.

Usually contains:

- App servers.
- Containers.
- Databases.
- Internal services.

Common architecture:

```text
Internet
  -> Public Load Balancer
  -> Private App Servers
  -> Private Database
```

[Back to Contents](#contents)

---

<a id="routing"></a>

### 13. Routing

Routing decides where network traffic goes.

A route table says:

```text
For this destination, send traffic here.
```

Example:

| Destination | Target |
| --- | --- |
| 10.0.0.0/16 | local |
| 0.0.0.0/0 | internet gateway |

`0.0.0.0/0` means "everything else."

Common routing problems:

| Symptom | Possible cause |
| --- | --- |
| Private server cannot reach internet | Missing NAT route |
| Public server cannot be reached | Missing internet gateway route |
| VPN connected but resources unreachable | Route table missing remote CIDR |
| VPC peering works one way only | Return route missing |
| Kubernetes pods cannot reach outside | CNI or route issue |

[Back to Contents](#contents)

---

<a id="load-balancers"></a>

### 14. Load Balancers

A load balancer distributes traffic across multiple backends.

Simple meaning:

> It is the front door that sends users to healthy servers.

Why use load balancers?

- High availability.
- Scaling.
- Health checks.
- TLS termination.
- Traffic distribution.
- Zero-downtime deployments.
- Routing to multiple services.

Types:

| Type | Layer | Use |
| --- | ---: | --- |
| Network Load Balancer | Layer 4 | TCP/UDP traffic |
| Application Load Balancer | Layer 7 | HTTP/HTTPS routing |
| Internal Load Balancer | Private | Internal services |
| External Load Balancer | Public | Internet-facing apps |

Layer 4 uses IPs and ports. Layer 7 understands HTTP concepts such as hostname, path, headers, methods, and cookies.

[Back to Contents](#contents)

---

<a id="reverse-proxy"></a>

### 15. Reverse Proxy

A reverse proxy sits in front of applications.

Examples:

- Nginx.
- HAProxy.
- Envoy.
- Traefik.
- Apache.
- Caddy.

Simple meaning:

> A reverse proxy receives requests and forwards them to backend services.

Common jobs:

- TLS termination.
- Path routing.
- Compression.
- Caching.
- Rate limiting.
- Authentication.
- Header manipulation.
- Load balancing.

Example Nginx idea:

```nginx
location /api {
    proxy_pass http://api-service:8080;
}
```

[Back to Contents](#contents)

---

<a id="health-checks"></a>

### 16. Health Checks

Health checks tell infrastructure whether a backend is working.

Example:

```text
GET /health -> 200 OK
```

If the app returns bad responses, the load balancer may stop sending traffic to it.

Common issues:

| Issue | Result |
| --- | --- |
| Wrong health check path | Backend marked unhealthy |
| Wrong port | Health check fails |
| App starts slowly | Marked unhealthy during startup |
| Auth required on health endpoint | Health check gets 401/403 |
| Firewall blocks health checker | Backend looks dead |

[Back to Contents](#contents)

---

<a id="containers-and-networking"></a>

### 17. Containers and Networking

Containers have their own network environment:

- Own IP.
- Own ports.
- Isolated network namespace.
- Bridge or overlay network.
- Service discovery through names.

Docker port mapping:

```bash
docker run -p 8080:80 nginx
```

Meaning:

```text
Host port 8080 -> Container port 80
```

Common mistake: the app inside the container listens only on `127.0.0.1`. It often needs to listen on `0.0.0.0`.

| Address | Meaning |
| --- | --- |
| 127.0.0.1 | Only inside the same container/machine |
| 0.0.0.0 | Listen on all network interfaces |

[Back to Contents](#contents)

---

<a id="kubernetes-networking"></a>

### 18. Kubernetes Networking

Core idea:

> Pods are temporary, so Kubernetes gives stable service names for reaching them.

Important objects:

| Object | Networking role |
| --- | --- |
| Pod | Runs containers |
| Service | Stable internal access to pods |
| Ingress | HTTP/HTTPS access from outside |
| NetworkPolicy | Firewall rules inside cluster |
| CNI | Plugin that provides pod networking |
| Endpoint / EndpointSlice | Actual pod IPs behind a service |

Each pod gets its own IP, but pod IPs are not stable. A Kubernetes Service gives a stable name and IP for a group of pods.

Example:

```text
api-service.default.svc.cluster.local
```

Service types:

| Type | Meaning |
| --- | --- |
| ClusterIP | Internal-only service |
| NodePort | Exposes service on each node's port |
| LoadBalancer | Creates external cloud load balancer |
| ExternalName | Maps service to external DNS name |

Ingress manages external HTTP/HTTPS access:

```text
app.example.com/api -> api-service
app.example.com/web -> web-service
```

NetworkPolicy controls traffic between pods, for example:

```text
Only frontend pods can talk to backend pods.
Only backend pods can talk to database pods.
```

[Back to Contents](#contents)

---

<a id="service-discovery"></a>

### 19. Service Discovery

Service discovery helps services find each other.

Examples:

```text
postgres.default.svc.cluster.local
mydb.xxxxxx.region.rds.amazonaws.com
```

Simple meaning:

> Services should not need to know fixed IP addresses.

[Back to Contents](#contents)

---

<a id="service-mesh"></a>

### 20. Service Mesh

A service mesh manages service-to-service traffic.

Examples:

- Istio.
- Linkerd.
- Consul.
- Kuma.

Common features:

- mTLS.
- Retries.
- Traffic splitting.
- Observability.
- Circuit breaking.
- Authorization policies.

Simple meaning:

> A service mesh adds advanced traffic control between services.

Use it when internal service communication becomes complex. Do not use it just because it sounds advanced.

[Back to Contents](#contents)

---

<a id="vpn-peering-and-private-connectivity"></a>

### 21. VPN, Peering, and Private Connectivity

DevOps often connects private networks.

| Technology | Simple meaning |
| --- | --- |
| VPN | Encrypted connectivity between networks, such as office network to cloud VPC |
| VPC peering | Private connectivity between two cloud networks |
| Private Endpoint / PrivateLink | Private access to services without public internet exposure |

[Back to Contents](#contents)

---

<a id="observability-for-networking"></a>

### 22. Observability for Networking

Useful networking signals:

| Signal | Meaning |
| --- | --- |
| Latency | How long requests take |
| Error rate | How many requests fail |
| Throughput | How much traffic is served |
| Saturation | Whether resources are overloaded |
| Packet loss | Whether packets disappear |
| DNS failures | Whether names resolve correctly |
| Connection resets | Whether connections are being dropped |

Important metrics:

| Area | Metrics |
| --- | --- |
| Load balancers | Request count, target response time, 4xx count, 5xx count, unhealthy host count |
| Services | Request duration, error rate, active connections, connection pool usage, timeout count |
| Infrastructure | Network in/out, packet drops, NAT gateway errors, firewall denies, DNS query failures |

[Back to Contents](#contents)

---

<a id="common-devops-networking-tools"></a>

### 23. Common DevOps Networking Tools

| Tool | Use |
| --- | --- |
| `curl -v https://example.com` | Test HTTP/HTTPS, headers, redirects, status codes |
| `ping 8.8.8.8` | Test basic ICMP reachability, if ICMP is allowed |
| `traceroute example.com` / `tracepath example.com` | Show packet path |
| `dig example.com` | Check DNS records and TTLs |
| `nslookup example.com` | DNS lookup |
| `nc -vz example.com 443` | Test whether a TCP port is reachable |
| `ss -tulnp` | Show sockets and listening ports |
| `sudo tcpdump -i eth0 port 443` | Capture packets |
| `ip addr` / `ip route` | Show IP addresses, interfaces, and routes |

[Back to Contents](#contents)

---

<a id="troubleshooting-framework"></a>

### 24. Troubleshooting Framework

Use this checklist:

1. **Is DNS working?** Run `dig app.example.com`.
2. **Is the port reachable?** Run `nc -vz app.example.com 443`.
3. **Is the service responding?** Run `curl -v https://app.example.com/health`.
4. **Is the load balancer healthy?** Check targets, backend port, health check path, and rules.
5. **Is the app listening correctly?** Run `ss -tulnp`.
6. **Are firewalls allowing traffic?** Check security groups, ACLs, OS firewall, NetworkPolicy, and mesh policy.
7. **Is routing correct?** Check `ip route`, route tables, NAT, internet gateways, peering, and VPN routes.

[Back to Contents](#contents)

---

<a id="common-real-world-problems"></a>

### 25. Common Real-World Problems

**Website returns 502**

Likely causes:

- Reverse proxy cannot reach backend.
- Backend service is down.
- Wrong backend port.
- Backend crashed.
- Firewall blocks proxy to app.
- App returns invalid response.

Debug with `curl -v backend:port`, `ss -tulnp`, `docker ps`, `kubectl get pods`, and `kubectl logs`.

**Website returns 504**

Likely causes:

- Backend is too slow.
- Timeout too short.
- Database is slow.
- App is stuck.
- Network path is blocked or delayed.

**App cannot connect to database**

Likely causes:

- Wrong database hostname or port.
- Firewall/security group blocked.
- Database not listening publicly or privately.
- Route missing.
- Credentials wrong.
- TLS requirement mismatch.

Debug:

```bash
dig db.example.internal
nc -vz db.example.internal 5432
```

**DNS resolves locally but not in cloud**

Likely causes: private hosted zone association, split-horizon DNS, VPC DNS settings, or resolver configuration.

**Kubernetes service not reachable**

Likely causes: pods not ready, service selector mismatch, wrong `targetPort`, NetworkPolicy block, app listening on wrong interface, or bad Ingress rule.

Debug:

```bash
kubectl get svc
kubectl get endpoints
kubectl get pods --show-labels
kubectl describe ingress
```

[Back to Contents](#contents)

---

<a id="network-security-for-devops"></a>

### 26. Network Security for DevOps

Good DevOps networking is also secure networking.

| Principle | Meaning |
| --- | --- |
| Least privilege | Allow only required traffic |
| Private by default | Keep internal systems private |
| Defense in depth | Use multiple layers of protection |
| Encrypt in transit | Use TLS/mTLS where needed |
| Segment networks | Separate public, app, and database layers |
| Log network access | Keep audit visibility |

Bad pattern:

```text
Database allows 0.0.0.0/0 on port 5432
```

Better pattern:

```text
Database allows TCP 5432 only from app subnet or app security group.
```

[Back to Contents](#contents)

---

<a id="cloud-networking-patterns"></a>

### 27. Cloud Networking Patterns

Basic web app:

```text
Internet
  -> DNS
  -> Public Load Balancer
  -> Private App Servers
  -> Private Database
```

Containerized app:

```text
Internet
  -> Cloud Load Balancer
  -> Kubernetes Ingress
  -> Kubernetes Service
  -> Pods
  -> Database
```

Private enterprise app:

```text
Corporate Network
  -> VPN / Direct Connect
  -> Private Load Balancer
  -> App Services
  -> Database
```

Microservices app:

```text
Gateway
  -> Service A
  -> Service B
  -> Service C
  -> Database / Queue / Cache
```

Add observability, retries, timeouts, and security policies.

[Back to Contents](#contents)

---

<a id="timeouts-retries-and-circuit-breakers"></a>

### 28. Timeouts, Retries, and Circuit Breakers

Networking is also about failure behavior.

| Pattern | Meaning |
| --- | --- |
| Timeout | Stop waiting after this long |
| Retry | Try again after failure |
| Circuit breaker | Stop sending traffic to a failing service for a while |

DevOps rule: always think about:

```text
connect timeout
request timeout
idle timeout
retry count
backoff
rate limits
```

[Back to Contents](#contents)

---

<a id="important-linux-networking-files"></a>

### 29. Important Linux Networking Files

| File | Purpose |
| --- | --- |
| `/etc/hosts` | Manual hostname mappings |
| `/etc/resolv.conf` | DNS resolver configuration |
| `/etc/nsswitch.conf` | Name resolution order |
| `/etc/services` | Common port names |
| `/proc/net/*` | Kernel networking info |

[Back to Contents](#contents)

---

<a id="important-commands-summary"></a>

### 30. Important Commands Summary

| Command | Use |
| --- | --- |
| `curl -v URL` | Test HTTP/HTTPS |
| `dig name` | Check DNS |
| `nslookup name` | Check DNS |
| `nc -vz host port` | Test TCP port |
| `ping host` | Test basic reachability |
| `traceroute host` | See network path |
| `ss -tulnp` | Show listening ports |
| `ip addr` | Show IP addresses |
| `ip route` | Show routing table |
| `tcpdump` | Capture packets |
| `kubectl get svc` | Check Kubernetes services |
| `kubectl get endpoints` | Check service backends |
| `kubectl describe ingress` | Check ingress routing |

[Back to Contents](#contents)

---

<a id="devops-networking-debug-flow"></a>

### 31. DevOps Networking Debug Flow

Use this when something does not work:

```text
1. Name resolution: can the hostname resolve?
2. Routing: is there a route to the destination?
3. Firewall: is traffic allowed?
4. Port: is the destination port open?
5. Service: is the app listening?
6. Proxy/load balancer: is traffic forwarded correctly?
7. Health check: is the backend marked healthy?
8. Logs and metrics: what do the systems say?
9. TLS/auth: is encryption or authentication failing?
10. Return path: can the response get back?
```

The return path matters. Sometimes the request reaches the server, but the response cannot return correctly.

[Back to Contents](#contents)

---

<a id="what-devops-should-memorize"></a>

### 32. What DevOps Should Memorize

Must know:

```text
HTTP: 80
HTTPS: 443
SSH: 22
DNS: 53
PostgreSQL: 5432
MySQL: 3306
Redis: 6379
```

Must understand:

```text
IP + Port + Protocol
DNS resolution
CIDR basics
Public vs. private subnets
Firewall rules
Load balancers
TLS certificates
HTTP status codes
Kubernetes Services and Ingress
```

Must be able to debug:

```text
DNS failure
Port blocked
Wrong route
Bad TLS certificate
Load balancer unhealthy backend
Kubernetes service selector mismatch
App listening on wrong interface
Database connectivity failure
```

[Back to Contents](#contents)

---

<a id="simple-devops-networking-checklist"></a>

### 33. Simple DevOps Networking Checklist

Before deploying an app, ask:

| Question | Why it matters |
| --- | --- |
| What domain points to it? | DNS |
| Is HTTPS configured? | Security |
| Which port does the app listen on? | Routing |
| Is the app behind a load balancer? | Availability |
| Is the health check correct? | Reliability |
| Is the backend private? | Security |
| Can the app reach the database? | Connectivity |
| Are firewall rules least-privilege? | Security |
| Are logs and metrics enabled? | Troubleshooting |
| Are timeouts configured? | Stability |

[Back to Contents](#contents)

---

<a id="devops-networking-final-mental-model"></a>

### 34. DevOps Networking Final Mental Model

For DevOps, networking is mostly about controlled traffic flow.

Remember:

> Name resolves to address. Address routes to network. Firewall allows port. Proxy forwards request. Service responds. Logs prove what happened.

That sentence covers most DevOps networking troubleshooting.

[Back to Contents](#contents)

---

<a id="voip-sip-for-devops"></a>

## VoIP / SIP for DevOps

[Back to Contents](#contents)

---

<a id="voip"></a>

### 1. VoIP

**VoIP** means **Voice over Internet Protocol**.

Simple meaning:

> Phone calls over IP networks instead of old telephone lines.

VoIP can carry:

| Type | Example |
| --- | --- |
| Voice calls | Office phones, softphones |
| Video calls | Video meetings |
| Messaging | Chat, presence |
| Fax over IP | T.38 fax |
| Call center traffic | Queues, agents, IVR |

[Back to Contents](#contents)

---

<a id="sip"></a>

### 2. SIP

**SIP** means **Session Initiation Protocol**.

Simple meaning:

> SIP sets up, manages, and ends calls.

SIP jobs:

| Job | Example |
| --- | --- |
| Registering phones | Extension 101 logs in |
| Starting calls | INVITE |
| Ending calls | BYE |
| Ringing | 180 Ringing |
| Authentication | Username/password |
| Routing calls | Send call to trunk/provider |

Common SIP ports:

| Protocol | Port |
| --- | ---: |
| SIP over UDP/TCP | 5060 |
| SIP over TLS | 5061 |

[Back to Contents](#contents)

---

<a id="rtp"></a>

### 3. RTP

SIP does **call control**, but it usually does not carry the actual voice. Audio usually travels through **RTP**.

Simple model:

```text
SIP = call setup
RTP = voice/audio stream
```

Example:

```text
Phone A -- SIP --> PBX -- SIP --> Phone B
Phone A -- RTP audio -----------> Phone B
```

This is why a call can ring successfully but have no audio:

- SIP signaling works.
- RTP media path is broken.

[Back to Contents](#contents)

---

<a id="important-sip-messages"></a>

### 4. Important SIP Messages

| SIP message | Meaning |
| --- | --- |
| REGISTER | Phone logs into PBX |
| INVITE | Start a call |
| TRYING | Request received |
| RINGING | Destination is ringing |
| OK / 200 OK | Call accepted |
| ACK | Confirmation |
| BYE | End call |
| CANCEL | Cancel call before answer |
| OPTIONS | Health/check capability |
| REFER | Transfer call |

[Back to Contents](#contents)

---

<a id="important-sip-response-codes"></a>

### 5. Important SIP Response Codes

| Code | Meaning |
| ---: | --- |
| 100 | Trying |
| 180 | Ringing |
| 183 | Session Progress |
| 200 | OK |
| 401 / 407 | Authentication required |
| 403 | Forbidden |
| 404 | Not found |
| 408 | Timeout |
| 480 | Temporarily unavailable |
| 486 | Busy here |
| 487 | Request terminated |
| 488 | Not acceptable here |
| 503 | Service unavailable |

[Back to Contents](#contents)

---

<a id="pbx"></a>

### 6. PBX

**PBX** means **Private Branch Exchange**.

Simple meaning:

> A private phone system for an organization.

A PBX handles extensions, call routing, voicemail, IVR menus, queues, ring groups, call recording, SIP trunks, conferences, forwarding, business hours, and caller ID rules.

Example:

```text
Customer
  -> SIP Trunk
  -> PBX
  -> IVR: Press 1 for Sales, 2 for Support
  -> Agent Extension
```

[Back to Contents](#contents)

---

<a id="sip-trunk"></a>

### 7. SIP Trunk

A **SIP trunk** connects your PBX to the public phone network.

Simple meaning:

> A virtual phone line from a VoIP provider.

Without a SIP trunk, a PBX may only call internal extensions. With a SIP trunk, it can call mobile phones, landlines, international numbers, and emergency services depending on provider and region.

[Back to Contents](#contents)

---

<a id="asterisk"></a>

### 8. Asterisk

**Asterisk** is a free and open-source communications framework used to build PBX and telephony systems. It is sponsored by Sangoma.

Simple meaning:

> Asterisk is the engine for building custom phone systems.

Asterisk can handle SIP extensions, SIP trunks, IVR, queues, voicemail, call recording, dial plans, conferencing, PSTN gateways, and custom telephony apps.

| Use case | Fit |
| --- | --- |
| Custom PBX | Excellent |
| Learning SIP deeply | Excellent |
| Highly customized call routing | Excellent |
| Beginner-friendly GUI | Not by itself |
| Enterprise support | Possible through vendors |

Asterisk is powerful, but it often requires understanding config files and dial plans.

[Back to Contents](#contents)

---

<a id="freepbx"></a>

### 9. FreePBX

**FreePBX** is a web GUI built around Asterisk. The Asterisk project describes FreePBX as a leading open-source GUI for Asterisk that makes it easier to build custom PBX systems.

Simple meaning:

> FreePBX is Asterisk with a web dashboard.

Best for small business PBX, lab PBX, call routing without raw Asterisk config, SIP trunks, extensions, IVR, queues, and voicemail.

Mental model:

```text
Asterisk = engine
FreePBX = dashboard
```

[Back to Contents](#contents)

---

<a id="3cx"></a>

### 10. 3CX

**3CX** is a commercial IP PBX / unified communications platform. It supports hosted, self-hosted, and on-premise deployment, with web/mobile apps, SIP phones, video conferencing, live chat, and contact center features.

Simple meaning:

> 3CX is a business-ready phone system with easier management.

Best for companies that want a polished GUI, SIP trunks, mobile/web softphones, call queues, contact center features, Windows/Linux deployments, and hosted or self-hosted PBX.

Compared with Asterisk:

| Feature | Asterisk | 3CX |
| --- | --- | --- |
| Type | Open-source framework | Commercial PBX/UC system |
| Flexibility | Very high | Medium-high |
| Ease of use | Harder | Easier |
| GUI | Needs FreePBX or similar | Built in |
| Custom dial logic | Strong | More limited |
| Business deployment | Good with expertise | Very strong |

[Back to Contents](#contents)

---

<a id="freeswitch"></a>

### 11. FreeSWITCH

**FreeSWITCH** is an open-source communication framework for voice, video, messaging, WebRTC, and software-defined telecom.

Simple meaning:

> FreeSWITCH is a scalable telecom engine, often used for large or custom voice platforms.

Best for carriers, conferencing, WebRTC, large call volumes, custom telecom applications, media handling, and SIP routing/bridging.

Compared with Asterisk:

| Feature | Asterisk | FreeSWITCH |
| --- | --- | --- |
| Common use | PBX and call apps | Scalable telecom/media platform |
| Learning curve | Medium-high | High |
| PBX use | Very common | Possible |
| Large-scale media | Good | Very strong |
| Custom telecom platform | Good | Very strong |

[Back to Contents](#contents)

---

<a id="main-platforms-in-this-category"></a>

### 12. Main Platforms in This Category

Open-source PBX / telephony engines:

| Platform | Simple description |
| --- | --- |
| Asterisk | Open-source PBX and telephony toolkit |
| FreeSWITCH | Scalable telecom/media framework |
| Yate | Telephony engine for VoIP and telecom routing |
| Kamailio | High-performance SIP server/proxy |
| OpenSIPS | SIP proxy/server for routing and scaling SIP |
| Routr | Lightweight SIP server for modern deployments |
| reSIProcate | SIP stack and tools |
| SEMS | SIP Express Media Server |

Important distinction:

```text
Asterisk / FreeSWITCH = PBX/media/call handling
Kamailio / OpenSIPS = SIP routing/proxy/scaling
```

Open-source PBX distributions / GUIs:

| Platform | Built on | Simple description |
| --- | --- | --- |
| FreePBX | Asterisk | Popular GUI-based PBX |
| Issabel | Asterisk | Elastix-style open-source PBX distro |
| VitalPBX | Asterisk | PBX platform with GUI and modules |
| Incredible PBX | Asterisk/FreePBX | Prebuilt PBX distribution |
| FusionPBX | FreeSWITCH | Web GUI for FreeSWITCH |
| ASTPP | FreeSWITCH | VoIP billing and softswitch platform |
| Kazoo | FreeSWITCH-based | Cloud communications platform |
| Wazo | Asterisk-based | UC and IPBX platform |
| XiVO | Asterisk-based | Open-source IPBX/UC platform |

Commercial / business PBX and UC systems:

| Platform | Simple description |
| --- | --- |
| 3CX | Commercial IP PBX and UC platform |
| Sangoma PBXact | Commercial PBX based on Asterisk/FreePBX ecosystem |
| Cisco Unified Communications Manager | Enterprise Cisco call control |
| Avaya Aura | Enterprise UC/PBX platform |
| Mitel MiVoice | Business phone and UC platform |
| Yeastar P-Series PBX | Appliance/cloud PBX |
| Grandstream UCM | IP PBX appliances |
| NEC Univerge | Enterprise PBX/UC |
| Alcatel-Lucent Enterprise OmniPCX | Enterprise PBX |
| Wildix | Cloud/browser-based UC |
| Zultys MX | Business IP PBX |
| PortaSwitch | Carrier-grade softswitch/billing platform |

Cloud phone systems / UCaaS:

| Platform | Simple description |
| --- | --- |
| RingCentral | Cloud business phone/UC |
| Zoom Phone | Cloud phone system inside Zoom ecosystem |
| Microsoft Teams Phone | Calling/PBX features for Teams |
| Google Voice for Business | Cloud phone service |
| Vonage Business Communications | Cloud UC/phone system |
| Dialpad | AI-focused cloud phone/contact center |
| GoTo Connect | Cloud phone and meetings |
| Nextiva | Cloud business communications |
| 8x8 | Cloud UC/contact center |
| Webex Calling | Cisco cloud calling |
| Aircall | Cloud phone system for support/sales |
| OpenPhone | Lightweight business phone system |

Contact center platforms:

| Platform | Simple description |
| --- | --- |
| Genesys Cloud CX | Enterprise contact center |
| Amazon Connect | AWS cloud contact center |
| Five9 | Cloud contact center |
| NICE CXone | Enterprise contact center |
| Talkdesk | Cloud contact center |
| Twilio Flex | Programmable contact center |
| Zendesk Talk | Support-focused calling |
| Freshcaller / Freshdesk Contact Center | Support phone system |

SIP trunk / CPaaS providers:

| Provider | Simple description |
| --- | --- |
| Twilio Elastic SIP Trunking | Programmable SIP trunks |
| Telnyx | SIP trunking and programmable voice |
| Flowroute | SIP trunking |
| Bandwidth | Voice, messaging, emergency services |
| Plivo | Voice/SMS APIs |
| SignalWire | Programmable telecom, FreeSWITCH-related ecosystem |
| VoIP.ms | SIP trunking |
| DIDWW | Global SIP trunking and DIDs |
| Anveo Direct | SIP trunking |
| SIP.US | SIP trunking |

These providers are not usually PBXs. They connect a PBX to the phone network.

[Back to Contents](#contents)

---

<a id="typical-voip-architecture"></a>

### 13. Typical VoIP Architecture

Basic flow:

```text
IP Phone / Softphone
  -> SIP REGISTER
  -> PBX
  -> SIP trunk
  -> VoIP Provider
  -> Public Telephone Network
  -> Mobile / Landline
```

Company flow:

```text
Users
  -> DNS
  -> Firewall / NAT
  -> PBX: Asterisk, FreePBX, 3CX, etc.
  -> SIP Trunk Provider
  -> Public Phone Network
```

[Back to Contents](#contents)

---

<a id="voip-ports"></a>

### 14. VoIP Ports

Common ports:

| Purpose | Port |
| --- | ---: |
| SIP UDP/TCP | 5060 |
| SIP TLS | 5061 |
| RTP audio | Usually dynamic UDP range |
| HTTP admin | 80 |
| HTTPS admin | 443 |
| IAX2, Asterisk-specific | 4569 UDP |
| STUN | 3478 UDP |

RTP port ranges depend on the platform:

| Platform | Common RTP range |
| --- | --- |
| Asterisk | Often 10000-20000 UDP |
| FreeSWITCH | Often 16384-32768 UDP |
| 3CX | Commonly uses configured media ranges |

Always verify the actual configuration.

[Back to Contents](#contents)

---

<a id="nat-problems-in-voip"></a>

### 15. NAT Problems in VoIP

VoIP is sensitive to NAT because SIP and RTP may advertise IP addresses inside packet bodies.

Common symptom:

```text
Call connects, but no audio.
```

Possible causes:

- RTP ports blocked.
- Wrong public IP advertised.
- NAT rewriting SIP badly.
- Firewall allows SIP but blocks RTP.
- One-way routing.
- SIP ALG interfering.
- Wrong STUN/TURN configuration.

Simple rule:

> SIP makes the call ring. RTP makes the voice work.

[Back to Contents](#contents)

---

<a id="sip-alg"></a>

### 16. SIP ALG

Many routers have **SIP ALG**, which tries to help SIP through NAT. In practice, it often breaks SIP.

Common DevOps recommendation:

> Disable SIP ALG when using a real PBX or SIP provider, unless your provider specifically requires it.

Symptoms of bad SIP ALG:

- Phones unregistering.
- Calls dropping.
- One-way audio.
- Failed inbound calls.
- Weird SIP headers.
- Intermittent problems.

[Back to Contents](#contents)

---

<a id="codecs"></a>

### 17. Codecs

A codec compresses and encodes audio.

| Codec | Simple meaning |
| --- | --- |
| G.711 | High quality, more bandwidth |
| G.729 | Lower bandwidth, often licensed |
| Opus | Modern, flexible, strong for internet/WebRTC |
| GSM | Older low-bandwidth codec |
| G.722 | Wideband/HD voice |
| iLBC | Designed for packet loss tolerance |

Simple idea:

```text
Codec = how voice is packaged for the network
```

[Back to Contents](#contents)

---

<a id="voip-quality-metrics"></a>

### 18. VoIP Quality Metrics

| Metric | Good target |
| --- | --- |
| Latency | Ideally under 150 ms one-way |
| Jitter | Low and stable |
| Packet loss | As close to 0% as possible |
| MOS score | Higher is better |
| Bandwidth | Enough for codec plus overhead |

VoIP is real-time. Unlike file downloads, it cannot simply wait forever for missing packets.

[Back to Contents](#contents)

---

<a id="asterisk-vs-freepbx-vs-3cx-vs-freeswitch"></a>

### 19. Asterisk vs. FreePBX vs. 3CX vs. FreeSWITCH

| Platform | Best mental model |
| --- | --- |
| Asterisk | Build-your-own PBX engine |
| FreePBX | Asterisk with GUI |
| 3CX | Commercial business phone system |
| FreeSWITCH | Scalable telecom/media engine |
| FusionPBX | FreeSWITCH with GUI |
| Kamailio | SIP traffic router/proxy |
| OpenSIPS | SIP routing/proxy platform |

[Back to Contents](#contents)

---

<a id="devops-troubleshooting-flow-for-voip"></a>

### 20. DevOps Troubleshooting Flow for VoIP

Use this order:

```text
1. DNS: does the phone resolve the PBX/SIP server?
2. Registration: does the phone successfully REGISTER?
3. Authentication: are SIP credentials correct?
4. Signaling: does INVITE reach the destination?
5. Firewall/NAT: are SIP and RTP ports allowed?
6. Media: does RTP flow both ways?
7. Codec: do both sides support the same codec?
8. Provider: is the SIP trunk accepting calls?
9. Logs: what does the PBX say?
10. Packet capture: what do SIP and RTP packets show?
```

[Back to Contents](#contents)

---

<a id="useful-voip-commands-and-tools"></a>

### 21. Useful VoIP Commands and Tools

| Tool | Use |
| --- | --- |
| `sngrep` | View SIP call flows in terminal |
| `tcpdump` | Capture SIP/RTP packets |
| `Wireshark` | Analyze SIP and RTP visually |
| `ngrep` | Search SIP traffic |
| `asterisk -rvvv` | Asterisk CLI |
| `fs_cli` | FreeSWITCH CLI |
| `sip show peers` / `pjsip show endpoints` | Asterisk endpoint checks |
| `rtp set debug on` | Asterisk RTP debugging |
| `curl` | Test web/admin/API endpoints |
| `dig` | DNS checks |
| `nc` | Basic port checks |

[Back to Contents](#contents)

---

<a id="simple-voip-final-model"></a>

### 22. Simple VoIP Final Model

Remember:

```text
VoIP = phone calls over IP
SIP = starts and controls the call
RTP = carries the voice
PBX = private phone system
SIP trunk = connection to phone network
Asterisk = open-source PBX engine
FreePBX = GUI for Asterisk
3CX = commercial PBX/UC system
FreeSWITCH = scalable telecom engine
Kamailio/OpenSIPS = SIP routing layer
```

Most common VoIP troubleshooting idea:

> If the call rings but audio fails, check RTP, NAT, firewall, and codecs.

[Back to Contents](#contents)

---

<a id="references"></a>

### References

- [Asterisk](https://www.asterisk.org/)
- [Asterisk Downloads / FreePBX](https://www.asterisk.org/downloads/)
- [3CX](https://www.3cx.com/)
- [FreeSWITCH by SignalWire](https://signalwire.com/freeswitch)

[Back to Contents](#contents)
