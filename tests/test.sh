#!/bin/bash

# usage() { # Function: Print a help message.
# 	echo "Usage: $0 [ -n NAME ] [ -t TIMES ]" 1>&2
# }
#
# exit_abnormal() { # Function: Exit with error.
# 	usage
# 	exit 1
# }
#
# while getopt "p:d:" OPTIONS; do
#
# 	case "${OPTIONS}" in
# 	d)
# 		HTTP_DEBUG=1
# 		;;
# 	p)
# 		HTTP_PORT=${OPTARG}
# 		;;
# 	:) # If expected argument omitted:
# 		echo "Error: -${OPTARG} requires an argument."
# 		exit_abnormal # Exit abnormally.
# 		;;
# 	*)
# 		exit_abnormal # Exit abnormally.
# 		;;
# 	esac
# done

for i in $@; do
	if [[ "$i" == "-nl" ]]; then
		HTTP_NO_LOG=1
	fi
	if [[ "$i" == "-d" ]]; then
		HTTP_DEBUG=1
	fi
	if [[ "$i" == "-p" ]]; then
		HTTP_PORT=$j
	fi

done

HTTP_LOG_DIR=./log

if [[ $HTTP_DEBUG == 1 ]]; then
	echo ""
	echo HTTP_DEBUG_MODE is on
	echo ""
	echo log directory..: $HTTP_LOG_DIR
	echo URL............: $1
	echo PORT...........: $HTTP_PORT

	echo ""
	if [[ $HTTP_NO_LOG == 1 ]]; then
		echo HTTP_NO_LOG_MODE is on
	fi

	echo ""
fi

if [[ $1 == "GET" ]]; then
	echo "$1 REQUEST"
	echo ""
	curl -X $1 $2 --http1.1 -s -o $HTTP_LOG_DIR/${1}_${2#www.}_body.log -D $HTTP_LOG_DIR/${1}_${2#www.}_header.log

elif [[ $1 == "POST" ]]; then
	echo "$1 REQUEST"
	echo ""
	curl -X $1 $2 -s -o $HTTP_LOG_DIR/${1}_${2#www.}_body.log -D $HTTP_LOG_DIR/${1}_${2#www.}_header.log

elif [[ $1 == "DEL" ]]; then
	echo "$1 REQUEST"
	echo ""

elif [[ $1 == "UNSPEC" ]]; then
	echo "$1 REQUEST"
	echo ""

else
	echo "Give a HTTP Methods [GET, POST, DEL, UNSPEC]"
fi

#https://reqbin.com/req/c-sma2qrvp/curl-post-form-example
