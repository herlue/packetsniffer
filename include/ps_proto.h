#ifndef PS_PROTO_H
#define PS_PROTO_H

typedef enum
{
  PS_PROTO_L2_NONE,
  PS_PROTO_L2_ETH
}
ps_proto_l2_t;

typedef enum
{
  PS_PROTO_L3_NONE,
  PS_PROTO_L3_ARP,
  PS_PROTO_L3_IP4,
  PS_PROTO_L3_IP6
}
ps_proto_l3_t;

typedef enum
{
  PS_PROTO_L4_NONE,
  PS_PROTO_L4_ICMP,
  PS_PROTO_L4_TCP,
  PS_PROTO_L4_UDP
}
ps_proto_l4_t;


const char* ps_proto_l2_str(ps_proto_l2_t);
const char* ps_proto_l3_str(ps_proto_l3_t);
const char* ps_proto_l4_str(ps_proto_l4_t); 

#endif
