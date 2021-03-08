#include <time.h>
#include <unistd.h>
#include "meow_log.h"

char meow_debug_str[MEOW_DEBUG_MSG_SIZE];
char meow_trace_str[MEOW_TRACE_MSG_SIZE];
char meow_warn_str[MEOW_WARN_MSG_SIZE];
char meow_error_str[MEOW_ERROR_MSG_SIZE];

void meow_log_put(int level, char *cnt)
{
    int n;
    time_t t;
    int output;
    struct tm *p;
    const char *level_str;
    char date_str[MEOW_LOG_DATE_STRLEN];
    char log_str[MEOW_LOG_BUFFER_SIZE];


    switch (level) {
        case MEOW_LOG_DEBUG:
            level_str = "DEBUG";
            output = STDOUT_FILENO;
            break;
        case MEOW_LOG_NOTICE:
            level_str = "NOTICE";
            output = STDOUT_FILENO;
            break;
        case MEOW_LOG_ERROR:
            level_str = "ERROR";
            output = STDERR_FILENO;
            break;
        case MEOW_LOG_WARNING:
            level_str = "WARNING";
            output = STDERR_FILENO;
            break;
        case MEOW_LOG_TRACE:
            level_str = "TRACE";
            output = STDOUT_FILENO;
            break;
        default:
            level_str = "INFO";
            output = STDOUT_FILENO;
            break;
    }

    t = time(NULL);
    p = localtime(&t);
    snprintf(date_str, MEOW_LOG_DATE_STRLEN, "%d-%02d-%02d %02d:%02d:%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

    n = snprintf(log_str, MEOW_LOG_BUFFER_SIZE, "[%s]\t%s\t%s\n", date_str, level_str, cnt);
    if (write(output, log_str, n) < 0) {
        printf("write log failed. Error: %s[%d].\n", strerror(errno), errno);
    }
}
