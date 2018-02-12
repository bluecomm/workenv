#!/bin/sh

if [ $# -ne 1 ]; then
    echo -e "shard num needed"
    exit 1
fi

echo $1

if [ $1 -eq "0" ]; then
echo -e "get 0"
else
echo -e "get $1"
fi
