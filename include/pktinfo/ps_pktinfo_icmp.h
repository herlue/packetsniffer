#ifndef PS_PKTINFO_ICMP_H
#define PS_PKTINFO_ICMP_H

#include "../ps_types.h"

typedef struct ps_pktinfo_icmp
{
  uint8_t type;
  uint8_t code;
}
ps_pktinfo_icmp_t;

#endif
