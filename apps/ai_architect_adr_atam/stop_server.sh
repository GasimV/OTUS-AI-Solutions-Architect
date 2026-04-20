#!/usr/bin/env bash
# Stops any running adr_atam_server process.
# Primary stop mechanism is Ctrl+C in the window running start_server.sh;
# this script is a fallback for backgrounded / orphaned processes.
set -uo pipefail

stopped=0

if command -v pkill >/dev/null 2>&1; then
  if pkill -f adr_atam_server; then
    stopped=1
  fi
fi

if [[ $stopped -eq 0 ]] && command -v taskkill >/dev/null 2>&1; then
  # Git Bash / MSYS2 on Windows: image name variant.
  if taskkill //IM adr_atam_server.exe //F >/dev/null 2>&1; then
    stopped=1
  fi
fi

if [[ $stopped -eq 1 ]]; then
  echo "Stopped adr_atam_server."
else
  echo "No running adr_atam_server process found."
fi
