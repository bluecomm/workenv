#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

inline bool get_time_now(char *fmt_time, size_t max_size) {
    time_t tt = time(NULL);
    struct tm *tm = localtime(&tt);
    size_t ret_size = strftime(fmt_time, max_size, "%Y-%m-%d %H:%M:%S %a", tm);
    return (ret_size <= max_size);
}

inline void write_fmtd(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

#ifndef WRITE_FORMATTED_LOG
#define WRITE_FORMATTED_LOG(_fmt_, args...) \
do { \
    char fmt_time[32] = {0}; \
    get_time_now(fmt_time, 31); \
    write_fmtd("[%s][%s][%s][%d]: " _fmt_ "\n",\
                fmt_time, __FILE__, __FUNCTION__, __LINE__, ##args); \
} while(0)
#endif
