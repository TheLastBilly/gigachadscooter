#ifndef __COMM_H__
#define __COMM_H__

#include <sys/select.h>
#include <sys/un.h>

#define COMM_MAX_CONNECTIONS                1
#define COMM_MAX_READ                       256

typedef struct comm_t
{
    int fd;
    struct sockaddr_un s_addr;

    struct
    {
        int fd[COMM_MAX_CONNECTIONS];
        fd_set set;
    } clients;
} comm_t;

typedef struct comm_buffer_t
{
    char data[COMM_MAX_READ];
    int len;
} comm_buffer_t;

int comm_init( comm_t * comm, const char * path );
int comm_destroy( comm_t * comm );
int comm_read( comm_t * comm, comm_buffer_t * buffer );

#endif