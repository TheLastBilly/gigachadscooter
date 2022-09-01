#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <pwd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"
#include "comm.h"

#define CONNECTION_TIMEOUT              0

static char READ_BUFFER[COMM_MAX_READ] = {0};
extern int errno;

int
comm_init( comm_t * comm, const char * path )
{
    int ret = 0;
    size_t size = 0;

    if(!path)
        return -1;
    
    comm_destroy(comm);

    comm->fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(comm->fd < 0)
    {
        err("cannot create socket on path \"%s\": %s", path,
            strerror(errno));
        return errno;
    }

    comm->s_addr.sun_family = AF_UNIX;

    size = strlen(path);
    size = size > arrlen(comm->s_addr.sun_path) ? arrlen(comm->s_addr.sun_path) : size;
    strncpy(comm->s_addr.sun_path, path, size);
    unlink(path);

    ret = bind(comm->fd, (const struct sockaddr *)&comm->s_addr, sizeof(comm->s_addr));
    if(ret < 0)
    {
        err("cannot bind socket: %s", strerror(errno));
        return ret;
    }
    
    ret = listen(comm->fd, COMM_MAX_CONNECTIONS);
    if(ret < 0)
    {
        err("cannot listen on socket: %s", strerror(errno));
        return ret;
    }
    
    return ret;
}

int
comm_destroy( comm_t * comm )
{
    int i = 0;

    if(comm->fd)
        close(comm->fd);
    
    for(; i < arrlen(comm->clients.fd); i++)
        if(comm->clients.fd[i] > 0)
            close(comm->clients.fd[i]);
    
    clear(comm);

    return 0;
}

int
comm_read( comm_t * comm, comm_buffer_t * buffer )
{
    int i = 0, s = 0, ret = 0;
    int max_fd = 0;
    size_t size = 0;
    struct timeval tv = {0};

    if(!buffer)
        return -1;

    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    FD_ZERO(&comm->clients.set);
    FD_SET(comm->fd, &comm->clients.set);

    max_fd = comm->fd;
    size = sizeof(comm->s_addr);

    // Get socket activity
    for(i = 0; i < COMM_MAX_CONNECTIONS; i++)
    {
        if(comm->clients.fd[i] <= 0)
            continue;
        
        FD_SET(comm->clients.fd[i], &comm->clients.set);
        if(comm->clients.fd[i] > max_fd)
            max_fd = comm->clients.fd[i];
    }

    s = select(max_fd + 1, &comm->clients.set, NULL, NULL, &tv);
    if(s < 0)
    {
        if(errno == EINTR)
            return 0;
        
        return -1;
    }

    // Listen for incomming connections
    if(FD_ISSET(comm->fd, &comm->clients.set))
    {
        s = accept(comm->fd, (struct sockaddr *)&comm->s_addr, (socklen_t *)&size);
        if(s < 0)
        {
            wrn("invalid socket (%d) read: %s", comm->fd, strerror(errno));
            return 0;
        }
        
        for(i = 0; i < COMM_MAX_CONNECTIONS; i++)
        {
            if(comm->clients.fd[i] > 0)
                continue;
            
            comm->clients.fd[i] = s;
            break;
        }
    }

    // Get any received data
    for(i = 0; i < COMM_MAX_CONNECTIONS; i++)
    {
        if(comm->clients.fd[i] <= 0 || !FD_ISSET(comm->clients.fd[i], &comm->clients.set))
            continue;
        
        clear_arr(READ_BUFFER);
        ret = read(comm->clients.fd[i], READ_BUFFER, COMM_MAX_READ * sizeof(READ_BUFFER[0]));

        if(ret < 0)
        {
            err("cannot read from socket (%d): %s", ret, strerror(errno));
        }
        else
        {
            buffer->len = ret;
            memset(buffer->data, 0, sizeof(buffer->data));
            memcpy(buffer->data, READ_BUFFER, ret * sizeof(READ_BUFFER[0]));

            if(ret < arrlen(READ_BUFFER))
                buffer->data[ret -1] = '\0';
        }

        FD_CLR(comm->clients.fd[i], &comm->clients.set);
        close(comm->clients.fd[i]);
        comm->clients.fd[i] = 0;

        break;
    }

    FD_ZERO(&comm->clients.set);

    return 0;
}