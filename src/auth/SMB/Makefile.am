#
#  Makefile for the Squid Object Cache server
#
#  $Id: Makefile.am 4 2006-03-26 20:20:41Z slavik $
#
#  Uncomment and customize the following to suit your needs:
#

# SAMBAPREFIX must point to the directory where Samba has been installed.
# By default, Samba is installed in /usr/local/samba. If you changed this
# by using the --prefix option when configuring Samba, you need to change
# SAMBAPREFIX accordingly.

sacc_statdir=${prefix}/sacc/bin
SMB_AUTH_HELPER	= smb_auth.sh
SAMBAPREFIX=${smb_prefix}
###/usr/local/samba
SMB_AUTH_HELPER_PATH = ${sacc_statdir}/$(SMB_AUTH_HELPER)

sacc_stat_SCRIPTS	= $(SMB_AUTH_HELPER)
sacc_stat_PROGRAMS = smb_auth
smb_auth_CFLAGS = -DSAMBAPREFIX=\"$(SAMBAPREFIX)\" -DHELPERSCRIPT=\"$(SMB_AUTH_HELPER_PATH)\"
EXTRA_DIST = smb_auth.sh

#LDADD	= -lmiscutil $(all_libraries) -L../lib
LDADD	= $(all_libraries) -L../lib
INCLUDES= $(all_includes) -I../include
