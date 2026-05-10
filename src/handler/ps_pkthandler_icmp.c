#include <netinet/ip_icmp.h>
#include "../../include/handler/ps_pkthandler_icmp.h"

int ps_pkthandler_icmp
(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  if (length < sizeof(struct icmphdr))
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete icmp header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct icmphdr* icmphdr = (const struct icmphdr*) data;

  pktinfo->l4.icmp.type = icmphdr->type;
  pktinfo->l4.icmp.code = icmphdr->code;

  pktinfo->proto_l4 = PS_PROTO_L4_ICMP;

  return PS_SUCCESS;
}
