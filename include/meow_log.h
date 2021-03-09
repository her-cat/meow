#ifndef MEOW_LOG_H
#define MEOW_LOG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MEOW_LOG_BUFFER_SIZE 1024
#define MEOW_LOG_DATE_STRLEN  64

#define MEOW_DEBUG_MSG_SIZE 512
#define MEOW_TRACE_MSG_SIZE 512
#define MEOW_WARN_MSG_SIZE 512
#define MEOW_ERROR_MSG_SIZE 512

extern char meow_debug_str[MEOW_DEBUG_MSG_SIZE];
extern char meow_trace_str[MEOW_TRACE_MSG_SIZE];
extern char meow_warn_str[MEOW_WARN_MSG_SIZE];
extern char meow_error_str[MEOW_ERROR_MSG_SIZE];

#ifndef MEOW_DEBUG
#define meow_debug(str, ...)
#else
#define meow_debug(str, ...)                                                         \
    snprintf(meow_debug_str, MEOW_DEBUG_MSG_SIZE, "%s: " str " in %s on line %d.", __func__, ##__VA_ARGS__, __FILE__, __LINE__); \
    meow_log_put(MEOW_LOG_DEBUG, meow_debug_str);
#endif

#define meow_trace(str, ...)                                                         \
    snprintf(meow_trace_str, MEOW_TRACE_MSG_SIZE, "%s: " str " in %s on line %d.", __func__, ##__VA_ARGS__, __FILE__, __LINE__); \
    meow_log_put(MEOW_LOG_TRACE, meow_trace_str);

#define meow_warn(str, ...)                                                         \
    snprintf(meow_warn_str, MEOW_ERROR_MSG_SIZE, "%s: " str " in %s on line %d.", __func__, ##__VA_ARGS__, __FILE__, __LINE__); \
    meow_log_put(MEOW_LOG_WARNING, meow_warn_str);

#define meow_error(str, ...)                                                         \
    snprintf(meow_error_str, MEOW_ERROR_MSG_SIZE, "%s: " str " in %s on line %d.", __func__, ##__VA_ARGS__, __FILE__, __LINE__); \
    meow_log_put(MEOW_LOG_ERROR, meow_error_str); \
    exit(-1);

enum meow_log_level
{
    MEOW_LOG_DEBUG = 0,
    MEOW_LOG_TRACE,
    MEOW_LOG_INFO,
    MEOW_LOG_NOTICE,
    MEOW_LOG_WARNING,
    MEOW_LOG_ERROR,
};

void meow_log_put(int level, char *cnt);

#endif /* MEOW_LOG_H */
