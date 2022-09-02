#include "visual.h"

#include <errno.h>
#include <math.h>

#include "log.h"
#include "comm.h"
#include "geometry.h"
#include "commons.h"

#define VISUAL_TEXT_Y                           .025
#define VISUAL_TEXT_X                           .025

#define TEMPERATURE_MAX_VAL                     1000.0f
#define TEMPERATURE_MIN_VAL                     -60.0f

#define TEMPERATURE_FONT                        GRAPHICS_FONT_MONOID_26
#define TEMPERATURE_TEXT_BUFFER_LEN             10
#define TEMPERATURE_FONT_COLOR                  GRAPHICS_HEX2RGBA(0xffffffff)

typedef struct ctx_t
{
    struct
    {
        comm_t comm;
        comm_buffer_t buffer;
    } comm;

} ctx_t;
static ctx_t ctx = {0};

extern int errno;

static char 
TEMPERATURE_BUFFER[TEMPERATURE_TEXT_BUFFER_LEN] = {0};

static void
intro( void )
{
    
}

static void
init( void )
{
    comm_init(&ctx.comm.comm, VISUAL_SOCKET_BASE "/temperature.sock");
}

static void
draw( void )
{
    float temp = 0.0f;

    comm_read(&ctx.comm.comm, &ctx.comm.buffer);
    if(ctx.comm.buffer.len > TEMPERATURE_TEXT_BUFFER_LEN)
        return;
    
    temp = strtof(ctx.comm.buffer.data, NULL);
    if(temp < TEMPERATURE_MIN_VAL || temp > TEMPERATURE_MAX_VAL)
        return;

    snprintf(TEMPERATURE_BUFFER, TEMPERATURE_TEXT_BUFFER_LEN, "%.1f\xB0""C", temp);
    graphics_draw_text(TEMPERATURE_FONT, VISUAL_TEXT_X, VISUAL_TEXT_Y, TEMPERATURE_FONT_COLOR, 
        TEMPERATURE_BUFFER);
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
VISUAL_DEFINE_STATIC(_visual_temperature, visual);