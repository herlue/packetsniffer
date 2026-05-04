#ifndef PS_ERROR_H
#define PS_ERROR_H

#include <stdarg.h>

#define PS_SUCCESS  0
#define PS_FAILURE -1

#define PS_ERROR_MESSAGE_SIZE 256

#define PS_ERROR_LEVEL_COLOR_FATAL "\x1b[31m"
#define PS_ERROR_LEVEL_COLOR_WARN  "\x1b[33m"
#define PS_ERROR_LEVEL_COLOR_RESET "\x1b[0m"

typedef enum
{
  PS_ERROR_LEVEL_NONE,
  PS_ERROR_LEVEL_FATAL,
  PS_ERROR_LEVEL_WARN
}
ps_error_level_t;

typedef enum
{
  PS_ERROR_TYPE_NONE,
  PS_ERROR_TYPE_INTERNAL,
  PS_ERROR_TYPE_CLI,
  PS_ERROR_TYPE_IO,
  PS_ERROR_TYPE_PARSE,
  PS_ERROR_TYPE_FORMAT,
  PS_ERROR_TYPE_PCAP,
  PS_ERROR_TYPE_PACKET
}
ps_error_type_t;

typedef struct ps_error
{
  ps_error_level_t level;
  ps_error_type_t  type;
  int              _truncated;
  char             message[PS_ERROR_MESSAGE_SIZE];
}
ps_error_t;


void               ps_error_clear             (ps_error_t*);
void               ps_error_set               (ps_error_t*, ps_error_level_t, ps_error_type_t, const char*, ...);
void               ps_error_log               (const ps_error_t*);
void               ps_error_log_raw           (ps_error_level_t, ps_error_type_t, const char*, ...);
int                ps_error_message_truncated (const ps_error_t*);

#endif
