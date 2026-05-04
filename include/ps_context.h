#ifndef PS_CONTEXT_H
#define PS_CONTEXT_H

#include "ps_config.h"
#include "ps_error.h"

typedef struct
{
  ps_config_t* config;
  ps_error_t*  error;
}
ps_context_t;

void ps_context_init(ps_context_t*, ps_config_t*, ps_error_t*);

#endif
