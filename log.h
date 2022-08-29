#ifndef __LOGGER_H__
#define __LOGGER_h__

#include <stdio.h>
#include <time.h>
#include <string.h>

// Logger

// It's just used to log events and get the function 
// and line number where they were called from
#define __FILENAME__                                            (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Color definitions for the logger
#define TERM_BLUE                                               "\033[1;94m"
#define TERM_GREEN                                              "\033[1;92m"
#define TERM_YELLOW                                             "\033[1;93m"
#define TERM_RED                                                "\033[1;91m"
#define TERM_CYAN                                               "\033[1;96m"
#define TERM_WHITE                                              "\033[1;97m"
#define TERM_PURPLE                                             "\033[1;95m"
#define TERM_RESET                                              "\033[0m"

#define TERM_TME                                                TERM_WHITE
#define TERM_INF                                                TERM_WHITE
#define TERM_WRN                                                TERM_YELLOW
#define TERM_ERR                                                TERM_RED
#define TERM_ABT                                                TERM_RED
#define TERM_DBG                                                TERM_CYAN
#define TERM_FUN                                                TERM_PURPLE
#define TERM_FLE                                                TERM_BLUE
#define TERM_NRM                                                TERM_RESET

#define STRTIME_BUFFER_LEN                                      20

// Not very C99 ish, but hey, nobody is perfect
#define _STD_LOG(_file, _fmt, ...)                              \
{                                                               \
    char _tstr[STRTIME_BUFFER_LEN] = {0};                       \
    get_time_str(_tstr, sizeof(_tstr)/sizeof(*_tstr));          \
    fprintf(                                                    \
        _file,                                                  \
        TERM_TME "[%s] "                                        \
        TERM_FLE "[%s:%d] "                                     \
        TERM_NRM _fmt "%s\n",                                   \
                                                                \
        _tstr,                                                  \
        __FILENAME__, __LINE__,                                 \
        __VA_ARGS__                                             \
    );                                                          \
}

// Used for logger internals
#define _FORM_LOG(_color, _name, ...)                           _STD_LOG(stdout, _color "[" _name "]" TERM_NRM " " __VA_ARGS__, "")

// ptr - works as regular printf
#define prt(...)                                                printf(__VA_ARGS__);

// info - logs regular information, use it as regular printf
#define inf(...)                                                _FORM_LOG(TERM_INF, "info", __VA_ARGS__)

// debug - logs debug information
#define dbg(...)                                                _FORM_LOG(TERM_DBG, "debug", __VA_ARGS__)

// warning - logs warnings
#define wrn(...)                                                _FORM_LOG(TERM_WRN, "warning", __VA_ARGS__)

// error - logs errors
#define err(...)                                                _FORM_LOG(TERM_ERR, "error", __VA_ARGS__)

// Gets current time string on "%02d:%02d:%02d" format
void get_time_str(char *buf, size_t len);

#endif