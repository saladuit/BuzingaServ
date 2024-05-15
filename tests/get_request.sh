#!/bin/bash

# Server configuration
SERVER_HOST="localhost"
SERVER_PORT="8080"

# Request file
REQUEST_FILE="tests/request/post_cgi.txt"

# Use netcat to send the contents of the request file to the server
nc "${SERVER_HOST}" "${SERVER_PORT}" <"${REQUEST_FILE}"
