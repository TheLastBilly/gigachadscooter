#ifndef __LOG_H__
#define __LOG_H__

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

#define TERM_INF                                                TERM_CYAN
#define TERM_WRN                                                TERM_YELLOW
#define TERM_ERR                                                TERM_RED
#define TERM_ABT                                                TERM_RED
#define TERM_DBG                                                TERM_CYAN
#define TERM_FUN                                                TERM_PURPLE
#define TERM_FLE                                                TERM_BLUE
#define TERM_TME                                                TERM_WHITE
#define TERM_NRM                                                TERM_RESET

#define __LOGGER_UPTIME_FMT                                     "%s"
#define __LOGGER_UPTIME_VAL                                     ""
#define __LOGGER_PRINTF                                         printf

#ifdef NDEBUG
#define __LOGGER_FILE_FMT                                       "%s"
#define __LOGGER_FILE_VAL                                       ""
#else
#define __LOGGER_FILE_FMT                                       TERM_FLE "[%s:%d] "
#define __LOGGER_FILE_VAL                                       __FILENAME__, __LINE__
#endif

#define _BASE_LOG(_end, _fmt, ...)                              \
{                                                               \
    __LOGGER_PRINTF(                                            \
        __LOGGER_UPTIME_FMT                                     \
        __LOGGER_FILE_FMT                                       \
                                                                \
        TERM_NRM _fmt "%s"_end,                                 \
                                                                \
        __LOGGER_UPTIME_VAL,                                    \
        __LOGGER_FILE_VAL,                                      \
        __VA_ARGS__                                             \
    );                                                          \
}

/**
 * @brief Alians for printf.
 * 
 * @param ...       printf arguments
 */
#define pnt(...)                                                __LOGGER_PRINTF(__VA_ARGS__)

/**
 * @brief Helper macro used to define loggers
 * 
 * @param _color    Log message's text color
 * @param _name     Log message's display name
 * @param ...       printf arguments
 */
#define _FORM_LOG(_color, _name, ...)                           _BASE_LOG("\n\r", _color "[" _name "]" TERM_NRM " " __VA_ARGS__, "")

/**
 * @brief Helper macro used to define loggers. 
 * 
 * Custom tailing characters can be specified.
 * 
 * @param _end      Tailing characters
 * @param _color    Log message's text color
 * @param _name     Log message's display name
 * @param ...       printf arguments
 */
#define _FORM_LOG_END(_end, _color, _name, ...)                 _BASE_LOG(_end, _color "[" _name "]" TERM_NRM " " __VA_ARGS__, "")

/**
 * @brief Prints out information log
 * 
 * @param ...       printf arguments
 */
#define inf(...)                                                _FORM_LOG(TERM_INF, "info", __VA_ARGS__)

/**
 * @brief Prints out information log
 * 
 * Custom tailing characters can be specified.
 * 
 * @param ...       printf arguments
 */
#define infe(_end, ...)                                         _FORM_LOG_END(_end, TERM_INF, "info", __VA_ARGS__)

#ifndef NDEBUG

/**
 * @brief Prints out debug log
 * 
 * @param ...       printf arguments
 */
#define dbg(...)                                                _FORM_LOG(TERM_DBG, "debug", __VA_ARGS__)

/**
 * @brief Prints out debug log
 * 
 * Custom tailing characters can be specified.
 * 
 * @param ...       printf arguments
 */
#define dbge(_end, ...)                                         _FORM_LOG_END(_end, TERM_DBG, "debug", __VA_ARGS__)

#else

/**
 * @brief Prints out debug log (Disabled)
 * 
 * @param ...       printf arguments
 */
#define dbg(...)                                                { do {} while(0); }

/**
 * @brief Prints out debug log
 * 
 * Custom tailing characters can be specified. (Disabled)
 * 
 * @param ...       printf arguments
 */
#define dbge(_end, ...)                                         { do {} while(0); }

#endif

/**
 * @brief Prints out warning log
 * 
 * @param ...       printf arguments
 */
#define wrn(...)                                                _FORM_LOG(TERM_WRN, "warning", __VA_ARGS__)

/**
 * @brief Prints out warning log
 * 
 * Custom tailing characters can be specified.
 * 
 * @param ...       printf arguments
 */
#define wrne(_end, ...)                                         _FORM_LOG_END(_end, TERM_WRN, "warning", __VA_ARGS__)

/**
 * @brief Prints out error log
 * 
 * @param ...       printf arguments
 */
#define err(...)                                                _FORM_LOG(TERM_ERR, "error", __VA_ARGS__)

/**
 * @brief Prints out error log
 * 
 * Custom tailing characters can be specified.
 * 
 * @param ...       printf arguments
 */
#define erre(_end, ...)                                         _FORM_LOG_END(_end, TERM_ERR, "error", __VA_ARGS__)

#endif