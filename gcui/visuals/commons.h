#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <stdlib.h>
#include <string.h>

#include "geometry.h"
#include "graphics.h"

typedef rgba_t (*comms_bars_color_cb_t)(float t, float value, int bar);

typedef struct commons_bars_t
{
    uint32_t max;

    float w; 
    float h;

    float radius;

    int max_bars;

    bool ratio;

    comms_bars_color_cb_t color_cb;
} commons_bars_t;

static void
draw_radius_bars(uint32_t value, commons_bars_t * cbars, rgba_t color)
{
    static const polygon_t TEMPLATE_BAR[] = 
    {
        (polygon_t) {
            .vertices[0].x = -0.5,
            .vertices[1].x = -0.5,
            .vertices[2].x = 0.5,

            .vertices[0].y = -0.5,
            .vertices[1].y = 0.5,
            .vertices[2].y = 0.5,
        },

        (polygon_t) {
            .vertices[0].x = 0.5,
            .vertices[1].x = 0.5,
            .vertices[2].x = -0.5,

            .vertices[0].y = 0.5,
            .vertices[1].y = -0.5,
            .vertices[2].y = -0.5,
        },
    };

    int i = 0, bars = 0, screen_width = 0, screen_height = 0;
    float t = 0.0f, d = 0.0f, dt = 0.0f, ratio = 0.0;
    polygon_t bar[2] = {0}, base_bar[2] = {0};
    
    d = cbars->max/cbars->max_bars;
    bars = (int)roundf(value/d);
    dt = 180.0/cbars->max_bars;

    if(cbars->ratio)
    {
        graphics_get_window_size(&screen_width, &screen_height);
        ratio = (float)screen_width/(float)screen_height;
    }
    else
        ratio = 1.0;

    memcpy(base_bar, TEMPLATE_BAR, sizeof(TEMPLATE_BAR));
    graphics_scale_polygons(base_bar, 2, cbars->w, cbars->h);

    t = -dt;
    for(i = 0; i <= bars; i++)
    {
        t += dt;

        memcpy(bar, base_bar, sizeof(bar));

        if(cbars->color_cb)
            color = cbars->color_cb(t, value, i);
        
        graphics_translate_polygons(bar, 2, -cbars->radius, 0);
        graphics_rotate_polygons(bar, 2, t);
        graphics_scale_polygons(bar, 2, 1.0, ratio);
        graphics_translate_polygons(bar, 2, .5, 1.0);
        graphics_draw_polygons(bar, 2, color);
    }
}   

#endif