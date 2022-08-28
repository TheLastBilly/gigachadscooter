#ifndef __MEMA_H__
#define __MEMA_H__

#include <stdlib.h>
#include <string.h>

// Garbage collerctor
// Used to allocate memory without having to worry about
// freeing it later

// Local memory allocators, lmema_init needs to be called
// before any of the other functions are called. Before
// returning.

// Initializes local mema context
#define lmema_init()                                            struct mema_context _mema_local = {0}

// malloc using local mema context
#define lmalloc(size)                                           _mema_malloc(&_mema_local, size)

// calloc using local mema context
#define lcalloc(nmemb, size)                                    _mema_calloc(&_mema_local, nmemb, size)

// free using local mema context
#define lfree(ptr)                                              _mema_free(&_mema_local, ptr)

// Frees local mema context
#define lmema_deinit()                                          _mema_free_all(&_mema_local)

// lreturn and lnreturn can be used to instead of return
// to exit a function by first freeing any locally
// allocated memory

// Frees local mema context and returns x
#define lreturn                                                 lmema_deinit(); return 

// Frees local mema context and returns
#define lnreturn                                                lmema_deinit(); return

// Global memory allocators
// Used to allocate memory on a global context. The data
// will be freed when the program exits (destructor called
// via atexit)

// Initializes global mema context if it wasn't already initialized
#define gmema_init()                                            _mema_global_init()

// malloc call using global mema context
#define gmalloc(size)                                           _mema_malloc(&_mema_global, size)

// calloc call using global mema context
#define gcalloc(nmemb, size)                                    _mema_calloc(&_mema_global, nmemb, size)

// free call using global mema context
#define gfree(ptr)                                              _mema_free(&_mema_global, ptr)

// Meant for internal use.
// All the data allocated on the garbage collector is
// stored as a linked list, which makes freeing kind of
// a pain in the ass, such is life
struct _mema_block
{
    void * data;

    size_t size;
    size_t nmemb;

    struct _mema_block * prev, * next;
};

// Garbage collector context
struct mema_context
{
    struct _mema_block *blocks;
};

// Meant for internal use.
// Context for the garbage collector global allocations
struct mema_context _mema_global;

void _mema_free_all(struct mema_context * ctx);
void * _mema_last_block(struct mema_context * ctx);
void * _mema_find_block(struct mema_context * ctx, void * ptr);
struct _mema_block * _allocate_next_block(struct _mema_block *r);
void * _mema_malloc(struct mema_context * ctx, size_t size);
void * _mema_calloc(struct mema_context * ctx, size_t nmemb, size_t size);
void _mema_free(struct mema_context * ctx, void *ptr);
void _mema_free_on_exit();
void _mema_global_init();

#endif