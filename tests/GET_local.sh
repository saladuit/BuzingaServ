#!/bin/bash

if [[ -z $1 ]]; then
	SERVER_PORT="9696"
else
	SERVER_PORT=$1
fi

HTTP_VERSION="http1.1"

SERVER_HOST="localhost"
SERVER_LOG_DIR=../build/log/

echo "GET REQUEST on localhost at port $SERVER_PORT"
echo ""

curl $SERVER_HOST:$SERVER_PORT --$HTTP_VERSION -s -o $SERVER_LOG_DIR/GET_${1}_body.log -D $SERVER_LOG_DIR/GET_${1}_header.log
