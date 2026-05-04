#ifndef PS_PKTHANDLER_H
#define PS_PKTHANDLER_H

#include <pcap.h>
#include "ps_types.h"

void ps_pkthandler(uchar*, const struct pcap_pkthdr*, const uchar*);

#endif
