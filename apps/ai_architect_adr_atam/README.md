# AI-Architect · ADR & ATAM

A standalone, offline C++ desktop tool that helps an AI Architect author and
reuse **Architecture Decision Records (ADR)** and **Architecture Tradeoff
Analysis Method (ATAM)** artifacts. The app embeds a local HTTP server and a
browser-based UI, and can optionally call either a locally installed **Ollama**
LLM (default model: `gemma4:e2b`) or the **Gemini API** (default model:
`gemma-4-26b-a4b-it`) to assist with drafting, summarization, and tradeoff
inference.

The app is fully usable with the LLM disabled or absent. Ollama mode stays
local; Gemini mode calls Google's hosted API and requires your own API key.

## Contents

1. [Project layout](#project-layout)
2. [Prerequisites](#prerequisites)
3. [Build](#build)
   - [Generic (any platform)](#generic-any-platform)
   - [Windows · MSYS2 UCRT64 (verified)](#windows--msys2-ucrt64-verified)
   - [Windows · MSVC](#windows--msvc)
4. [Run](#run)
5. [Stopping the server](#stopping-the-server)
6. [Docker (Linux container)](#docker-linux-container)
7. [Command-line options](#command-line-options)
8. [Runtime AI config](#runtime-ai-config)
9. [Tests](#tests)
10. [Data on disk](#data-on-disk)
11. [REST API (short)](#rest-api-short)
12. [Extending](#extending)
13. [Troubleshooting](#troubleshooting)

---

## Project layout

```
apps/ai_architect_adr_atam/
├── CMakeLists.txt
├── README.md              ← this file
├── DESIGN.md              ← architecture / design
├── DEMO.md                ← short walkthrough
├── start_server.bat       ← Windows launcher (foreground)
├── start_server.sh        ← Unix / Git Bash launcher (foreground)
├── stop_server.bat        ← Windows stop helper
├── stop_server.sh         ← Unix / Git Bash stop helper
├── Dockerfile             ← Linux container build (Ubuntu 24.04)
├── .dockerignore
├── server/                ← C++ sources
│   ├── main.cpp
│   ├── third_party/       ← httplib.h, json.hpp (single-header deps)
│   ├── util/              ← uuid, time, fs helpers
│   ├── domain/            ← ADR / ATAM domain models
│   ├── persistence/       ← FileStore + repositories
│   ├── services/          ← ADR / ATAM / export / templates / reuse
│   ├── ai/                ← LlmClient interface, Ollama/Gemini clients, AiService
│   └── http/              ← REST API routes
├── web/                   ← HTML / CSS / JS frontend
├── templates/             ← ADR & ATAM JSON templates (reusable)
├── data/                  ← Local artifacts (ADRs + ATAM sessions)
├── exports/               ← Exported Markdown/JSON lands here
└── tests/                 ← C++ test suite (no external deps)
```

[↑ Back to contents](#contents)

---

## Prerequisites

- **C++ compiler with C++17**
  - MSVC 19.3+ (Visual Studio 2022), or
  - GCC 10+ / MinGW-w64 UCRT64 g++ 13+ (tested), or
  - Clang 11+.
- **CMake 3.15+**.
- **Windows 10 or later** on Windows (the embedded `cpp-httplib` requires
  `_WIN32_WINNT=0x0A00`; the CMake build sets this automatically).
- (Optional) **Ollama** running locally with the `gemma4:e2b` model pulled:

  ```
  ollama serve
  ollama pull gemma4:e2b
  ```

  The app still runs fully without this — AI buttons are inactive and the
  topbar shows "AI: offline".
- (Optional) **OpenSSL development libraries** if you want the native build to
  call Gemini API over HTTPS. The CMake build still succeeds without OpenSSL,
  but Gemini status will report that HTTPS support is unavailable.
  On MSYS2 UCRT64, install the matching packages:

  ```bash
  pacman -Syu
  pacman -S mingw-w64-ucrt-x86_64-openssl mingw-w64-ucrt-x86_64-ca-certificates
  ```

- (Optional) **Gemini API key** in `.env` if using `ADRA_LLM_PROVIDER=gemini`.
  Start from `.env.example`; `.env` is ignored by git.

[↑ Back to contents](#contents)

---

## Build

From `apps/ai_architect_adr_atam/`.

### Generic (any platform)

```bash
cmake -S . -B build
cmake --build build --config Release
```

Outputs:
- `build/adr_atam_server` (or `.exe` on Windows)
- `build/tests/adr_atam_tests` (or `.exe` on Windows)
- `build/libadr_atam_core.a` (static core library)

### Windows · MSYS2 UCRT64 (verified)

This is the combination this project was verified on (g++ 13.2, UCRT64).

From a **UCRT64 shell** (or any shell with the UCRT64 toolchain on PATH):

```bash
cmake -S . -B build -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe \
  -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe \
  -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64 \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Explicitly pinning `CMAKE_CXX_COMPILER` and `CMAKE_MAKE_PROGRAM` avoids the
common failure mode where CMake picks up an LLVM clang on PATH and then fails
to link against MinGW runtime (e.g.
`lld-link: error: could not open 'kernel32.lib'`).

The resulting executable depends on UCRT64 runtime DLLs
(`libstdc++-6.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`) and, when
Gemini/OpenSSL support is enabled, OpenSSL DLLs (`libssl-3-x64.dll`,
`libcrypto-3-x64.dll`). If you run the binary or tests from a shell that does
**not** have `C:\msys64\ucrt64\bin` on PATH, you will see error `127` or a
"DLL not found" dialog. Put UCRT64 first on PATH:

```bash
export PATH="/c/msys64/ucrt64/bin:/usr/bin:$PATH"   # Git Bash / MSYS2
# or, in PowerShell:
#   $env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
# or, in cmd:
#   set PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%
```

### Windows · MSVC

From a **Developer PowerShell / x64 Native Tools Command Prompt**:

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

MSVC builds produce statically linked executables — no runtime DLL PATH setup
needed.

[↑ Back to contents](#contents)

---

## Run

```bash
./build/adr_atam_server
# or on Windows
build\adr_atam_server.exe
```

Then open http://127.0.0.1:8090/ in your browser.

Launchers (must be run from `apps/ai_architect_adr_atam/`, foreground):
- Windows: `start_server.bat`
- Unix / Git Bash: `./start_server.sh`

On launch the server prints a banner showing the resolved paths, the selected
AI provider, configured provider targets, and the listen address, ending with
`Press Ctrl+C to stop.`.

[↑ Back to contents](#contents)

---

## Stopping the server

The server runs in the **foreground**; the primary way to stop it is:

- **Ctrl+C** in the terminal window running `start_server.sh` / `start_server.bat`.

`main.cpp` installs `SIGINT` and `SIGTERM` handlers that call
`httplib::Server::stop()` for a clean shutdown.

If the server was backgrounded, the window was closed without Ctrl+C, or the
process was orphaned and is still holding port 8090, use the fallback stop
helpers:

- Windows: `stop_server.bat` → runs `taskkill /IM adr_atam_server.exe /F`.
- Unix / Git Bash: `./stop_server.sh` → tries `pkill -f adr_atam_server`, then
  falls back to `taskkill` (for Git Bash on Windows).

To confirm the port is free afterwards:

```bash
# Unix
lsof -i :8090
# Windows (cmd or PowerShell)
netstat -ano | findstr :8090
```

[↑ Back to contents](#contents)

---

## Docker (Linux container)

A multi-stage `Dockerfile` is provided for reproducible Linux builds. It uses
`ubuntu:24.04` for both build and runtime stages, compiles with the default
`build-essential` toolchain (g++ 13), and runs the full test suite inside the
build stage — so the image only tags successfully if all 19 tests pass.

### Build the image

From `apps/ai_architect_adr_atam/`:

```bash
docker build -t adr-atam:latest .
```

Verified outcome (Docker Desktop linux/amd64 engine):
- 100% tests passed
- Final image is based on `ubuntu:24.04` with `libstdc++6`, OpenSSL runtime,
  the binary, and web/templates/data assets.

### Run the container

```bash
docker run --rm -p 8090:8090 --name adr-atam adr-atam:latest
# then open http://127.0.0.1:8090/ on the host
```

The image sets `ADRA_HOST=0.0.0.0` so the server binds on all container
interfaces; the host port mapping is yours to choose via `-p HOST:8090`.

Persist ADRs, ATAM sessions, and exports across container restarts by
bind-mounting volumes:

```bash
docker run --rm -p 8090:8090 \
  -v "$(pwd)/data:/app/data" \
  -v "$(pwd)/exports:/app/exports" \
  --name adr-atam adr-atam:latest
```

Disable AI at startup:

```bash
docker run --rm -p 8090:8090 adr-atam:latest --no-ai
```

Use Gemini API from Docker:

```bash
docker run --rm -p 8090:8090 \
  -e ADRA_LLM_PROVIDER=gemini \
  -e GEMINI_API_KEY="$GEMINI_API_KEY" \
  -e GEMINI_MODEL=gemma-4-26b-a4b-it \
  adr-atam:latest
```

### Talking to Ollama from inside the container

The default `--ollama-host 127.0.0.1` points at the *container*, not the host,
so Ollama running on the host will be unreachable from a default network.
Options:

- **Docker Desktop (Windows/macOS):** use the magic hostname.

  ```bash
  docker run --rm -p 8090:8090 \
    -e OLLAMA_HOST=host.docker.internal adr-atam:latest
  ```

- **Linux host with Ollama on the host:** share the host network.

  ```bash
  docker run --rm --network host adr-atam:latest
  ```

- **Sibling Ollama container:** put both in the same user-defined network and
  point `OLLAMA_HOST` at the Ollama service name.

### Stop the container

```bash
docker stop adr-atam
```

`--rm` (above) automatically removes it after stop. `stop_server.sh` /
`stop_server.bat` are *not* for container use — they kill host processes.

[↑ Back to contents](#contents)

---

## Command-line options

| Flag | Default | Purpose |
|---|---|---|
| `--host HOST` | `127.0.0.1` | Listen address |
| `--port N` | `8090` | Listen port |
| `--root DIR` | auto-detected | Base dir (must contain `web/`, `templates/`, `data/`) |
| `--ai-provider NAME` | `ollama` | LLM provider: `ollama` or `gemini` |
| `--ollama-host HOST` | `127.0.0.1` | Ollama HTTP host |
| `--ollama-port N` | `11434` | Ollama HTTP port |
| `--ollama-model NAME` | `gemma4:e2b` | Default model |
| `--gemini-api-host HOST` | `generativelanguage.googleapis.com` | Gemini API HTTPS host |
| `--gemini-api-port N` | `443` | Gemini API HTTPS port |
| `--gemini-model NAME` | `gemma-4-26b-a4b-it` | Gemini API model |
| `--no-ai` | — | Disable AI integration entirely |

Environment variables `ADRA_HOST`, `ADRA_PORT`, `ADRA_LLM_PROVIDER`,
`OLLAMA_HOST`, `OLLAMA_PORT`, `OLLAMA_MODEL`, `GEMINI_API_KEY`,
`GEMINI_MODEL`, `GEMINI_API_HOST`, `GEMINI_API_PORT`, and `ADRA_AI_ENABLED`
are honored as fallbacks. The server also loads `.env` from the app directory
at startup; existing process environment variables take precedence.

[↑ Back to contents](#contents)

---

## Runtime AI config

The AI badge in the topbar opens a config dialog
(provider/Ollama host/Ollama port/model/enabled). It also posts to
`POST /api/ai/config`, which updates the in-memory `LlmConfig` without
restarting.

For Gemini API, create `.env` from `.env.example` and set:

```bash
ADRA_LLM_PROVIDER=gemini
GEMINI_API_KEY=your-api-key
GEMINI_MODEL=gemma-4-26b-a4b-it
```

Do not commit `.env`; it is ignored by the app-level `.gitignore`.

[↑ Back to contents](#contents)

---

## Tests

```bash
cmake --build build --target adr_atam_tests
./build/tests/adr_atam_tests
# or
ctest --test-dir build --output-on-failure
```

The suite covers util, domain round-trips, persistence CRUD, and service
behavior including template apply, exports, and reuse similarity. No network
or LLM provider required. Current status: **19 tests, all passing** on MSYS2
UCRT64 g++ 13.2.

> On MSYS2 UCRT64, remember to have `/c/msys64/ucrt64/bin` (or the cmd
> equivalent) on PATH when invoking the test binary from outside a UCRT64
> shell, otherwise the loader exits with code `127`.

[↑ Back to contents](#contents)

---

## Data on disk

- `data/adr/<uuid>.json` — one ADR per file.
- `data/atam/<uuid>.json` — one ATAM session per file.
- `templates/adr/*.json` and `templates/atam/*.json` — seed content.
- `exports/` — Markdown/JSON written when the user clicks an export button
  with `save=true` (the UI triggers this on export-to-file).

Files are human-readable and safe to inspect, edit, or commit. Delete a file
to remove the artifact.

[↑ Back to contents](#contents)

---

## REST API (short)

See [DESIGN.md](DESIGN.md) for the full reference. Highlights:

- `GET  /api/status`
- `GET  /api/ai/status` · `POST /api/ai/config`
- `GET/POST /api/adr` · `GET/PUT/DELETE /api/adr/:id` · `GET /api/adr/:id/export?format=md|json`
- `GET/POST /api/atam` · `GET/PUT/DELETE /api/atam/:id` · `GET /api/atam/:id/export?format=md|json`
- `POST /api/reuse/adrs` · `POST /api/reuse/scenarios`
- `POST /api/ai/draft-adr` · `POST /api/ai/improve-section` · `POST /api/ai/summarize-context`
- `POST /api/ai/suggest-alternatives|consequences|risks`
- `POST /api/ai/suggest-scenarios`
- `POST /api/ai/atam/:id/infer-tradeoffs|summary|adr-candidates`

[↑ Back to contents](#contents)

---

## Extending

- Swap the LLM provider by implementing the `adra::ai::LlmClient` interface.
  `ProviderLlmClient` currently routes to `OllamaClient` or `GeminiClient`
  without touching the routes or services.
- Add domain fields by extending `domain/adr.h` + `domain/atam.h` and their
  `to_json/from_json` — the file store is schema-flexible.
- Add a template by dropping a `*.json` file into `templates/adr/` or
  `templates/atam/` — the UI will list it automatically.

[↑ Back to contents](#contents)

---

## Troubleshooting

- **`lld-link: error: could not open 'kernel32.lib'` during CMake configure on Windows.**
  CMake picked up LLVM clang instead of the MinGW/UCRT64 g++. Pass
  `-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe` and
  `-DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe` (see
  [Windows · MSYS2 UCRT64](#windows--msys2-ucrt64-verified)).

- **`#error "cpp-httplib doesn't support Windows 8 or lower"`.**
  The build requires `_WIN32_WINNT>=0x0A00`. The project's `CMakeLists.txt`
  already sets this for `WIN32`; if you see this error, verify you are
  configuring from the project's own `CMakeLists.txt` (not an outer one).

- **Test binary exits with code `127` on Windows.**
  UCRT64 runtime DLLs are not on PATH. Add `C:\msys64\ucrt64\bin` to PATH
  before invoking the binary.

- **`Failed to bind 127.0.0.1:8090. Port may be in use.`**
  Another instance is still running. Use `stop_server.bat` / `./stop_server.sh`
  or check with `netstat -ano | findstr :8090` (Windows) /
  `lsof -i :8090` (Unix).

- **AI badge stays "offline" even though Ollama is running.**
  Confirm Ollama is reachable at `127.0.0.1:11434` and the configured model
  (default `gemma4:e2b`) is pulled. You can change host/port/model at runtime
  via the topbar AI config dialog without restarting.

- **Gemini says `GEMINI_API_KEY is not set`.**
  Copy `.env.example` to `.env`, set `ADRA_LLM_PROVIDER=gemini`, and add your
  real `GEMINI_API_KEY`. Restart the server after changing `.env`.

- **Gemini says OpenSSL support is unavailable.**
  Install OpenSSL development libraries and re-run CMake so
  `CPPHTTPLIB_OPENSSL_SUPPORT` can be enabled. On MSYS2 UCRT64:

  ```bash
  pacman -Syu
  pacman -S mingw-w64-ucrt-x86_64-openssl mingw-w64-ucrt-x86_64-ca-certificates
  cmake -S . -B build -G "MinGW Makefiles" \
    -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe \
    -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe \
    -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64 \
    -DCMAKE_BUILD_TYPE=Release
  cmake --build build -j
  ```

  If you run from PowerShell after an MSYS2 update, put the updated UCRT64
  runtime first:

  ```powershell
  $env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
  ```

  Docker builds install this dependency automatically.

- **AI badge is "offline" only when running in Docker.**
  Inside a container, `127.0.0.1` is the container itself, not the host. Use
  `-e OLLAMA_HOST=host.docker.internal` on Docker Desktop (Windows/macOS),
  or `--network host` on Linux. See
  [Docker · Talking to Ollama from inside the container](#talking-to-ollama-from-inside-the-container).

[↑ Back to contents](#contents)
