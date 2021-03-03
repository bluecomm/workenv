#!/bin/sh
#需要python 2.7以上

PORT=8888
#PYTHON=/home/work/python2.7/bin/python2.7
#PYTHON=/home/work/Python-2.7.9/python

if [ ! -d ./logs ]; then
    mkdir ./logs
fi

if [ ! -d ./logs/stat ]; then
    mkdir ./logs/stat
fi

rm -rf ./log/stat/CGIHTTPServer.pid

# python 2
# nohup python -m CGIHTTPServer $PORT 2>>./logs/dashboard.log 1>&2 &
# python 3
nohup python -m http.server $PORT 2>>./logs/dashboard.log 1>&2 &

echo $! > ./logs/stat/CGIHTTPServer.pid
