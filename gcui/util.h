#ifndef __UTIL_H__
#define __UTIL_H__

#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mema.h"
#include "log.h"

#define WORKING_DIRECTORY_BUFF_LEN                              200

// General porpuse utilities
// Used all across the code, not really related to any
// particular module

// fatal - ogs out a fatal message and aborts the program
#define ftl(...)                                                {_FORM_LOG(TERM_ERR, "fatal", __VA_ARGS__); abort();}

// Aborts the program if x == NULL while logging
// "[name of the variable holding x] cannot be null"
#define abort_if_null(x)                                        if(x == NULL) {ftl(#x " cannot be null");}

#define swap(X, Y, T)                                           do { T s = X; X = Y; Y = s; } while(0)
#define unused(x)                                               ((void)(x))
#define clear(x)                                                memset((void *)(x), 0, sizeof(*(x)))
#define clear_arr(x)                                            memset(x, 0, sizeof(x))
#define arrlen(s)                                               (sizeof(s)/sizeof(*s))
#define arrcopy(s, d)                                           (memcpy(d, s, sizeof(s)))

#define min(x, y)                                               ((x) <= (y) ? (x) : (y))
#define max(x, y)                                               ((x) >= (y) ? (x) : (y))

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
typedef unsigned int uint;
#endif

static inline uint64_t
millis(void)
{
	struct timeval current_time = {0};
	gettimeofday(&current_time, 0);
    return current_time.tv_sec*1000 + current_time.tv_usec/1000;
}

static inline int 
msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

static inline int 
read_file(const char * file_path, char ** data, size_t *size)
{
    ssize_t read_size = 0;
    FILE * fp = NULL;
    
    extern int errno;
    
    gmema_init();

    fp = fopen(file_path, "r");
    if(fp == NULL)
    {
        err("cannot open \"%s\"", file_path)
        return errno;
    }
    
    fseek(fp, 0, SEEK_END);
    read_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *data = gcalloc(read_size + 1, 1);

    if(size)
        *size = read_size;
    while(read_size > 0)
        read_size -= fread(*data, sizeof(**data), read_size, fp);

    fclose(fp);

    return 0;
}

static inline const char *
get_working_directory( void )
{
    static char buff[WORKING_DIRECTORY_BUFF_LEN] = {0};

    getcwd(buff, sizeof(buff));

    return buff;
}

static long long
strtolmm(const char *str, const char **end_ptr, long long min,
    long long max, int base)
{
    long long ret = 0;

    ret = strtol(str, NULL, 10);

    if(ret > max)
        ret = max;
    if(ret < min)
        ret = min;

    return ret;
}

#endif