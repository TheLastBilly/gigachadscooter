#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "geometry.h"
#include "commons.h"

#define VISUAL_TEXT_Y                           .925
#define VISUAL_TEXT_X                           -1.0

#define BATTERY_MAX_VAL                         100
#define BATTERY_MIN_VAL                         0

#define BATTERY_BARS                            20
#define BATTERY_DELTA                           (((float)BATTERY_MAX_VAL)/((float)BATTERY_BARS))
#define BATTERY_DELTA_ANGLE                     (180.0f/BATTERY_BARS)
#define BATTERY_RADIUS                          .075
#define BATTERY_BAR_WIDTH                       .02
#define BATTERY_BAR_HEIGHT                      .005

#define BATTERY_MAX_LEN                         5

#if COMM_MAX_READ > BATTERY_MAX_LEN
#define BATTERY_END_IDX                         BATTERY_MAX_LEN
#else
#define BATTERY_END_IDX                         0
#endif

extern int errno;

static commons_bars_t bars = (commons_bars_t){
    .h = BATTERY_BAR_HEIGHT,
    .w = BATTERY_BAR_WIDTH,

    .max = BATTERY_MAX_VAL,
    .radius = BATTERY_RADIUS,

    .ratio = true,

    .max_bars = BATTERY_BARS
};

static void
init( void )
{
}

static bool
draw( bool redraw )
{
    int ret = 0;
    char *buffer = NULL;
    long long level = 0;
    char buf[BATTERY_MAX_LEN + 2] = {0}; 

    static long long past_level = 0;


    ret = read_file(VISUAL_SOCKET_BASE "/battery.sock", &buffer, NULL);
    if(!ret)
    {
        level = strtolmm(buffer, NULL,BATTERY_MIN_VAL, BATTERY_MAX_VAL, 10);
    }
    else
        level = 0;
    

    if(!redraw && past_level == level)
        return false;

    past_level = level;

    snprintf(buf, BATTERY_MAX_LEN, "%lli%%", level);
    
    graphics_draw_text(GRAPHICS_FONT_MONOID_64, 1- 0.545, 1 - 0.0725, 
        GRAPHICS_HEX2RGBA(0xffffffff), buf);
    draw_radius_bars(level, &bars, GRAPHICS_HEX2RGBA(0xffffffff));

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
VISUAL_DEFINE_STATIC(_visual_battery, visual);