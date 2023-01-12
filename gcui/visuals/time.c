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
init( void )
{
}

static bool
draw( bool redraw )
{
    time_t tt;
    struct tm * ti;

    static char last_time[arrlen(TIME_BUFFER)] = {0};

    time(&tt);
    ti = localtime(&tt);

    snprintf(TIME_BUFFER, TIME_TEXT_BUFFER_LEN, 
        "%02d%02d", 

        ti->tm_hour,
        ti->tm_min
    );

    // Maybe not the most efficient thing in the world... but it works*
    if(!redraw && strcmp(last_time, TIME_BUFFER) == 0)
        return false;

    arrcopy(TIME_BUFFER, last_time);
    graphics_draw_text(TIME_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, TIME_FONT_COLOR, 
        TIME_BUFFER);
    
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
VISUAL_DEFINE_STATIC(_visual_time, visual);