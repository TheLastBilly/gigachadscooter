#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "mema.h"
#include "logger.h"

#define TIME_STR_FORMAT                                         "%a %Y-%m-%d %H:%M:%S"
#define TIME_STR_BUFFER_SIZE                                    50
#define MS_IN_S                                                 1000

#define abort_if_null(x)                                        if(x == NULL) {err(#x " cannot be null");}

#define copy(x, y)                                              memcpy((void *)(x), (void *)(y), sizeof(*(x)))
#define clear(x)                                                memset((void *)(x), 0, sizeof(*(x)))

#define swap(X, Y, T)                                           do { T s = X; X = Y; Y = s; } while(0)
#define unused(x)                                               ((void)(x))
#define arrlen(s)                                               (sizeof(s)/sizeof(*s))

#define arrcopy(a, b)                                           memcpy((void *)(a), (void *)(b), sizeof(b))
#define arrclear(a)                                             memset((void *)(a), 0, sizeof(a))

#define min(x, y, t)                                            ({t _x = (x), _y = (y); ((_x) <= (_y) ? (_x) : (_y));})
#define max(x, y, t)                                            ({t _x = (x), _y = (y); ((_x) > (_y) ? (_x) : (_y));})

#define CREATE_ATOMICS(name, init_val)                          \
    static atomic_t name = ATOMIC_INIT(init_val);               \
                                                                \
    static inline void                                          \
    set_##name(atomic_val_t val) {atomic_set(&name, val);}      \
                                                                \
    static inline atomic_val_t                                  \
    get_##name() {return atomic_get(&name);}

#define PACKED                                                  __attribute__((packed))
#define PACKED_STRUCT(_name, ...)                               typedef struct PACKED _name { __VA_ARGS__ } PACKED _name;

#define NOTHING()                                               do {} while(0)

typedef uint8_t addr48_t[6];

/**
 * @brief Converts the macro provided in _x into a 
 * string literal
 * 
 * @param _x    macro to convert into string
 */
#define STR(_x)                                                 #_x

/**
 * @brief Converts the evaluation of the macro provided in _x into a 
 * string literal
 * 
 * @param _x    macro to convert into string
 */
#define XSTR(_x)                                                STR(_x)

/**
 * @brief Read file contents into memory. The pointer where
 * the read data is stored will be written to **data, and the 
 * size of the file is written on size
 * 
 * @param file_path file path to read
 * @param data      pointer to char* which will contained data address
 * @param size      pointer to read size variable
 * 
 * @return int      0 on success, errno on failure
 */
static inline int 
read_file(const char * file_path, char ** data, size_t *size)
{
    ssize_t read_size = 0;
    FILE * fp = NULL;
    
    extern int errno;
    
    gmema_init();

    fp = fopen(file_path, "r");
    if(fp == NULL)
        return errno;
    
    fseek(fp, 0, SEEK_END);
    read_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *data = (char *)gcalloc(read_size + 1, 1);

    if(size)
        *size = read_size;
    while(read_size > 0)
        read_size -= fread(*data, sizeof(**data), read_size, fp);

    fclose(fp);

    return 0;
}

/**
 * @brief Checks if file exists on filesystem
 * 
 * @param path  path to file
 * 
 * @return int  1 if file exists, 0 otherwhise
 */
static inline int
is_path( const char *path )
{
    struct stat st = {0};
    return (stat(path, &st) == 0);
}

static inline const char *
timestamp_to_str( time_t stamp )
{
    struct tm ts = {0};

    static char buffer[TIME_STR_BUFFER_SIZE] = {0};

    ts = *localtime(&stamp);
    strftime(buffer, sizeof(buffer), TIME_STR_FORMAT, &ts);

    return buffer;
}