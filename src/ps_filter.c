#include "../include/ps_filter.h"
#include "../include/ps_util.h"

#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

static int ps_filter_get_optval(char**, char**, ps_error_t*, const char*, const char*);

const struct ps_filter PS_FILTER_DEFAULT = {
  .has_src_ip4  = 0,
  .has_dst_ip4  = 0,
  .has_src_ip6  = 0,
  .has_dst_ip6  = 0,
  .has_src_port = 0,
  .has_dst_port = 0
};

void ps_filter_init(ps_filter_t* filter)
{
  *filter = PS_FILTER_DEFAULT;
}

int ps_filter_parse(ps_filter_t* filter, ps_error_t* error, char* str, const char* exe)
{
  char *optptr, *delimptr, *optnam, *optval;

  optptr = strtok(str, "+");
  while (optptr)
  {
    if ((delimptr = strchr(optptr, ':')))
      *delimptr = '\0';

    optnam = optptr;
    puts(optptr);

    if (!strcmp(optnam, "frame"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "frame:<arp|ip4|ip6>"))
        return PS_FAILURE;

      do
      {
        delimptr = strchr(optval, ',');
        if (delimptr)
          *delimptr = '\0';

        if (!strcmp(optval, "arp"))
          filter->frame_proto |= PS_FRAME_PROTO_ARP;
        else if (!strcmp(optval, "ip4"))
          filter->frame_proto |= PS_FRAME_PROTO_IP4;
        else if (!strcmp(optval, "ip6"))
          filter->frame_proto |= PS_FRAME_PROTO_IP6;
        else
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "frame: expected arp|ip4|ip6 but got \"%s\"",
            optval
          );
          return PS_FAILURE;
        }

        if (delimptr)
        {
          delimptr++;
          optval = delimptr;
        }
      }
      while (delimptr);
    }
    else if (!strcmp(optnam, "dgram"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "dgram:<icmp|tcp|udp"))
        return PS_FAILURE;

      do
      {
        delimptr = strchr(optval, ',');
        if (delimptr)
          *delimptr = '\0';

        if (!strcmp(optval, "icmp"))
          filter->dgram_proto |= PS_DGRAM_PROTO_ICMP;
        else if (!strcmp(optval, "tcp"))
          filter->dgram_proto |= PS_DGRAM_PROTO_TCP;
        else if (!strcmp(optval, "udp"))
          filter->dgram_proto |= PS_DGRAM_PROTO_UDP;
        else
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "dgram: expected icmp|tcp|udp but got \"%s\"",
            optval
          );
          return PS_FAILURE;
        }

        if (delimptr)
        {
          delimptr++;
          optval = delimptr;
        }
      }
      while (delimptr);
    }
    else if (!strcmp(optnam, "segment"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "segment:<protocols>"))
        return PS_FAILURE;

      struct servent* serv;
      
      do
      {
        delimptr = strchr(optval, ',');
        if (delimptr)
          *delimptr = '\0';

        serv = getservbyname(optval, "tcp");
        if (serv)
        {
          // use a linked list here
        }
        else
        {
          // try udp here
        }
      }
      while (delimptr);
      // use getservbyname here
    }

    else if (!strcmp(optnam, "srcip") || !strcmp(optnam, "srcip4"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "srcip:<ipv4addr>"))
        return PS_FAILURE;

      if (ps_util_parse_ipv4(optval, &filter->src_ip4, error))
        return PS_FAILURE;

      filter->has_src_ip4 = true;
    }
    else if (!strcmp(optnam, "dstip") || !strcmp(optnam, "dstip4"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "dstip:<ipv4addr>"))
        return PS_FAILURE;
        
      if (ps_util_parse_ipv4(optval, &filter->dst_ip4, error))
        return PS_FAILURE;
    }
    /* IPv6 currently not supported */
    else if (!strcmp(optnam, "srcip6") || !(strcmp(optnam, "dstip6")))
    {
      ps_error_set(
        error,
        PS_ERROR_LEVEL_FATAL,
        PS_ERROR_TYPE_CLI,
        "ipv6 is currently not supported"
      );
      return PS_FAILURE;
    }
    else if (!strcmp(optnam, "srcport"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "srcport:<portnumber>"))
        return PS_FAILURE;

      // ... parse port

      filter->has_src_port = true;
    }
    else if (!strcmp(optnam, "dstport"))
    {
      if (ps_filter_get_optval(&delimptr, &optval, error, exe, "dstport:<portnumber>"))
        return PS_FAILURE;

      // ... parse port

      filter->has_dst_port = true;
    }
    else
    {
      ps_error_set(
        error,
        PS_ERROR_LEVEL_FATAL,
        PS_ERROR_TYPE_CLI,
        "unknown filter option: \"%s\"",
        optnam
      );
      return PS_FAILURE;
    }

    optptr = strtok(NULL, "+");
  }

  return PS_SUCCESS;
}

static int ps_filter_get_optval
(
  char**      delimptr,
  char**      optval,
  ps_error_t* error,
  const char* exe,
  const char* usage
)
{
  if (!*delimptr)
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_CLI,
      "usage: %s -f %s",
      exe,
      usage
    );
    return PS_FAILURE;
  }
  (*delimptr)++;
  *optval = *delimptr;
  return PS_SUCCESS;
}

// static size_t ps_filter_parse_optlist(char* optval, char** optlist, size_t m)
// {
//   if (!*optval)
//     return 0;

//   size_t n;
//   char*  delimptr;

//   for (n = 0; n < m; n++)
//   {
//     optlist[n] = optval;
//     delimptr = strchr(optval, ',');
//     if (!delimptr)
//       break;

//     *delimptr = '\0';
//     delimptr++;
//     optval = delimptr;
//   }

//   return n;
// }