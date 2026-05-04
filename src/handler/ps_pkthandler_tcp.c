#define _DEFAULT_SOURCE
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "../../include/handler/ps_pkthandler_tcp.h"

int ps_pkthandler_tcp
(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  if (length < sizeof(struct tcphdr))
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete tcp header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct tcphdr* tcphdr = (const struct tcphdr*) data;

  uint16_t hlen = 4 * tcphdr->doff;
  if (hlen < 20)
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "invalid tcp header size [hlen = %u bytes]",
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
      "incomplete tcp header [size = %u bytes, hlen = %u bytes]",
      length,
      hlen
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  pktinfo->l4.tcp.src_port = ntohs(tcphdr->source);
  pktinfo->l4.tcp.dst_port = ntohs(tcphdr->dest);

  pktinfo->l4.tcp.flags = 0;

  if (tcphdr->fin)
    pktinfo->l4.tcp.flags |= PS_TCP_FLAG_FIN;
  if (tcphdr->syn)
    pktinfo->l4.tcp.flags |= PS_TCP_FLAG_SYN;
  if (tcphdr->rst)
    pktinfo->l4.tcp.flags |= PS_TCP_FLAG_RST;
  if (tcphdr->ack)
    pktinfo->l4.tcp.flags |= PS_TCP_FLAG_ACK;

  pktinfo->proto_l4 = PS_PROTO_L4_TCP;


  return PS_SUCCESS;
}
