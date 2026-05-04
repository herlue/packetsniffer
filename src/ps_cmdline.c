#include <stdlib.h>
#include <limits.h>

#include "../include/ps_cmdline.h"

static int ps_cmdline_parse_int(const char*, int*, ps_error_t*);

#define PS_CMDLINE_SUCCESS  0
#define PS_CMDLINE_FAILURE -1

int ps_cmdline_parse
(
  int          argc,
  char**       argv,
  ps_config_t* config,
  ps_error_t*  error
)
{
  if (argc < 2)
    return PS_CMDLINE_SUCCESS;

  int i = 1;
  char *arg;

  while (i < argc)
  {
    arg = argv[i];
    if (*arg != '-')
    {
      ps_error_set(
        error,
        PS_ERROR_LEVEL_FATAL,
        PS_ERROR_TYPE_CLI,
        "expected \"-\" but got \"%s\"",
        arg
      );
      return PS_CMDLINE_FAILURE;
    }
    arg++;
    if (*arg == '\0' || *(arg + 1) != '\0')
    {
      ps_error_set(
        error,
        PS_ERROR_LEVEL_FATAL,
        PS_ERROR_TYPE_CLI,
        "invalid option: \"%s\"",
        arg
      );
      return PS_CMDLINE_FAILURE;
    }
    switch (*arg)
    {
      case 'c':
        if (++i >= argc)
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "usage: %s -c <packetcount>",
            *argv
          );
          return PS_CMDLINE_FAILURE;
        }
        arg = argv[i];
        if (ps_cmdline_parse_int(arg, &config->packetcount, error))
          return PS_CMDLINE_FAILURE;
        break;
      case 'f':
        if (++i >= argc)
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "usage: %s -f <filteroptions>",
            *argv
          );
          return PS_FAILURE;
        }
        arg = argv[i];
        return ps_filter_parse(&config->filter, error, arg, *argv);
      case 'i':
        if (++i >= argc)
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "usage: %s -i <interface>",
            *argv
          );
          return PS_CMDLINE_FAILURE;
        }
        arg = argv[i];
        config->interface = arg;
        break;
      case 'l':
        config->log = 1;
        break;
      case 'r':
        config->resolve = 1;
        break;
      case 't':
        if (++i >= argc)
        {
          ps_error_set(
            error,
            PS_ERROR_LEVEL_FATAL,
            PS_ERROR_TYPE_CLI,
            "usage: %s -t <timeoutMs>",
            *argv
          );
          return PS_CMDLINE_FAILURE;
        }
        arg = argv[i];
        if (ps_cmdline_parse_int(arg, &config->timeoutms, error))
          return PS_CMDLINE_FAILURE;
        break;
      
      default:
        ps_error_set(
          error,
          PS_ERROR_LEVEL_FATAL,
          PS_ERROR_TYPE_CLI,
          "unknown option \"-%c\"",
          *arg
        );
        return PS_CMDLINE_FAILURE;
    }
    i++;
  }
  return PS_CMDLINE_SUCCESS;
}

static int ps_cmdline_parse_int(const char* arg, int* ptr, ps_error_t* error)
{
  long  tmpval;
  char* endptr;
  tmpval = strtol(arg, &endptr, 10);
  if (*endptr != '\0')
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_CLI,
      "expected integer but got \"%s\"",
      arg
    );
    return PS_CMDLINE_FAILURE;
  }
  if (tmpval < 0 || tmpval >= INT_MAX)
  {
    ps_error_set(
      error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_CLI,
      "expected integer in range [0, %d] but got \"%s\"",
      INT_MAX,
      arg
    );
    return PS_CMDLINE_FAILURE;
  }
  *ptr = (int) tmpval;
  return PS_CMDLINE_SUCCESS;
}
