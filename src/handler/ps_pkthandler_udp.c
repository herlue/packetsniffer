#include <netinet/udp.h>
#include <arpa/inet.h>
#include "../../include/handler/ps_pkthandler_udp.h"

int ps_pkthandler_udp
(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  uint hlen = sizeof(struct udphdr);
  if (length < hlen)
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete udp header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct udphdr* udphdr = (const struct udphdr*) data;

  pktinfo->l4.udp.src_port = ntohs(udphdr->source);
  pktinfo->l4.udp.dst_port = ntohs(udphdr->dest);

  pktinfo->proto_l4 = PS_PROTO_L4_UDP;

  return PS_SUCCESS;
}
