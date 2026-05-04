#include "../include/ps_context.h"

void ps_context_init(ps_context_t* context, ps_config_t* config, ps_error_t* error)
{
  context->config = config;
  context->error  = error;
}
