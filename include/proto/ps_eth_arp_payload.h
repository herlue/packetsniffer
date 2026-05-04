#ifndef PS_ARP_H
#define PS_ARP_H

#include "../ps_types.h"

typedef struct ps_eth_arp_payload
{
  uint8_t sha[6];
  uint8_t spa[4];
  uint8_t tha[6];
  uint8_t tpa[4];
}
ps_eth_arp_payload_t;

#endif
