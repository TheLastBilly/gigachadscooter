#include "visual.h"

#include <errno.h>

#include "commons.h"
#include "log.h"

#define VISUAL_X                                -1.0
#define VISUAL_Y                                .685
#define SPEED_FONT                              GRAPHICS_FONT_MONOID_64
#define SPEED_FONT_COLOR                        GRAPHICS_HEX2RGBA(0xffffffff)

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

extern int errno;

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

static inline void
draw_text(const char * text)
{
    graphics_draw_text(SPEED_FONT, 0.f, 0.f, SPEED_FONT_COLOR, 
        "TESST");
}

static void
init( void )
{
}

static bool
draw( bool redraw )
{
    char buf[SPEED_MAX_LEN + 2] = {0}; 
    char * speed_data = NULL;
    long long speed = 0;
    int ret = 0;

    static long long last_speed = 0;

    ret = read_file(VISUAL_SOCKET_BASE "/speedometer.sock", &speed_data, NULL);
    if(!ret)
    {
        speed = strtolmm(speed_data, NULL, SPEED_MIN_VAL, SPEED_MAX_VAL, 10);
    }
    else
        speed = 0;

    snprintf(buf, SPEED_MAX_LEN, "%lli", speed);

    if(!redraw && last_speed == speed)
        return false;
    
    last_speed = speed;
    
    draw_radius_bars(speed, &bars, GRAPHICS_HEX2RGBA(0xffffffff));
    draw_text(buf);

    return true;
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
}

static const visual_t visual = (visual_t){
    .init = init,

    .draw = draw,

    .on_sleep = on_sleep,
    .on_wake = on_wake,

    .terminate = terminate
};
VISUAL_DEFINE_STATIC(_visual_speedometer, visual);