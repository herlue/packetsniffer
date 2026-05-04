#ifndef PS_PKTINFO_UDP_H
#define PS_PKTINFO_UDP_H

#include "../ps_types.h"

typedef struct ps_pktinfo_udp
{
  port_t src_port;
  port_t dst_port;
}
ps_pktinfo_udp_t;

#endif
