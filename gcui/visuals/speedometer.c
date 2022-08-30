#include "visual.h"

#include "log.h"

typedef struct speedometer_t
{
    struct
    {
        comm_t comm;
        comm_buffer_t buffer;
    } comm;

} speedometer_t;

static speedometer_t speedometer;

static void
init( void )
{
    comm_init(&speedometer.comm.comm, "/tmp/spd.sock");
}

static void
draw( void )
{
    comm_read(&speedometer.comm.comm, &speedometer.comm.buffer);
    graphics_draw_text(GRAPHICS_FONT_MONOID_64, -1, -1, GRAPHICS_HEX2RGBA(0xffffffff),
        speedometer.comm.buffer.data);
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
    comm_destroy(&speedometer.comm.comm);
}

static const visual_t visual = (visual_t){
    .init = init,

    .draw = draw,

    .on_sleep = on_sleep,
    .on_wake = on_wake,

    .terminate = terminate
};
const visual_t * const _visual_speedometer = &visual;