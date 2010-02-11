/*
 * $Id: initgroups.h 17 2008-01-16 09:25:35Z slavik $
 */
#ifndef SQUID_INITGROUPS_H
#define SQUID_INITGROUPS_H

/* if you have configure you can use this */
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

extern int initgroups(const char *user, gid_t group);
#endif /* SQUID_INITGROPS_H */
