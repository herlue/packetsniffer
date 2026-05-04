#ifndef PS_PKTINFO_TCP_H
#define PS_PKTINFO_TCP_H

#include "../ps_types.h"

typedef struct ps_pktinfo_tcp
{
  port_t src_port;
  port_t dst_port;
  
  uint8_t flags;
}
ps_pktinfo_tcp_t;

#endif
