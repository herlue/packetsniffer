#ifndef PS_FLAGS_H
#define PS_FLAGS_H

#include <stdint.h>

#define PS_FRAME_PROTO_ARP 1
#define PS_FRAME_PROTO_IP4 2
#define PS_FRAME_PROTO_IP6 4

#define PS_DGRAM_PROTO_ICMP 1
#define PS_DGRAM_PROTO_TCP  2
#define PS_DGRAM_PROTO_UDP  4

typedef uint8_t ps_proto_flags_t;

#endif
