#ifndef PS_PROTO_TCP_H
#define PS_PROTO_TCP_H

typedef enum
{
  PS_TCP_FLAG_FIN = 0x01,
  PS_TCP_FLAG_SYN = 0x02,
  PS_TCP_FLAG_RST = 0x04,
  PS_TCP_FLAG_ACK = 0x10
}
ps_tcp_flag_t;

#endif
