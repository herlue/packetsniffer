#include <net/ethernet.h>
#include <netinet/ether.h>
#include <string.h>
#include "../../include/handler/ps_pkthandler_eth.h"
#include "../../include/handler/ps_pkthandler_ip4.h"
#include "../../include/handler/ps_pkthandler_eth_arp.h"
#include "../../include/ps_util.h"

int ps_pkthandler_eth
(
  const ps_context_t* context,
  ps_pktinfo_t*       pktinfo,
  uint32_t            length,
  const uchar*        data
)
{
  if (length < ETH_HLEN)
  {
    ps_error_set(
      context->error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PACKET,
      "incomplete ethernet header [size = %u bytes]",
      length
    );
    ps_error_log(context->error);
    return PS_FAILURE;
  }

  const struct ether_header* ethhdr = (const struct ether_header*) data;
  
  int rc;

  rc = ps_util_eth_str((const struct ether_addr*) ethhdr->ether_dhost, pktinfo->l2.eth.dst_addr, context->error);
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l2.eth.dst_addr, "?");
  }

  rc = ps_util_eth_str((const struct ether_addr*) ethhdr->ether_shost, pktinfo->l2.eth.src_addr, context->error);
  if (rc)
  {
    ps_error_log(context->error);
    strcpy(pktinfo->l2.eth.src_addr, "?");
  }

  pktinfo->proto_l2 = PS_PROTO_L2_ETH;

  uint32_t     nextlength = length - ETH_HLEN;
  const uchar* nextdata   = data + ETH_HLEN;

  ps_pkthandler_fn nexthandler;

  switch (ntohs(ethhdr->ether_type))
  {
    case ETHERTYPE_IP:
      nexthandler = ps_pkthandler_ip4;
      break;

    case ETHERTYPE_ARP:
      nexthandler = ps_pkthandler_eth_arp;
      break;

    default:
      return PS_FAILURE;
  }

  return nexthandler(context, pktinfo, nextlength, nextdata);
}
