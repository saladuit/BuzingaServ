echo "GET REQUEST on localhost at port $1"
echo ""

HTTP_LOG_DIR=../build/log/

curl localhost:$1 --http1.1 -s -o $HTTP_LOG_DIR/GET_${1}_body.log -D $HTTP_LOG_DIR/GET_${1}_header.log
