#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "comm.h"
#include "geometry.h"
#include "commons.h"

#define BLINKERS_X                      0.025
#define BLINKERS_Y                      0.775
#define BLINKERS_W                      0.1
#define BLINKERS_H                      0.2

#define BLINKERS_ICON_PATH              "icons/blinker.png"

#define BLINKERS_BLINK_DELAY            500

extern int errno;

typedef struct ctx_t
{
    struct
    {
        comm_t comm;
        comm_buffer_t buffer;
    } comm;

    struct
    {
        bool blink_on;
        uint32_t last_update;
    } blinker[2];

    sprite_t sprite;
} ctx_t;
static ctx_t ctx = {0};

static void
init( void )
{
    int ret = 0;

    comm_init(&ctx.comm.comm, VISUAL_SOCKET_BASE "/blinker.sock");
    ret = graphics_load_sprite(BLINKERS_ICON_PATH, &ctx.sprite);
    if(ret < 0)
        abort();
}

static void
draw( void )
{
    int i = 0;
    bool r = false, l = false;
    comm_read(&ctx.comm.comm, &ctx.comm.buffer);

    if(strcmp(ctx.comm.buffer.data, "R") == 0)
        r = true;
    else if(strcmp(ctx.comm.buffer.data, "L") == 0)
        l = true;

    if(strcmp(ctx.comm.buffer.data, "LR") == 0)
    {
        r = l = true;
        ctx.blinker[0] = ctx.blinker[1];
    }
    
    for(i = 0; i < 2; i++)
    {
        if(i == 0 && !r)
            continue;
        if(i == 1 && !l)
            continue;
        
        if(graphics_millis() - ctx.blinker[i].last_update > BLINKERS_BLINK_DELAY)
        {
            ctx.blinker[i].last_update = graphics_millis();
            ctx.blinker[i].blink_on = !ctx.blinker[i].blink_on;
        }

        if(ctx.blinker[i].blink_on)
            graphics_draw_sprite(&ctx.sprite, BLINKERS_W, BLINKERS_H, 
                i == 1 ? BLINKERS_X : 1.0f - BLINKERS_X - BLINKERS_W, BLINKERS_Y, 
                0.0, i == 0, false);
    }
}

static void
on_sleep( void )
{
}
static void
on_wake( void )
{
}

static void
terminate( void )
{
    comm_destroy(&ctx.comm.comm);
    graphics_free_sprite(&ctx.sprite);
}

static const visual_t visual = (visual_t){
    .init = init,

    .draw = draw,

    .on_sleep = on_sleep,
    .on_wake = on_wake,

    .terminate = terminate
};
VISUAL_DEFINE_STATIC(_visual_blinkers, visual);