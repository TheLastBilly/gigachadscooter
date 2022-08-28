#include "mema.h"

#include <errno.h>

#include "util.h"

extern int errno;

static bool initialized = false;
struct mema_context _mema_global = {0};

// Frees all the elements allocated on mema context
void
_mema_free_all(struct mema_context * ctx)
{
    abort_if_null(ctx);

    struct _mema_block *b = NULL, *ob = NULL;

    // Go through all the blocks on the context
    // and free them and their data
    b = ctx->blocks;
    while(b)
    {
        if(b->data)
            free(b->data);

        ob = b;
        b = ob->next;

        free(ob);
    }
}

// Meant for internal use.
// Gets the last element on the context's block
// list, that is, the first element with no data
void *
_mema_last_block(struct mema_context * ctx)
{
    struct _mema_block *b = NULL;

    abort_if_null(ctx);

    b = ctx->blocks;
    while(b)
    {
        if(!b->next)
            break;

        b = b->next;
    }

    if(!b)
        ctx->blocks = b = calloc(1, sizeof(*b));

    return b;
}

// Meant for internal use.
// Finds the block containing ptr. Returns NULL
// if none was found.
void *
_mema_find_block(struct mema_context * ctx, void * ptr)
{
    struct _mema_block *b = NULL, *m = NULL;

    abort_if_null(ctx);

    b = ctx->blocks;
    while(b)
    {
        if(b->data == ptr)
            m = b;

        b = b->next;
    }

    return m;
}

// Meant for internal use.
// Allocates a block next to another
struct _mema_block *
_allocate_next_block(struct _mema_block *r)
{
    struct _mema_block *b = NULL;

    b = (struct _mema_block *)calloc(1, sizeof(*b->next));
    if(!b)
        ftl("%s", strerror(errno));

    b->prev = r;

    return b;
}

// Allocates memory and adds allocation to mema context
void *
_mema_malloc(struct mema_context * ctx, size_t size)
{
    struct _mema_block *b = NULL;

    abort_if_null(ctx);

    b = _mema_last_block(ctx);
    b->data = malloc(size);
    if(!b->data)
        ftl("%s", strerror(errno));

    b->size = size;
    b->nmemb = 1;

    b->next = _allocate_next_block(b);

    return b->data;
}

// Allocates and initializes memory and adds allocation
// to mema context
void *
_mema_calloc(struct mema_context * ctx, size_t nmemb, size_t size)
{
    struct _mema_block *b = NULL;

    abort_if_null(ctx);

    b = _mema_last_block(ctx);
    b->data = calloc(nmemb, size);
    if(!b->data)
        ftl("%s", strerror(errno));

    b->size = size;
    b->nmemb = nmemb;

    b->next = _allocate_next_block(b);

    return b->data;
}

// Frees memory allocated on mema context and deletes
// allocation from context. If no such ptr was found
// on ctx, it does nothing
void 
_mema_free(struct mema_context * ctx, void *ptr)
{
    struct _mema_block *b = NULL;

    abort_if_null(ctx);

    b = _mema_find_block(ctx, ptr);
    if(!b)
        return;
    
    if(b->prev)
        b->prev->next = b->next;
    if(b->next)
        b->next->prev = b->prev; 

    // In case we are freeing the first block
    // on the context, set its next block as
    // the new first
    if(b == ctx->blocks)
        ctx->blocks = b->next;

    if(b->data)
        free(b->data);
    
    free(b);
}

// Meant from internal use.
// Sets up atexit callback for mema global context
void
_mema_free_on_exit()
{
    _mema_free_all(&_mema_global);
    initialized = false;
}

// Meant from internal use.
// Initializes global context
void
_mema_global_init()
{
    if(initialized)
        return;
    
    atexit(_mema_free_on_exit);
    clear(&_mema_global);

    initialized = true;
}