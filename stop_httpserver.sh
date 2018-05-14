#!/bin/sh

CGIHTTPServer_pid_file="./logs/stat/CGIHTTPServer.pid"

if [ -f $CGIHTTPServer_pid_file ]; then
    CGIHTTPServer_pid=`head -1 $CGIHTTPServer_pid_file | cut -f1`
    kill -9 $CGIHTTPServer_pid
fi
