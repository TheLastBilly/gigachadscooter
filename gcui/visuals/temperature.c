#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "geometry.h"
#include "commons.h"

#define VISUAL_TEXT_Y                           .025
#define VISUAL_TEXT_X                           .025

#define TEMPERATURE_MAX_VAL                     1000.0f
#define TEMPERATURE_MIN_VAL                     -60.0f

#define TEMPERATURE_FONT                        GRAPHICS_FONT_MONOID_26
#define TEMPERATURE_TEXT_BUFFER_LEN             10
#define TEMPERATURE_FONT_COLOR                  GRAPHICS_HEX2RGBA(0xffffffff)

extern int errno;

static char 
TEMPERATURE_BUFFER[TEMPERATURE_TEXT_BUFFER_LEN] = {0};

static void
init( void )
{
}

static bool
draw( bool redraw )
{
    int ret = 0;
    char *temp_data = NULL;
    float temp = 0.0f;

    static float last_temp = 0.0f;

    ret = read_file(VISUAL_SOCKET_BASE "/temperature.sock", &temp_data, NULL);
    if(!ret)
    {
        temp = strtof(temp_data, NULL);
    }
    else
        temp = 0.f;
    
    if(temp < TEMPERATURE_MIN_VAL || temp > TEMPERATURE_MAX_VAL)
        return false;

    if(!redraw && last_temp == temp)
        return false;

    last_temp = temp;

    snprintf(TEMPERATURE_BUFFER, TEMPERATURE_TEXT_BUFFER_LEN, "%.1f °C", temp);
    graphics_draw_text(TEMPERATURE_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, TEMPERATURE_FONT_COLOR, 
        TEMPERATURE_BUFFER);
    
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
VISUAL_DEFINE_STATIC(_visual_temperature, visual);