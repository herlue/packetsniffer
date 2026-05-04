#ifndef PS_PKTINFO_H
#define PS_PKTINFO_H

#define PS_PKTINFO_BUFSIZE 1024

#include "ps_proto.h"
#include "pktinfo/ps_pktinfo_eth.h"
#include "pktinfo/ps_pktinfo_arp.h"
#include "pktinfo/ps_pktinfo_ip4.h"
#include "pktinfo/ps_pktinfo_tcp.h"
#include "pktinfo/ps_pktinfo_udp.h"

#include "proto/ps_proto_tcp.h" // FLAGS like FIN, SYN, RST, ACK

typedef struct
{
  ps_proto_l2_t proto_l2;
  union
  {
    ps_pktinfo_eth_t eth;
  } l2;

  ps_proto_l3_t proto_l3;
  union
  {
    ps_pktinfo_arp_t arp;
    ps_pktinfo_ip4_t ip4;
  } l3;

  ps_proto_l4_t proto_l4;
  union
  {
    ps_pktinfo_tcp_t tcp;
    ps_pktinfo_udp_t udp;
  } l4;
}
ps_pktinfo_t;

void ps_pktinfo_clear(ps_pktinfo_t*);
void ps_pktinfo_print(const ps_pktinfo_t*);

#endif
