#include "../include/ps_pkthandler.h"
#include "../include/ps_context.h"
#include "../include/ps_pktinfo.h"
#include "../include/handler/ps_pkthandler_eth.h"

void ps_pkthandler(uchar* user, const struct pcap_pkthdr* pkthdr, const uchar* data)
{
  const ps_context_t* context = (const ps_context_t*) user;

  ps_pktinfo_t pktinfo;
  ps_pktinfo_clear(&pktinfo);

  if (!ps_pkthandler_eth(context, &pktinfo, pkthdr->caplen, data))
    ps_pktinfo_print(&pktinfo);
}
