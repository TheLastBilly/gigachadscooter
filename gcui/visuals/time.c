#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "comm.h"
#include "geometry.h"
#include "commons.h"

#define VISUAL_TEXT_Y                           .15
#define VISUAL_TEXT_X                           -1.0
#define TIME_FONT                               GRAPHICS_FONT_MONOID_128
#define TIME_TEXT_BUFFER_LEN                    10
#define TIME_FONT_COLOR                         GRAPHICS_HEX2RGBA(0xffffffff)
#define TIME_INTRO_TEXT                         "Giga Chad"
#define TIME_INTRO_ALPHA_DELAY                  10

extern int errno;

static char 
TIME_BUFFER[TIME_TEXT_BUFFER_LEN] = {0};

static void
intro( void )
{
    rgba_t color = TIME_FONT_COLOR;

    graphics_draw_text(TIME_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, GRAPHICS_HEX2RGBA(0x000000ff), 
        TIME_INTRO_TEXT);
    graphics_draw_text(TIME_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, color, 
        TIME_INTRO_TEXT);
    graphics_render();

    color.a += 1;
    graphics_msleep(TIME_INTRO_ALPHA_DELAY);
}

static void
init( void )
{
}

static void
draw( void )
{
    time_t tt;
    struct tm * ti;

    time(&tt);
    ti = localtime(&tt);

    snprintf(TIME_BUFFER, TIME_TEXT_BUFFER_LEN, 
        "%02d%02d", 

        ti->tm_hour,
        ti->tm_min
    );

    graphics_draw_text(TIME_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, TIME_FONT_COLOR, 
        TIME_BUFFER);
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
    .intro = intro,

    .init = init,

    .draw = draw,

    .on_sleep = on_sleep,
    .on_wake = on_wake,

    .terminate = terminate
};
VISUAL_DEFINE_STATIC(_visual_time, visual);