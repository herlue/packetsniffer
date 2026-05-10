#include <netinet/ip.h>
#include <string.h>
#include "../../include/handler/ps_pkthandler_ip4.h"
#include "../../include/handler/ps_pkthandler_icmp.h"
#include "../../include/handler/ps_pkthandler_tcp.h"
#include "../../include/handler/ps_pkthandler_udp.h"
#include "../../include/ps_util.h"

int ps_pkthandler_ip4(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  if (length < sizeof(struct iphdr))
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete ipv4 header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct iphdr* iphdr = (const struct iphdr*) data;

  uint hlen = 4 * iphdr->ihl;
  if (hlen < 20)
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "invalid ipv4 header size [hlen = %u bytes]",
      hlen
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }
  if (hlen > length)
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete ipv4 header [size = %u bytes, hlen = %u bytes]",
      length,
      hlen
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  int rc;

  rc = ps_util_ipv4_str(
    (const struct in_addr*) &iphdr->saddr,
    pktinfo->l3.ip4.src_addr,
    PS_IP4_ADDR_STRLEN,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.ip4.src_addr, "?");
  }

  rc = ps_util_ipv4_str(
    (const struct in_addr*) &iphdr->daddr,
    pktinfo->l3.ip4.dst_addr,
    PS_IP4_ADDR_STRLEN,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.ip4.dst_addr, "?");
  }

  pktinfo->proto_l3 = PS_PROTO_L3_IP4;

  uint32_t     nextlength = length - hlen;
  const uchar* nextdata   = data + hlen;

  ps_pkthandler_fn nextpkthandler;

  switch (iphdr->protocol)
  {
    case IPPROTO_ICMP:
      nextpkthandler = ps_pkthandler_icmp;
      break;
    case IPPROTO_TCP:
      nextpkthandler = ps_pkthandler_tcp;
      break;

    case IPPROTO_UDP:
      nextpkthandler = ps_pkthandler_udp;
      break;

    default:
      return PS_FAILURE;
  }

  return nextpkthandler(context, pktinfo, nextlength, nextdata);
}
