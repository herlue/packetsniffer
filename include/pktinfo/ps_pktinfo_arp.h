#ifndef PS_PKTINFO_ARP_H
#define PS_PKTINFO_ARP_H

#include "../proto/ps_eth_arp_payload.h"

#define PS_ARP_REQ 1
#define PS_ARP_REP 2

#define PS_MAC_ADDR_STRLEN 18
#define PS_IP4_ADDR_STRLEN 16

typedef struct ps_pktinfo_arp
{
  uint16_t opcode;

  char s_haddr[PS_MAC_ADDR_STRLEN];
  char s_paddr[PS_IP4_ADDR_STRLEN];
  char t_haddr[PS_MAC_ADDR_STRLEN];
  char t_paddr[PS_MAC_ADDR_STRLEN];
}
ps_pktinfo_arp_t;

#endif
