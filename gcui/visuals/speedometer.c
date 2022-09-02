#include "visual.h"

#include <errno.h>

#include "commons.h"
#include "log.h"
#include "comm.h"

#define VISUAL_X                                -1.0
#define VISUAL_Y                                .675

#define SPEED_MAX_VAL                           80
#define SPEED_MIN_VAL                           0

#define SPEED_LOW_COLOR                         GRAPHICS_HEX2RGBA(0x45ba48ff)
#define SPEED_MID_COLOR                         GRAPHICS_HEX2RGBA(0xfff84cff)
#define SPEED_HIGH_COLOR                        GRAPHICS_HEX2RGBA(0xc30505ff)

#define SPEED_MID_AREA                          100.0
#define SPEED_MID_START                         (90.0 - SPEED_MID_AREA/2.0)
#define SPEED_MID_END                           (90.0 + SPEED_MID_AREA/2.0)

#define SPEED_BARS                              20
#define SPEED_DELTA                             (((float)SPEED_MAX_VAL)/((float)SPEED_BARS))
#define SPEED_DELTA_ANGLE                       (180.0f/SPEED_BARS)
#define SPEED_RADIUS                            .25
#define SPEED_BAR_WIDTH                         .08
#define SPEED_BAR_HEIGHT                        .02

#define SPEED_MAX_LEN                           8

#if COMM_MAX_READ > SPEED_MAX_LEN
#define SPEED_END_IDX                           SPEED_MAX_LEN
#else
#define SPEED_END_IDX                           0
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

rgba_t 
bar_color_cb(float t, float value, int bar)
{
    if(t >= 0.0f && t < SPEED_MID_START)
        return SPEED_LOW_COLOR;
    
    else if(t >= SPEED_MID_START && t <= SPEED_MID_END)
        return SPEED_MID_COLOR;
    
    else
        return SPEED_HIGH_COLOR;
    
    return (rgba_t){0};
}

static commons_bars_t bars = (commons_bars_t){
    .h = SPEED_BAR_HEIGHT,
    .w = SPEED_BAR_WIDTH,

    .max = SPEED_MAX_VAL,
    .radius = SPEED_RADIUS,
    .ratio = true,

    .max_bars = SPEED_BARS,

    .color_cb = bar_color_cb
};

static void
intro( void )
{
    
}

static void
init( void )
{
    comm_init(&ctx.comm.comm, VISUAL_SOCKET_BASE "/speedometer.sock");
}

static void
draw( void )
{
    char buf[SPEED_MAX_LEN + 2] = {0}; 
    long long speed = 0;

    comm_read(&ctx.comm.comm, &ctx.comm.buffer);
    if(ctx.comm.buffer.len > SPEED_MAX_LEN)
        return;
    
    speed = strtolmm(ctx.comm.buffer.data, NULL,
        SPEED_MIN_VAL, SPEED_MAX_VAL, 10);

    snprintf(buf, SPEED_MAX_LEN, "%lli", speed);
    graphics_draw_text(GRAPHICS_FONT_MONOID_64, VISUAL_X, VISUAL_Y, 
        GRAPHICS_HEX2RGBA(0xffffffff), buf);
    
    draw_radius_bars(speed, &bars, GRAPHICS_HEX2RGBA(0xffffffff));
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
VISUAL_DEFINE_STATIC(_visual_speedometer, visual);