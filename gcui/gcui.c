#include "graphics.h"
#include "log.h"
#include "util.h"
#include "comm.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "visuals/visual.h"

#define BACKGROUND_COLOR                            0x181A18FF

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
    VISUAL_SPEEDOMETER
};

int 
main(int argc, char const *argv[])
{
    int i = 0;
    comm_t comm = {0};
    comm_buffer_t buffer = {0};

    SET_CURRENT_VISUALS(MAIN_TAB);

    RUN_ON_VISUALS(MAIN_TAB, init);

    graphics_init();
    while(1)
    {
        graphics_clear();
        RUN_ON_VISUALS(MAIN_TAB, draw);
        graphics_render();
        usleep(100);
    }
    graphics_terminate();

    RUN_ON_VISUALS(MAIN_TAB, terminate);

    return 0;
}
