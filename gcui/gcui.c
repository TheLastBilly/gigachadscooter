#include "graphics.h"
#include "log.h"
#include "util.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "visuals/visual.h"

// #define BACKGROUND_COLOR                            GRAPHICS_HEX2RGBA(0x0c120aff)
#define BACKGROUND_COLOR                            GRAPHICS_HEX2RGBA(0x222222ff)

#define POST_INTRO_WAIT                             1000
#define MAIN_THREAD_WAIT                            (1000/30)

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
main(int argc, char *argv[])
{
	int opt = 0;
    int i = 0, ret = 0;
    bool should_clear = true, clear_requested = false, vflip = false;
    uint32_t ticks = 0, delta = 0;

    gmema_init();
   	
   	while((opt = getopt(argc, argv, "fr:")) != -1)
   	{
   		switch(opt)
   		{
   		case 'f':
            vflip = true;
   			break;
   		case 'r':
   			graphics_set_root(optarg);
   			break;
   		default:
   			break;
   		}
   	}

    ret = graphics_init();
    if(ret != 0)
    {
        err("failed to initialize graphics");
        return ret;
    }

    if(vflip)
        graphics_vertical_flip(true);

    graphics_set_background_color(BACKGROUND_COLOR);

    // play_intro();

    graphics_clear();

    SET_CURRENT_VISUALS(MAIN_TAB);
    RUN_ON_VISUALS(MAIN_TAB, init);

    signal(SIGINT, signal_handler);

    ticks = 0;
    while(keep_running && !graphics_should_close())
    {
        graphics_listen_for_events();
        
        // should_clear = clear_requested && !should_clear;

        delta = (graphics_millis() - ticks);
        if (delta < MAIN_THREAD_WAIT) {
            graphics_msleep(1);
            continue;
        }

        graphics_clear();
        
        clear_requested = false;
        for(i = 0; i < visuals.len; i++)
        {
            ret = visual_get_visual(visuals.IDs[i])->draw(should_clear);
            if(ret)
                clear_requested = true;
        }
        
        ticks = graphics_millis();
        graphics_render();

    }


    graphics_terminate();

    RUN_ON_VISUALS(MAIN_TAB, terminate);

    return 0;
}
