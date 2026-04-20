#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

if [[ -d /c/msys64/ucrt64/bin ]]; then
  export PATH="/c/msys64/ucrt64/bin:/c/msys64/usr/bin:$PATH"
elif [[ -d /ucrt64/bin ]]; then
  export PATH="/ucrt64/bin:/usr/bin:$PATH"
fi

if [[ ! -x build/adr_atam_server ]]; then
  echo "Binary not found. Build first:"
  echo "  cmake -S . -B build"
  echo "  cmake --build build --config Release"
  exit 1
fi
exec ./build/adr_atam_server "$@"
