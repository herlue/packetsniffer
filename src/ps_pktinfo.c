#include <stdio.h>
#include <stdarg.h>
#include "../include/ps_pktinfo.h"
#include "../include/ps_error.h"
#include "../include/util/ps_color.h"

static void ps_pktinfo_format       (char*, size_t, const ps_pktinfo_t*);
static int  ps_pktinfo_append       (char**, size_t*, const char* fmt, ...);
static void ps_pktinfo_tcp_flags_str(uint8_t, char*);

void ps_pktinfo_clear(ps_pktinfo_t* pktinfo)
{
  pktinfo->proto_l2 = PS_PROTO_L2_NONE;
  pktinfo->proto_l3 = PS_PROTO_L3_NONE;
  pktinfo->proto_l4 = PS_PROTO_L4_NONE;
}

void ps_pktinfo_print(const ps_pktinfo_t* pktinfo)
{
  char buffer[PS_PKTINFO_BUFSIZE];
  ps_pktinfo_format(buffer, sizeof(buffer), pktinfo);
  puts(buffer);
}

static void ps_pktinfo_format(char* buffer, size_t buffersize, const ps_pktinfo_t* pktinfo)
{
  switch (pktinfo->proto_l2)
  {
    case PS_PROTO_L2_ETH:
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "[Ethernet] # Src: %17s | Dst: %17s\n",
        pktinfo->l2.eth.src_addr,
        pktinfo->l2.eth.dst_addr
      );
      break;

    default:
      break;
  }

  switch (pktinfo->proto_l3)
  {
    case PS_PROTO_L3_IP4:
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "[IPv4    ] # Src: %17s | Dst: %17s\n",
        pktinfo->l3.ip4.src_addr,
        pktinfo->l3.ip4.dst_addr
      );
      break;

    case PS_PROTO_L3_ARP:
      if (pktinfo->l3.arp.opcode == PS_ARP_REQ)
      {
        ps_pktinfo_append(
          &buffer,
          &buffersize,
          "[ARP(Req)] # Who has %14s | Tell %17s\n",
          pktinfo->l3.arp.t_paddr,
          pktinfo->l3.arp.s_paddr
        );
        break;
      }
      if (pktinfo->l3.arp.opcode == PS_ARP_REP)
      {
        ps_pktinfo_append(
          &buffer,
          &buffersize,
          "[ARP(Rep)] # %22s | is at %16s\n",
          pktinfo->l3.arp.s_paddr,
          pktinfo->l3.arp.s_haddr
        );
      }
      break;

    default:
      break;
  }

  switch (pktinfo->proto_l4)
  {
    case PS_PROTO_L4_ICMP:
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "[ICMP    ] # Type: %16u | Code: %16u\n",
        pktinfo->l4.icmp.type,
        pktinfo->l4.icmp.code
      );
      break;

    case PS_PROTO_L4_TCP:
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "[TCP     ] # Src: %17u | Dst: %17u",
        pktinfo->l4.tcp.src_port,
        pktinfo->l4.tcp.dst_port
      );
      if (pktinfo->l4.tcp.flags)
      {
        char flagbuf[64]; // refactor
        ps_pktinfo_tcp_flags_str(
          pktinfo->l4.tcp.flags,
          flagbuf
        );
        ps_pktinfo_append(
          &buffer,
          &buffersize,
          flagbuf
        );
      }
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "\n"
      );
      break;

    case PS_PROTO_L4_UDP:
      ps_pktinfo_append(
        &buffer,
        &buffersize,
        "[UDP     ] # Src: %17u | Dst: %17u\n",
        pktinfo->l4.udp.src_port,
        pktinfo->l4.udp.dst_port
      );
      break;

    default:
      break;
  }

  return;
}

static int ps_pktinfo_append(char** buffer, size_t* buffersize, const char* fmt, ...)
{
  if (!*buffersize)
    return PS_FAILURE;

  int n;

  va_list args;
  va_start(args, fmt);
  n = vsnprintf(*buffer, *buffersize, fmt, args);
  va_end(args);

  if (n < 0)
    return PS_FAILURE;

  if ((size_t) n >= *buffersize)
  {
    *buffer     += *buffersize - 1;
    *buffersize = 1;
    return PS_FAILURE;
  }

  *buffer     += n;
  *buffersize -= n;

  return PS_SUCCESS;
}

static void ps_pktinfo_tcp_flags_str(uint8_t flags, char* flagbuf)
{
  char* p = flagbuf;

  p += sprintf(p, "\t");

  if (flags & PS_TCP_FLAG_FIN)
    p += sprintf(p, PS_COLOR_BLUE "FIN " PS_COLOR_RESET);
  if (flags & PS_TCP_FLAG_SYN)
    p += sprintf(p, PS_COLOR_BLUE "SYN " PS_COLOR_RESET);
  if (flags & PS_TCP_FLAG_RST)
    p += sprintf(p, PS_COLOR_RED  "RST " PS_COLOR_RESET);
  if (flags & PS_TCP_FLAG_ACK)
    p += sprintf(p, "ACK ");
}
