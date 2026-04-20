#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
if [[ ! -x build/adr_atam_server ]]; then
  echo "Binary not found. Build first:"
  echo "  cmake -S . -B build"
  echo "  cmake --build build --config Release"
  exit 1
fi
exec ./build/adr_atam_server "$@"
