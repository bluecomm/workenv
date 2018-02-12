#!/bin/sh

s="a"
for((i=0;i<100;i++))
do
    s=$s$i
    j=$[$i+1]
    echo -e "{$i, $j, \"$s\"},"
done
