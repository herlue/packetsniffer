#ifndef PS_PKTHANDLER_FN_H
#define PS_PKTHANDLER_FN_H

#include "../ps_context.h"
#include "../ps_pktinfo.h"
#include "../ps_types.h"

typedef int (*ps_pkthandler_fn)(const ps_context_t*, ps_pktinfo_t*, uint32_t, const uchar*); 

#endif
