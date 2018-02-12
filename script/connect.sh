#!bin/sh

userid=0
if [ $# -ge 1 ];then
userid=$1
echo -e "userid: ${userid}"
fi

shard=$((${userid}%2))
#shard=`expr ${userid} % 2`
echo -e "shard: $shard"

