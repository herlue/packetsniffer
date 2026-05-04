#ifndef PS_CONFIG_H
#define PS_CONFIG_H

#include "ps_filter.h"

typedef struct ps_config
{
  char*       interface;
  int         _interface_allocated;
  int         linktype;
  int         log;
  int         packetcount;
  int         resolve;
  int         timeoutms;
  ps_filter_t filter;
}
ps_config_t;

extern const struct ps_config PS_CONFIG_DEFAULT;

void ps_config_init(ps_config_t*);

#endif
