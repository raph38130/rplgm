#!/bin/bash
#
# mycgi.sh
# CGI sample in bash
#
# RP Thu Oct 25 12:11:41 CEST 2001
#	- initial revision
#
echo "content-type: text/html"
echo
echo "<HTML>"
#echo "request method = "
if test $REQUEST_METHOD = "POST"; then
	read query
	echo $query
fi
if test $REQUEST_METHOD = "GET"; then
	echo $QUERY_STRING
fi
echo "</HTML>"
