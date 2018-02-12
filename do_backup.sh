#!/bin/sh

backup_filename_prefix="docker_bak"
username=` who | awk '{print $1}' `
hostname=` hostname `
time_readable=` date "+%Y%m%d%H%M%S" `
time_seconds=` date "+%s" `

#backup_filename=${backup_filename_prefix}_${hostname}_${time_readable}_${time_seconds}.tar.gz
backup_filename=docker_bak/${backup_filename_prefix}_${hostname}_${time_readable}.tar.gz
echo -e "backup file: "${backup_filename}
#exit 0

cp /etc/bashrc ./etc_bashrc
tar -zcvf ${backup_filename} \
    do_backup.sh do_setup.sh \
    dev/ memo/ notes/ \
    readme.md \
    .vim/ .vimrc \
    .bash_profile .bashrc \
    etc_bashrc

rm ./etc_bashrc

echo
echo -e "backup file: "${backup_filename}
echo -e "Done"
echo 
