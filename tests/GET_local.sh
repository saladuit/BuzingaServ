#!/bin/bash

if [[ -z $1 ]]; then
	SERVER_PORT="8080"
else
	SERVER_PORT=$1
fi

HTTP_VERSION="HTTP/1.1"

SERVER_HOST="localhost"
SERVER_LOG_DIR=../build/log/

HTTP_REQUEST="./request/get.txt"

echo "GET REQUEST on localhost at port $SERVER_PORT"
echo ""

echo "Script ran:"
echo curl $SERVER_HOST:$SERVER_PORT --$HTTP_VERSION -s -o $SERVER_LOG_DIR/GET_${SERVER_HOST}:${SERVER_PORT}_body.log -D $SERVER_LOG_DIR/GET_${SERVER_HOST}:${SERVER_PORT}_header.log
echo ""

curl $SERVER_HOST:$SERVER_PORT --$HTTP_VERSION -s -o $SERVER_LOG_DIR/GET_${SERVER_HOST}:${SERVER_PORT}_body.log -D $SERVER_LOG_DIR/GET_${SERVER_HOST}:${SERVER_PORT}_header.log
