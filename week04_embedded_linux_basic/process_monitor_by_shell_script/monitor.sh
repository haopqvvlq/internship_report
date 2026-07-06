#!/bin/bash

PROCESS_NAME="bash"

PID=$(pgrep -x "$PROCESS_NAME" | head -n 1)

if [ -n "$PID" ]; then
    echo "Process '$PROCESS_NAME' đang chạy."
    echo "PID: $PID"

    STATE=$(ps -o stat= -p "$PID")
    echo "State: $STATE"
else
    echo "Process '$PROCESS_NAME' không chạy."
fi
