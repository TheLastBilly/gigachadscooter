#include "graphics.h"

#include "util.h"

#include <stdlib.h>

#define INTRO_LOGO                                  "images/gigachadguy.png"
#define INTRO_FADE_DELAY                            5
#define INTRO_REST_DELAY                            1000
#define INTRO_ALPHA_STEP                            5
#define INTRO_MAX_ALPHA                             (255/INTRO_ALPHA_STEP)
#define INTRO_BACKGROUND                            GRAPHICS_HEX2RGBA(0x222222ff)

void
play_intro( void )
{
    uint32_t last_update = 0, delta = 0;
    int screen_width = 0, screen_height = 0;
    float w = 0.0, h = 0.0, x = 0.0, y = 0.0;
    int ret = 0, i = 0, loop = 0;
    sprite_t sprite = {0};

    ret = graphics_load_sprite(INTRO_LOGO, &sprite);
    if(ret < 0)
        abort();

    graphics_set_background_color(INTRO_BACKGROUND);

    graphics_get_window_size(&screen_width, &screen_height);
    h = 1.0;
    w = (float)screen_height/(float)screen_width;
    x = ((float)screen_height/(float)screen_width)/2 - w/2 + 0.25;
    y = 0.0;
    
    while(true)
    {
        last_update = graphics_millis();
        graphics_set_sprite_alpha(&sprite, min(INTRO_MAX_ALPHA, i*INTRO_ALPHA_STEP));

        graphics_clear();
        graphics_draw_sprite(&sprite, w, h, x, y, 0, 0, 0);
        graphics_render();

        delta = graphics_millis() - last_update;
        if(delta < INTRO_FADE_DELAY)
            graphics_msleep(INTRO_FADE_DELAY - delta);

        if(loop == 0)
        {
            if(i >= INTRO_MAX_ALPHA)
            {
                loop += 1;
                graphics_msleep(INTRO_REST_DELAY);
            }
            else
                i += 1;
        }
        else
        {
            if(i <= 0)
                break;
            else
                i -= 1;
        }
    }

    graphics_free_sprite(&sprite);
}