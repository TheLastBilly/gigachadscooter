#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

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
    GRAPHICS_FONT_MONOID_28,

    _GRAPHICS_FONT_END
} font_id_t;

typedef struct rgba_t
{
    uint8_t r, g, b, a;
} rgba_t;

int graphics_initialize( void );
int graphics_terminate( void );
int graphics_set_window_size( int width, int height );
int graphics_draw_text(font_id_t font, int x, int y, rgba_t color, const char * text);
int graphics_clear( void );
int graphics_render( void );
int graphics_set_background_color(rgba_t color);

#endif