#ifndef PS_PKTINFO_ETH_H
#define PS_PKTINFO_ETH_H

#define PS_MAC_ADDR_STRLEN 18

typedef struct ps_pktinfo_eth
{
  char src_addr[PS_MAC_ADDR_STRLEN];
  char dst_addr[PS_MAC_ADDR_STRLEN];
}
ps_pktinfo_eth_t;

#endif
