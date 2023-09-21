#!/bin/bash

# Server configuration
SERVER_HOST="localhost"
SERVER_PORT="80"

# Request file
REQUEST_FILE="tests/request/get.txt"

# Use netcat to send the contents of the request file to the server
nc "${SERVER_HOST}" "${SERVER_PORT}" < "${REQUEST_FILE}"
