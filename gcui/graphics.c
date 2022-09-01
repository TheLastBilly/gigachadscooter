#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "graphics.h"
#include "geometry.h"

#include "log.h"
#include "util.h"

#define SDL_HARDCODED_DISPLAY                                   ":4.0"

#define SDL_IMAGE_DEFAULT_FLAGS                                 (IMG_INIT_JPG | IMG_INIT_PNG)
#define SDL_DEFAULT_FLAGS                                       SDL_INIT_VIDEO
#define SDL_DEFAULT_WINDOW_FLAGS                                0
#define SDL_DEFAULT_RENDERER_FLAGS                              SDL_RENDERER_ACCELERATED

#define SDL_DEFAULT_FONTS_SIZE                                  18

#define DEFINE_FONT(_f, _p, _s)                                 [_f] = (font_request_t){.name = #_f, .path = _p, .size = _s}

#define RGBA2SDLCOLOR(c)                                        (SDL_Color){.r = c.r, .g = c.g, .b = c.b, .a = c.a}
#define SDLCOLOR2RGBA(c)                                        (rgba_t){.r = c.r, .g = c.g, .b = c.b, .a = c.a}

#define TRANSFORM_POLYGONS(_p, _l, _func, ...)                  \
{                                                               \
    int i = 0, s = 0;                                           \
    vertex2d_t v = {0}, r = {0};                                \
                                                                \
    for(i = 0; i < len; i++)                                    \
    {                                                           \
        for(s = 0; s < arrlen(polygons[0].vertices); s++)       \
        {                                                       \
            point2d(polygons[i].vertices[s].x,                  \
                polygons[i].vertices[s].y, v);                  \
            _func(v, __VA_ARGS__, r);                           \
            polygons[i].vertices[s].x = r[0];                   \
            polygons[i].vertices[s].y = r[1];                   \
        }                                                       \
    }                                                           \
}

typedef struct sdl_context_t
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Color background_color;

    struct 
    {
        int width;
        int height;
        int index;
    } screen;

    struct
    {
        TTF_Font ** data;
        size_t len;
    } fonts;

    struct
    {
        SDL_Texture * texture;

        int width;
        int height;
    } text;
} sdl_context_t;

typedef struct font_request_t
{
    const char *name;
    const char *path;
    int size;
} font_request_t;

static sdl_context_t sdl;

const font_request_t FONT_REQUESTS[] = {
    DEFINE_FONT(GRAPHICS_FONT_MONOID_28, "Monoid/Monoid-Regular.ttf", 28),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_64, "Monoid/Monoid-Regular.ttf", 64),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_128, "Monoid/Monoid-Regular.ttf", 128),
};

static const char *
get_assets_global_path( const char * path )
{
    size_t size = 0;
    const char * workdir = NULL;
    static char * buf = NULL;

    if(buf)
        gfree(buf);

    workdir = get_working_directory();
    size = strlen(path) + arrlen(GRAPHICS_FONTS_PATH) + strlen(workdir) + 2;
    buf = gcalloc(size, sizeof(char));
    
    snprintf(buf, size, "%s/%s%s", workdir, GRAPHICS_FONTS_PATH, path);

    return buf;
}

int
graphics_init( void )
{
    const char * path = NULL;
    int i = 0;
    SDL_DisplayMode dm = {0};

    clear(&sdl);
    gmema_init();

#ifdef SDL_HARDCODED_DISPLAY
    putenv("DISPLAY=" SDL_HARDCODED_DISPLAY);
#endif

    if(SDL_Init(SDL_DEFAULT_FLAGS) != 0)
        return -1;
    if(IMG_Init(SDL_IMAGE_DEFAULT_FLAGS) != SDL_IMAGE_DEFAULT_FLAGS)
        return -1;

    sdl.window = SDL_CreateWindow( APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        10, 10, SDL_DEFAULT_WINDOW_FLAGS);

    sdl.screen.index = SDL_GetWindowDisplayIndex(sdl.window);
    SDL_GetCurrentDisplayMode(sdl.screen.index, &dm);

    sdl.screen.height = dm.h;
    sdl.screen.width = dm.w;
    
    SDL_SetWindowSize(sdl.window, sdl.screen.width, sdl.screen.height);
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN);
    
    sdl.renderer = SDL_CreateRenderer( sdl.window, -1, SDL_DEFAULT_RENDERER_FLAGS );
    if(!sdl.renderer)
        return -1;

    if(!sdl.window)
        return -1;
    
    if(TTF_Init())
        return -1;

    sdl.fonts.len = arrlen(FONT_REQUESTS);
    if(sdl.fonts.len > 0)
    {
        sdl.fonts.data = gcalloc(sizeof(sdl.fonts.data[0]), sdl.fonts.len);

        for(i = 0; i < sdl.fonts.len; i++)
        {
            path = get_assets_global_path(FONT_REQUESTS[i].path);

            sdl.fonts.data[i] = TTF_OpenFont(path, FONT_REQUESTS[i].size);
            if(sdl.fonts.data[i] == NULL)
            {
                err("cannot load font \"%s\" from \"%s\": %s", FONT_REQUESTS[i].name, path,
                    TTF_GetError());
                return -1;
            }
        }
    }

    return 0;
}

int
graphics_terminate( void )
{
    int i = 0;

    if(sdl.text.texture)
    {
        SDL_DestroyTexture(sdl.text.texture);
        sdl.text.texture = NULL;
    }

    if(sdl.fonts.len > 0)
    {
        for(i = 0; i < sdl.fonts.len; i++)
            TTF_CloseFont(sdl.fonts.data[i]);
        
        sdl.fonts.len = 0;

        gfree(sdl.fonts.data);
    }

    TTF_Quit();

    if(sdl.renderer)
        SDL_DestroyRenderer(sdl.renderer);

    if(sdl.window)
        SDL_DestroyWindow(sdl.window);

    SDL_Quit();
}

int
graphics_set_window_size( int width, int height )
{
    SDL_SetWindowSize(sdl.window, width, height);
    if(sdl.renderer)
        SDL_DestroyRenderer(sdl.renderer);
    
    sdl.renderer = SDL_CreateRenderer( sdl.window, -1, SDL_DEFAULT_RENDERER_FLAGS );
    sdl.screen.width = width;
    sdl.screen.height = height;

    return 0;
}

int 
graphics_get_window_size( int *width, int *height )
{
    if(width)
        *width = sdl.screen.width;
    if(height)
        *height = sdl.screen.height;
    
    return 0;
}

int
graphics_draw_text( font_id_t font, float xp, float yp, rgba_t rgba, const char * text )
{
    int ret = 0;
    SDL_Rect rect = {0};
    SDL_Surface * surface = NULL;
    SDL_Color color = {0};
    int x = 0, y = 0;

    color = RGBA2SDLCOLOR(rgba);

    if(strlen(text) < 1)
        return 0;

    if(font >= _GRAPHICS_FONT_END || font < 0)
    {
        err("unknown font (%d)", font);
        return -1;
    }

    surface = TTF_RenderText_Solid(sdl.fonts.data[font], text, color);
    if(!surface)
    {
        err("cannot create text surface: %s", TTF_GetError());
        return -1;
    }

    if(sdl.text.texture)
        SDL_DestroyTexture(sdl.text.texture);
    
    sdl.text.texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    if(!sdl.text.texture)
    {
        err("cannot create text texture: %s", SDL_GetError());
        return -1;
    }

    sdl.text.width = surface->w;
    sdl.text.height = surface->h;

    SDL_FreeSurface(surface);

    rect.h = sdl.text.height;
    rect.w = sdl.text.width;

    x = xp * sdl.screen.width;
    y = yp * sdl.screen.height;
    
    rect.x = x < 0 ? sdl.screen.width/2 - rect.w/2 : x;
    rect.y = y < 0 ? sdl.screen.height/2 - rect.h/2 : y;
    
    SDL_RenderCopyEx(sdl.renderer, sdl.text.texture, NULL, &rect, 0, NULL, 0);

    return ret;
}

int
graphics_clear( void )
{
    SDL_RenderClear(sdl.renderer);
    return 0;
}

int
graphics_render( void )
{
    SDL_RenderPresent(sdl.renderer);
    return 0;
}

int
graphics_set_background_color( rgba_t rgba )
{
    sdl.background_color = RGBA2SDLCOLOR(rgba);
    SDL_SetRenderDrawColor(sdl.renderer, sdl.background_color.r, sdl.background_color.g,
        sdl.background_color.b,  sdl.background_color.a);
    return 0;
}

static int
graphics_update_background()
{
    graphics_set_background_color(SDLCOLOR2RGBA(sdl.background_color));
    return 0;
}

int
graphics_draw_polygons( polygon_t * polygons, int len, rgba_t color )
{
    int i = 0;
    Sint16 vx[3] = {0}, vy[3] = {0};

    if(len < 0)
        return -1;

    for(i = 0; i < len; i++)
    {
        vx[0] = polygons[i].vertices[0].x * sdl.screen.width;
        vy[0] = polygons[i].vertices[0].y * sdl.screen.height;

        vx[1] = polygons[i].vertices[1].x * sdl.screen.width;
        vy[1] = polygons[i].vertices[1].y * sdl.screen.height;

        vx[2] = polygons[i].vertices[2].x * sdl.screen.width;
        vy[2] = polygons[i].vertices[2].y * sdl.screen.height;
        
        filledPolygonRGBA(sdl.renderer, vx, vy, 3, color.r, 
            color.g, color.b, color.a);
    }

    graphics_update_background();

    return 0;
}

int
graphics_rotate_polygons( polygon_t * polygons, int len, float t )
{
    TRANSFORM_POLYGONS(polygons, len, rotate_vertex, t);
    return 0;
}

int
graphics_translate_polygons( polygon_t * polygons, int len, float x, float y )
{
    TRANSFORM_POLYGONS(polygons, len, translate_vertex, x, y);
    return 0;
}

int 
graphics_scale_polygons( polygon_t * polygons, int len, float w, float h )
{
    TRANSFORM_POLYGONS(polygons, len, scale_vertex, w, h);
    return 0;
}

uint32_t
graphics_millis( void )
{
    return SDL_GetTicks();
}

void
graphics_msleep( uint32_t sleep )
{
    SDL_Delay(sleep);
}