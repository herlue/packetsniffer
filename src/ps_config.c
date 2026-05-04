#include <stdio.h>
#include "../include/ps_config.h"

const struct ps_config PS_CONFIG_DEFAULT = {
  .interface            = NULL,
  ._interface_allocated = 0,
  .linktype             = 0,
  .log                  = 0,
  .packetcount          = 0,
  .resolve              = 0,
  .timeoutms            = 1000
};

void ps_config_init(ps_config_t* config)
{
  *config = PS_CONFIG_DEFAULT;
  ps_filter_init(&config->filter);
}
