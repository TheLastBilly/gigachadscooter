#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "geometry.h"

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

    ret = graphics_load_sprite(BLINKERS_ICON_PATH, &ctx.sprite);
    if(ret < 0)
        abort();
}

static bool
draw( bool redraw )
{
    int i = 0, ret = 0;
    char * buffer = NULL;
    bool r = false, l = false, should_clear = false;

    static bool was_on[2] = {0};

    ret = read_file(VISUAL_SOCKET_BASE "/blinker.sock", &buffer, NULL);
    if(ret)
        return false;

    if(strcmp(buffer, "R") == 0)
        r = true;
    else if(strcmp(buffer, "L") == 0)
        l = true;

    if(strcmp(buffer, "LR") == 0)
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

        if(ctx.blinker[i].blink_on && (redraw || !was_on[i]))
        {
            graphics_draw_sprite(&ctx.sprite, BLINKERS_W, BLINKERS_H, 
                i == 1 ? BLINKERS_X : 1.0f - BLINKERS_X - BLINKERS_W, BLINKERS_Y, 
                0.0, i == 0, false);
            
            should_clear = true;
        }

        was_on[i] = ctx.blinker[i].blink_on;
    }

    return should_clear;
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