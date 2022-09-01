#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "comm.h"
#include "geometry.h"

#define VISUAL_X                                .025
#define VISUAL_Y                                .025

#define BATTERY_MAX_VAL                         100
#define BATTERY_MIN_VAL                         0

#define BATTERY_BARS                            40
#define BATTERY_DELTA                           (((float)BATTERY_MAX_VAL)/((float)BATTERY_BARS))
#define BATTERY_DELTA_ANGLE                     (180.0f/BATTERY_BARS)
#define BATTERY_RADIUS                          .5
#define BATTERY_BAR_WIDTH                       .025
#define BATTERY_BAR_HEIGHT                      .015

#define BATTERY_MAX_LEN                         4

#if COMM_MAX_READ > BATTERY_MAX_LEN
#define BATTERY_END_IDX                         BATTERY_MAX_LEN
#else
#define BATTERY_END_IDX                         0
#endif

typedef struct ctx_t
{
    struct
    {
        comm_t comm;
        comm_buffer_t buffer;
    } comm;

} ctx_t;

extern int errno;

static ctx_t ctx = {0};

const polygon_t TEMPLATE_BAR[] = 
{
    (polygon_t) {
        .vertices[0].x = 0,
        .vertices[1].x = 0,
        .vertices[2].x = BATTERY_BAR_WIDTH,

        .vertices[0].y = 0,
        .vertices[1].y = BATTERY_BAR_HEIGHT,
        .vertices[2].y = BATTERY_BAR_HEIGHT,
    },

    (polygon_t) {
        .vertices[0].x = BATTERY_BAR_WIDTH,
        .vertices[1].x = BATTERY_BAR_WIDTH,
        .vertices[2].x = 0,

        .vertices[0].y = BATTERY_BAR_HEIGHT,
        .vertices[1].y = 0,
        .vertices[2].y = 0,
    },
};

static void
intro( void )
{
    
}

static void
init( void )
{
    comm_init(&ctx.comm.comm, VISUAL_SOCKET_BASE "/battery.sock");
}

static void
draw_bars(long long battery, rgba_t color)
{
    int i = 0;
    int bars = 0;
    float t = 0.0f;
    polygon_t bar[2] = {0};

    bars = (int)roundf(battery/BATTERY_DELTA);

    for(i = 0; i < bars; i++)
    {
        bar[0] = TEMPLATE_BAR[0];
        bar[1] = TEMPLATE_BAR[1];

        

        t += BATTERY_DELTA_ANGLE;
    }
}   

static void
draw( void )
{
    char buf[BATTERY_MAX_LEN + 2] = {0}; 
    long long level = 0;

    comm_read(&ctx.comm.comm, &ctx.comm.buffer);
    if(ctx.comm.buffer.len > BATTERY_MAX_LEN)
        return;
    
    level = strtolmm(ctx.comm.buffer.data, NULL,
        BATTERY_MIN_VAL, BATTERY_MAX_VAL, 10);

    snprintf(buf, BATTERY_MAX_LEN, "%lli", level);
    graphics_draw_text(GRAPHICS_FONT_MONOID_28, VISUAL_X, VISUAL_Y, 
        GRAPHICS_HEX2RGBA(0xffffffff), buf);
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
}

static const visual_t visual = (visual_t){
    .intro = intro,

    .init = init,

    .draw = draw,

    .on_sleep = on_sleep,
    .on_wake = on_wake,

    .terminate = terminate
};
VISUAL_DEFINE_STATIC(_visual_battery, visual);