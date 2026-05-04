#include <net/if_arp.h>
#include <string.h>
#include "../../include/handler/ps_pkthandler_eth_arp.h"
#include "../../include/proto/ps_eth_arp_payload.h"
#include "../../include/ps_util.h"

int ps_pkthandler_eth_arp
(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  if (length < sizeof(struct arphdr) + sizeof(ps_eth_arp_payload_t))
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete eth arp header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct arphdr* arphdr = (const struct arphdr*) data;

  pktinfo->l3.arp.opcode = ntohs(arphdr->ar_op);

  const ps_eth_arp_payload_t* arp_payload = (const ps_eth_arp_payload_t*) (data + sizeof(struct arphdr));

  int rc;

  rc = ps_util_eth_str(
    (const struct ether_addr*) arp_payload->sha,
    pktinfo->l3.arp.s_haddr,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.arp.s_haddr, "?");
  }

  rc = ps_util_ipv4_str(
    (const struct in_addr*) arp_payload->spa,
    pktinfo->l3.arp.s_paddr,
    PS_IP4_ADDR_STRLEN,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.arp.s_paddr, "?");
  }

  rc = ps_util_eth_str(
    (const struct ether_addr*) arp_payload->tha,
    pktinfo->l3.arp.t_haddr,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.arp.t_haddr, "?");
  }

  rc = ps_util_ipv4_str(
    (const struct in_addr*) arp_payload->tpa,
    pktinfo->l3.arp.t_paddr,
    PS_IP4_ADDR_STRLEN,
    context->error
  );
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l3.arp.t_paddr, "?");
  }

  pktinfo->proto_l3 = PS_PROTO_L3_ARP;

  return PS_SUCCESS;
}
