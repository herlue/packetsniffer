#ifndef PS_FILTER_H
#define PS_FILTER_H

#include <stdbool.h>
#include <netinet/in.h>

#include "ps_flags.h"
#include "ps_error.h"

#define PS_FILTER_OPTLIST_SIZE 8

typedef struct ps_filter
{
  bool has_src_ip4;
  bool has_dst_ip4;
  bool has_src_ip6;
  bool has_dst_ip6;
  bool has_src_port;
  bool has_dst_port;

  ps_proto_flags_t frame_proto;
  ps_proto_flags_t dgram_proto;
  ps_proto_flags_t segment_proto;
  ps_proto_flags_t app_proto;

  struct in_addr src_ip4;
  struct in_addr dst_ip4;

  struct in6_addr src_ip6;
  struct in6_addr dst_ip6;
  
  in_port_t src_port;
  in_port_t dst_port;
}
ps_filter_t;

extern const struct ps_filter PS_FILTER_DEFAULT;

void ps_filter_init (ps_filter_t*);
int  ps_filter_parse(ps_filter_t*, ps_error_t*, char*, const char*);

#endif
