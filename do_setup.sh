###############################################################
#creat docker container:
#       docker run -d -e "container=docker" --privileged=true -v /sys/fs/cgroup:/sys/fs/cgroup -p 8885:8885 -p 8886:8886 -p 8887:8887 -p 8888:8888 -p 8889:8889 --name centosx centos:latest /usr/sbin/init
# 
# start container:
#       docker start centosx
#
# login container:
#       docker exec -it centosx /bin/bash
# 
###############################################################
# install sys tools
yum install systemd initscripts passwd vim crontabs

# install ssh
yun install openssh openssh-server libssh libssh-devel libssh-config

# create user:
useradd blue
passwd blue

# gen ssh key
ssh-keygen -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key

# set ssh port and address
# Port 8885
# ListenAddress 0.0.0.0
vim /etc/ssh/sshd_config

# start ssh
service sshd start

# set ssh start on init
chkconfig sshd on
chkconfig sshd

# login container reomotely:
ssh -p 8885 blue@localhost 

# install other tools
yum install \
git \
net-tools \
make \
gcc gcc-c++ \
boost boost-devel
#protobuf protobuf-devel protobuf-c protobuf-c-devel protobuf-compiler protobuf-c-compiler

# look for sys time
timedatectl
timedatectl list-timezones
#timedatectl set-local-rtc 1 # set localtimezone
timedatectl set-timezone Asia/Shanghai # set local timezone

###############################################################
# backup files
# phisycs host rontab:
# 0 20 1 * * mkdir -p ~/work/Software/tmp; mkdir -p ~/work/Software/docker_bak; /usr/local/bin/docker cp centosx:/home/blue/docker_bak ~/work/Software/tmp/; mv ~/work/Software/tmp/docker_bak/docker* ~/work/Software/docker_bak/; rm -rf ~/work/Software/tmp/docker_bak/
# container crontab:
# * 0 1 * * cd; sh do_backup.sh

