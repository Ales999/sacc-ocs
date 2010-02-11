#include "info.h"

#if HAVE_CRYPT_H
#include <crypt.h>
#endif
#if HAVE_LIBC_H
#include <libc.h>
#endif
#if HAVE_PWD_H
#include <pwd.h>
#endif
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "language.h"
