#include "../include/ps_proto.h"

const char* ps_proto_l2_str(ps_proto_l2_t proto)
{
  switch (proto)
  {
    case PS_PROTO_L2_NONE: return "-";
    case PS_PROTO_L2_ETH : return "Ethernet";
    default              : return "?";
  }
}

const char* ps_proto_l3_str(ps_proto_l3_t proto)
{
  switch (proto)
  {
    case PS_PROTO_L3_NONE: return "-";
    case PS_PROTO_L3_ICMP: return "ICMP";
    case PS_PROTO_L3_ARP : return "ARP";
    case PS_PROTO_L3_IP4 : return "IPv4";
    case PS_PROTO_L3_IP6 : return "IPv6";
    default              : return "?";
  }
}

const char* ps_proto_l4_str(ps_proto_l4_t proto)
{
  switch (proto)
  {
    case PS_PROTO_L4_NONE: return "-";
    case PS_PROTO_L4_TCP : return "TCP";
    case PS_PROTO_L4_UDP : return "UDP";
    default              : return "?";
  }
}
