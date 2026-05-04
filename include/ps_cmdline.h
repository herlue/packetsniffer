#ifndef PS_CMDLINE_H
#define PS_CMDLINE_H

#include "ps_config.h"
#include "ps_error.h"
#include "ps_filter.h"

int ps_cmdline_parse(int, char**, ps_config_t*, ps_error_t*);

#endif
