#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include "../include/ps_util.h"

void flush_stdin(void)
{
  int c;
  do
  {
    c = getchar();
  }
  while (c != '\n' && c != EOF);
}

int ps_util_parse_ipv4
(
  const char*     str,
  struct in_addr* addr,
  ps_error_t*     error
)
{
  if (!inet_aton(str, addr))
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_CLI,
      "expected ipv4 address but got \"%s\"",
      str
    );
    return PS_FAILURE;
  }
  return PS_SUCCESS;
}

int ps_util_ipv4_str
(
  const struct in_addr* addr,
  char*                 buffer,
  socklen_t             buffersize,
  ps_error_t*           error
)
{
  if (!inet_ntop(AF_INET, addr, buffer, buffersize))
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_FORMAT,
      "failed to format ipv4 address [value = %u]",
      addr->s_addr
    );
    return PS_FAILURE;
  }
  return PS_SUCCESS;
}

int ps_util_eth_str
(
  const struct ether_addr* addr,
  char*                    buffer,
  ps_error_t*              error
)
{
  if (!ether_ntoa_r(addr, buffer))
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_FORMAT,
      "failed to format mac address"
    );
    return PS_FAILURE;
  }
  return PS_SUCCESS;
}

