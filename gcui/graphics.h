#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

#define APP_NAME                            "gcui"

#define GRAPHICS_ASSETS_PATH                "assets/"
#define GRAPHICS_FONTS_PATH                 GRAPHICS_ASSETS_PATH "fonts/"
#define GRAPHICS_SHADERS_PATH               GRAPHICS_ASSETS_PATH "shaders/"

#define GRAPHICS_HEX2RGBA(hex)              \
(rgba_t){                                   \
    .r = (uint8_t)(0xff & ((hex) >> 24)),   \
    .g = (uint8_t)(0xff & ((hex) >> 16)),   \
    .b = (uint8_t)(0xff & ((hex) >> 8)),    \
    .a = (uint8_t)(0xff & ((hex)))          \
}

typedef enum font_id_t
{
    GRAPHICS_FONT_MONOID_12,
    GRAPHICS_FONT_MONOID_18,
    GRAPHICS_FONT_MONOID_22,
    GRAPHICS_FONT_MONOID_26,
    GRAPHICS_FONT_MONOID_64,
    GRAPHICS_FONT_MONOID_28,
    GRAPHICS_FONT_MONOID_128,

    _GRAPHICS_FONT_END
} font_id_t;

typedef struct rgba_t
{
    uint8_t r, g, b, a;
} rgba_t;

typedef struct sprite_t
{
    void * texture;
} sprite_t;

typedef struct polygon_t
{
    struct
    {
        float x, y;
    } vertices[3];
} polygon_t;


static inline const char *
graphics_get_assets_global_path( const char * relative, const char * path )
{
    size_t size = 0;
    const char * workdir = NULL;
    static char * buf = NULL;

    if(buf)
    {
        gfree(buf);
        buf = NULL;
    }

    workdir = get_working_directory();
    size = strlen(path) + strlen(relative) + strlen(workdir) + 2;
    buf = gcalloc(size, sizeof(char));
    
    snprintf(buf, size, "%s/%s%s", workdir, relative, path);

    return buf;
}

int graphics_init( void );
int graphics_terminate( void );

int graphics_load_sprite( const char * path, sprite_t * sprite );
int graphics_free_sprite( sprite_t * sprite );
int graphics_set_sprite_alpha( sprite_t * sprite, uint8_t alpha );
int graphics_draw_sprite( sprite_t * sprite, float w, float h, float x, float y, float t, bool flipx, bool flipy );

int graphics_set_window_size( int width, int height );
int graphics_get_window_size( int *width, int *height );
int graphics_draw_text(font_id_t font, float x, float y, rgba_t color, const char * text);
int graphics_clear( void );
int graphics_listen_for_events( void );
int graphics_render( void );
int graphics_set_background_color(rgba_t color);
int graphics_update_background();
int graphics_load_sprite( const char * path, sprite_t * sprite);
int graphics_render_sprite( float x, float y, float w, float h, sprite_t * sprite );
int graphics_draw_polygons( polygon_t * polygons, int len, rgba_t color );

int graphics_rotate_polygons( polygon_t * polygons, int len, float t );
int graphics_translate_polygons( polygon_t * polygons, int len, float x, float y );
int graphics_scale_polygons( polygon_t * polygons, int len, float w, float h );

uint32_t graphics_millis( void );
void graphics_msleep( uint32_t sleep );

#endif