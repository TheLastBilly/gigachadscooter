#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdint.h>

#define APP_NAME                            "gcui"

#define GRAPHICS_ASSETS_PATH                "assets/"
#define GRAPHICS_FONTS_PATH                 GRAPHICS_ASSETS_PATH "fonts/"

#define GRAPHICS_HEX2RGBA(hex)              \
(rgba_t){                                   \
    .r = (uint8_t)(0xff & ((hex) >> 24)),   \
    .g = (uint8_t)(0xff & ((hex) >> 16)),   \
    .b = (uint8_t)(0xff & ((hex) >> 8)),    \
    .a = (uint8_t)(0xff & ((hex)))          \
}

typedef enum font_id_t
{
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
    void * data;
} sprite_t;

typedef struct polygon_t
{
    struct
    {
        float x, y;
    } vertices[3];
} polygon_t;

int graphics_init( void );
int graphics_terminate( void );
int graphics_set_window_size( int width, int height );
int graphics_get_window_size( int *width, int *height );
int graphics_draw_text(font_id_t font, float x, float y, rgba_t color, const char * text);
int graphics_clear( void );
int graphics_render( void );
int graphics_set_background_color(rgba_t color);
int graphics_load_sprite( const char * path, sprite_t * sprite);
int graphics_render_sprite( float x, float y, float w, float h, sprite_t * sprite );
int graphics_draw_polygons( polygon_t * polygons, int len, rgba_t color );
int graphics_rotate_polygons( polygon_t * polygons, int len, float t );
int graphics_translate_polygons( polygon_t * polygons, int len, float x, float y );

#endif