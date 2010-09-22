#!/bin/bash

# --enable-mysql-sh=localhost \



# export CPPFLAG_ADD='-DFREE_HTTPS'
export CC='gcc'
export CXX='g++'
unset CXXFLAGS
# export CXXFLAGS='-DDEBUG -DDEBIAN -g -O0 -Wall'
export CXXFLAGS='-O0 -g -W -Weffc++ -DDEBUG -DDEBIAN  -march=prescott -funroll-loops -mfpmath=sse'
# export CXXFLAGS='-g -Wall -DDEBIAN  -march=prescott -funroll-loops -mfpmath=sse'
export SHELL='/bin/bash'

# -mtune=native'
#  --enable-mysql-host=localhost --enable-mysql-sh=localhost \

./configure --enable-mysql-root=root --enable-mysql-rp=ChangeRootPWD \
 --enable-mysql-db=sacc --enable-mysql-user=sacc --enable-mysql-passwd=saccpwd \
 --with-smb-domain=OTHERDOM --with-samba-prefix=/usr \
 --with-squid-orig-conf=/etc/squid3/squid.orig.conf \
 --with-squid-conf=/etc/squid3/squid.conf \
 --with-squid-pid=/var/run/squid3.pid \
 --with-squid-log=/var/log/squid3/access.log \
 --with-www=/var/www \
 --prefix=/usr/local

#  --with-mysql-prefix=/usr/lib \