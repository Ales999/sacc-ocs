/*
 * $Id: stub_memaccount.c 17 2008-01-16 09:25:35Z slavik $
 */

/* Stub function for programs not implementing statMemoryAccounted */
#include "config.h"
#include "util.h"
int
statMemoryAccounted(void)
{
    return -1;
}
