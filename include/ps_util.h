#ifndef PS_UTIL_H
#define PS_UTIL_H

#include <netinet/in.h>
#include <net/ethernet.h>
#include "ps_error.h"

void flush_stdin        (void);
int  ps_util_parse_ipv4 (const char*, struct in_addr*, ps_error_t*);
int  ps_util_ipv4_str   (const struct in_addr*, char*, socklen_t, ps_error_t*);
int  ps_util_eth_str    (const struct ether_addr*, char*, ps_error_t*);

#endif
