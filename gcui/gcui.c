#include "graphics.h"
#include "log.h"
#include "util.h"
#include "comm.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL.h>

#include "visuals/visual.h"

// #define BACKGROUND_COLOR                            GRAPHICS_HEX2RGBA(0x0c120aff)
#define BACKGROUND_COLOR                            GRAPHICS_HEX2RGBA(0x222222ff)

#define POST_INTRO_WAIT                             1000
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

static volatile bool keep_running = true;
static void
signal_handler(int sig)
{
    keep_running = false;
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
    VISUAL_TIME,
    VISUAL_TEMPERATURE,
    VISUAL_BLINKERS
};

extern void play_intro( void );

int 
main(int argc, char const *argv[])
{
    int i = 0, ret = 0;
    uint32_t ticks = 0, delta = 0;
    comm_t comm = {0};
    comm_buffer_t buffer = {0};

    gmema_init();

    ret = graphics_init();
    if(ret != 0)
    {
        err("failed to initialize graphics");
        return ret;
    }

    graphics_set_background_color(BACKGROUND_COLOR);

    // play_intro();

    graphics_clear();

    SET_CURRENT_VISUALS(MAIN_TAB);
    RUN_ON_VISUALS(MAIN_TAB, init);

    signal(SIGINT, signal_handler);

    while(keep_running)
    {
        ticks = graphics_millis();

        graphics_clear();
        RUN_ON_VISUALS(MAIN_TAB, draw);
        graphics_render();

        delta = graphics_millis() - ticks;
        if (delta < MAIN_THREAD_WAIT) {
            graphics_msleep(MAIN_THREAD_WAIT);
        }
    }

    graphics_terminate();

    RUN_ON_VISUALS(MAIN_TAB, terminate);

    return 0;
}
