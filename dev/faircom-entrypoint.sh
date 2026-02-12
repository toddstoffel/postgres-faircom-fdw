#!/usr/bin/env bash
set -euo pipefail

FAIRCOM_BIN="/opt/faircom/server/faircom"
INIT_SCRIPT="/opt/faircom/init_faircom.py"
INIT_MARKER="/opt/faircom/.init_done"

# Start FairCom server in background
"$FAIRCOM_BIN" &
FAIRCOM_PID=$!

# Wait for FairCom to become ready
READY_FILE_PRIMARY="/opt/faircom/data/CTSTATUS.FCS"
READY_FILE_FALLBACK="/opt/faircom/server/data/CTSTATUS.FCS"

max_wait=60
elapsed=0

while true; do
  if [[ -f "$READY_FILE_PRIMARY" ]] || [[ -f "$READY_FILE_FALLBACK" ]]; then
    break
  fi
  if ! kill -0 "$FAIRCOM_PID" 2>/dev/null; then
    echo "FairCom server exited before ready" >&2
    exit 1
  fi
  if [[ "$elapsed" -ge "$max_wait" ]]; then
    echo "Timeout waiting for FairCom server to become ready" >&2
    exit 1
  fi
  sleep 1
  elapsed=$((elapsed + 1))
done

# Run initialization once
if [[ ! -f "$INIT_MARKER" ]]; then
  echo "Running FairCom initialization..."
  if python3 "$INIT_SCRIPT"; then
    touch "$INIT_MARKER"
    echo "FairCom initialization complete"
  else
    echo "FairCom initialization failed" >&2
    exit 1
  fi
else
  echo "FairCom initialization already completed"
fi

# Bring FairCom server process to foreground
wait "$FAIRCOM_PID"
