#include "graphics.h"
#include "log.h"
#include "util.h"
#include "comm.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL.h>

#include "visuals/visual.h"

#define BACKGROUND_COLOR                            0x181A18FF

#define MAIN_THREAD_WAIT                            32

#define SET_CURRENT_VISUALS(_v)                     \
{                                                   \
    visuals.IDs = _v;                               \
    visuals.len = arrlen(_v);                       \
}

#define RUN_ON_VISUALS(_v, _f)                      \
{                                                   \
    int _i = 0;                                     \
    for(_i; _i < visuals.len; _i++)                 \
        visual_get_visual(visuals.IDs[_i])->_f();   \
}

struct
{
    const visual_id_t *IDs;
    int len;
} visuals;

static const visual_id_t MAIN_TAB[] =
{
    VISUAL_SPEEDOMETER,
    VISUAL_BATTERY,
    VISUAL_TIME
};

int 
main(int argc, char const *argv[])
{
    int i = 0;
    uint32_t ticks = 0;
    comm_t comm = {0};
    comm_buffer_t buffer = {0};

    gmema_init();

    SET_CURRENT_VISUALS(MAIN_TAB);

    RUN_ON_VISUALS(MAIN_TAB, init);

    graphics_init();
    while(1)
    {
        ticks = graphics_millis();

        graphics_clear();
        RUN_ON_VISUALS(MAIN_TAB, draw);
        graphics_render();

        if ((graphics_millis() - ticks) < MAIN_THREAD_WAIT) {
            graphics_msleep(MAIN_THREAD_WAIT);
        }
    }
    graphics_terminate();

    RUN_ON_VISUALS(MAIN_TAB, terminate);

    return 0;
}
