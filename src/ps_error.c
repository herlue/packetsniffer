#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "../include/ps_error.h"

static void        ps_error_message_clear (ps_error_t*);
static void        ps_error_vset          (ps_error_t*, ps_error_level_t, ps_error_type_t, const char*, va_list);
static void        ps_error_vlog_raw      (ps_error_level_t, ps_error_type_t, const char*, va_list);
static const char* ps_error_level_str     (ps_error_level_t);
static const char* ps_error_type_str      (ps_error_type_t);
static const char* ps_error_level_color   (ps_error_level_t);

void ps_error_clear(ps_error_t* error)
{
  if (!error)
    return;

  error->level = PS_ERROR_LEVEL_NONE;
  error->type  = PS_ERROR_TYPE_NONE;
  ps_error_message_clear(error);
}

static void ps_error_message_clear(ps_error_t* error)
{
  if (!error)
    return;

  error->_truncated = 0;
  error->message[0] = '\0';
}

static void ps_error_vset
(
  ps_error_t*      error,
  ps_error_level_t level,
  ps_error_type_t  type,
  const char*      fmt,
  va_list          vargs
)
{
  if (!error)
    return;

  error->level = level;
  error->type  = type;
  ps_error_message_clear(error);
  
  if (!fmt)
    return;

  int n = vsnprintf(error->message, sizeof(error->message), fmt, vargs);

  if (n < 0)
  {
    ps_error_message_clear(error);
    fprintf(stderr, "%s vnsprintf failed while formatting error message\n", __func__); // refactor
    return;
  }

  if ((size_t) n >= sizeof(error->message))
    error->_truncated = 1;
}


void ps_error_set
(
  ps_error_t*      error,
  ps_error_level_t level,
  ps_error_type_t  type,
  const char*      fmt,
  ...
)
{
  va_list vargs;
  va_start(vargs, fmt);
  ps_error_vset(error, level, type, fmt, vargs);
  va_end(vargs);
}

void ps_error_log(const ps_error_t* error)
{
  if (!error || error->level == PS_ERROR_LEVEL_NONE)
    return;

  fprintf(
    stderr,
    "%s%-8s%s %-8s | %s\n",
    ps_error_level_color(error->level),
    ps_error_level_str(error->level),
    PS_ERROR_LEVEL_COLOR_RESET,
    ps_error_type_str(error->type),
    error->message
  );

  if (error->_truncated)
  {
    // REFACTOR
    fprintf(stderr, "^ error message above has been truncated\n\n");
  }

  if (error->level == PS_ERROR_LEVEL_FATAL)
    exit(EXIT_FAILURE);
}

static void ps_error_vlog_raw
(
  ps_error_level_t level,
  ps_error_type_t  type,
  const char*      fmt,
  va_list          vargs
)
{
  ps_error_t error;
  ps_error_vset(&error, level, type, fmt, vargs);
  ps_error_log(&error);
}

void ps_error_log_raw
(
  ps_error_level_t level,
  ps_error_type_t  type,
  const char*      fmt,
  ...
)
{
  va_list vargs;
  va_start(vargs, fmt);
  ps_error_vlog_raw(level, type, fmt, vargs);
  va_end(vargs);
}

int ps_error_message_truncated(const ps_error_t* error)
{
  return error ? error->_truncated : 0;
}

static const char* ps_error_level_str(ps_error_level_t level)
{
  switch (level)
  {
    case PS_ERROR_LEVEL_NONE : return "NONE";
    case PS_ERROR_LEVEL_FATAL: return "FATAL";
    case PS_ERROR_LEVEL_WARN : return "WARN";
    default                  : return "UNKNOWN";
  }
}

static const char* ps_error_type_str(ps_error_type_t type)
{
  switch (type)
  {
    case PS_ERROR_TYPE_NONE    : return "NONE";
    case PS_ERROR_TYPE_INTERNAL: return "INTERNAL";
    case PS_ERROR_TYPE_CLI     : return "CLI";
    case PS_ERROR_TYPE_IO      : return "IO";
    case PS_ERROR_TYPE_PARSE   : return "PARSE";
    case PS_ERROR_TYPE_FORMAT  : return "FORMAT";
    case PS_ERROR_TYPE_PCAP    : return "PCAP";
    case PS_ERROR_TYPE_PACKET  : return "PACKET";
    default                    : return "UNKNOWN";
  }
}

static const char* ps_error_level_color(ps_error_level_t type)
{
  switch (type)
  {
    case PS_ERROR_LEVEL_FATAL: return PS_ERROR_LEVEL_COLOR_FATAL;
    case PS_ERROR_LEVEL_WARN : return PS_ERROR_LEVEL_COLOR_WARN;
    default                  : return PS_ERROR_LEVEL_COLOR_RESET;
  }
}
