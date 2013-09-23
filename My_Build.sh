#!/bin/bash

# --enable-mysql-sh=localhost \

export SHELL='/bin/bash'
export CC='gcc'
export CXX='g++'
unset CXXFLAGS
export CXXFLAGS='-O2 -W -Wall -Weffc++ -DNDEBUG -DDEBIAN  -march=prescott -funroll-loops -mfpmath=sse'

./configure --enable-mysql-root=root --enable-mysql-rp=ChangeRootPwd \
 --enable-mysql-db=sacc --enable-mysql-user=sacc --enable-mysql-passwd=ChangeSaccPwd \
 --with-smb-domain=KRBD --with-samba-prefix=/usr \
 --with-squid-orig-conf=/etc/squid3/squid.orig.conf \
 --with-squid-conf=/etc/squid3/squid.conf \
 --with-squid-pid=/var/run/squid3.pid \
 --with-squid-log=/var/log/squid3/access.log \
 --with-www=/var/www \
 --prefix=/usr/local

make clean
make
# Warning !!! If Install - DB removed and Re-Created !!!
# Внимание, Если выполнить 'make install' то база будет УДАЛЕНА и создана с нуля !!!
#sudo make install