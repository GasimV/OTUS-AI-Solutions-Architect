# syntax=docker/dockerfile:1.6

# ----------------------------------------------------------------------------
# Build stage: compile adr_atam_server and run the test suite.
# ----------------------------------------------------------------------------
FROM ubuntu:24.04 AS build

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY CMakeLists.txt ./
COPY server ./server
COPY tests ./tests

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build -j"$(nproc)"

RUN ctest --test-dir build --output-on-failure

# ----------------------------------------------------------------------------
# Runtime stage: minimal image with just the binary + static assets.
# ----------------------------------------------------------------------------
FROM ubuntu:24.04 AS runtime

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libstdc++6 \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /src/build/adr_atam_server ./adr_atam_server
COPY web ./web
COPY templates ./templates
COPY data ./data
RUN mkdir -p ./exports

# Bind to all interfaces inside the container; the host maps a port with -p.
ENV ADRA_HOST=0.0.0.0 \
    ADRA_PORT=8090

EXPOSE 8090

VOLUME ["/app/data", "/app/exports"]

ENTRYPOINT ["/app/adr_atam_server"]
