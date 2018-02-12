# create docker container and set port mapping

docker run -d -e "container=docker" --privileged=true -v /sys/fs/cgroup:/sys/fs/cgroup --p 8885:8885 -p 8886:8886 -p 8887:8887 -p 8888:8888 -p 8889:8889 --name centosx centos:latest /usr/sbin/init

docker exec -it centosx /bin/bash 

# start sshd service on docker/centos

chkconfig sshd on
    Note: Forwarding request to 'systemctl enable sshd.service'.

chkconfig sshd
    Note: Forwarding request to 'systemctl is-enabled sshd.service'.

ssh root@localhost -p 8885
