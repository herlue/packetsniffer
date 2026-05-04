#ifndef PS_PKTINFO_IP4_H
#define PS_PKTINFO_IP4_H

#define PS_IP4_ADDR_STRLEN 16

typedef struct ps_pktinfo_ip4
{
  char src_addr[PS_IP4_ADDR_STRLEN];
  char dst_addr[PS_IP4_ADDR_STRLEN];
}
ps_pktinfo_ip4_t;

#endif
