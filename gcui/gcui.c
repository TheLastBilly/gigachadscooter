#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>

#include "log.h"
#include "util.h"

#define APP_NAME                            "gcui"

#define HEX_TO_RGB(hex)                     \
(                                           \
    (uint8_t)(0xff & ((hex) >> 24)),        \
    (uint8_t)(0xff & ((hex) >> 16)),        \
    (uint8_t)(0xff & ((hex) >> 8)),         \
    (uint8_t)(0xff & ((hex)))               \
)

#define SDL_ASSETS_PATH                     "assets/"
#define SDL_FONTS_PATH                      SDL_ASSETS_PATH "fonts/"
#define SDL_IMAGE_DEFAULT_FLAGS             (IMG_INIT_JPG | IMG_INIT_PNG)
#define SDL_DEFAULT_FLAGS                   SDL_INIT_VIDEO
#define SDL_DEFAULT_WINDOW_FLAGS            0
#define SDL_DEFAULT_RENDERER_FLAGS          SDL_RENDERER_ACCELERATED

#define SDL_DEFAULT_FONTS_SIZE              18

#define SDL_DEFINE_FONT(_f, _p, _s)         [_f] = (font_request_t){.name = #_f, .path = _p, .size = _s}

#define BACKGROUND_COLOR                    0x181A18

typedef struct ui_t
{
    SDL_Window * window;
    SDL_Renderer * renderer;

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
} ui_t;

static ui_t ui;

typedef enum font_id_t
{
    SDL_FONT_MONOID_28,
} font_id_t;

typedef struct font_request_t
{
    const char *name;
    const char *path;
    int size;
} font_request_t;

const font_request_t FONT_REQUESTS[] = {
    SDL_DEFINE_FONT(SDL_FONT_MONOID_28, "Monoid/Monoid-Regular.ttf", 28),
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
    size = strlen(path) + arrlen(SDL_FONTS_PATH) + strlen(workdir) + 2;
    buf = gcalloc(size, sizeof(char));
    
    snprintf(buf, size, "%s/%s%s", workdir, SDL_FONTS_PATH, path);

    return buf;
}

static int
sdl_initialize( void )
{
    const char * path = NULL;
    int i = 0;
    SDL_DisplayMode dm = {0};

    putenv("DISPLAY=:4.0");

    if(SDL_Init(SDL_DEFAULT_FLAGS) != 0)
        return -1;
    if(IMG_Init(SDL_IMAGE_DEFAULT_FLAGS) != SDL_IMAGE_DEFAULT_FLAGS)
        return -1;

    ui.window = SDL_CreateWindow( APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        10, 10, SDL_DEFAULT_WINDOW_FLAGS);

    ui.screen.index = SDL_GetWindowDisplayIndex(ui.window);
    SDL_GetCurrentDisplayMode(ui.screen.index, &dm);

    ui.screen.height = dm.h;
    ui.screen.width = dm.w;
    
    SDL_SetWindowSize(ui.window, ui.screen.width, ui.screen.height);
    SDL_SetWindowFullscreen(ui.window, SDL_WINDOW_FULLSCREEN);
    
    ui.renderer = SDL_CreateRenderer( ui.window, -1, SDL_DEFAULT_RENDERER_FLAGS );
    if(!ui.renderer)
        return -1;

    if(!ui.window)
        return -1;
    
    if(TTF_Init())
        return -1;

    ui.fonts.len = arrlen(FONT_REQUESTS);
    if(ui.fonts.len > 0)
    {
        ui.fonts.data = gcalloc(sizeof(ui.fonts.data[0]), ui.fonts.len);

        for(i = 0; i < ui.fonts.len; i++)
        {
            path = get_assets_global_path(FONT_REQUESTS[i].path);

            ui.fonts.data[i] = TTF_OpenFont(path, FONT_REQUESTS[i].size);
            if(ui.fonts.data[i] == NULL)
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
sdl_terminate( void )
{
    int i = 0;

    if(ui.renderer)
        SDL_DestroyRenderer(ui.renderer);

    if(ui.window)
        SDL_DestroyWindow(ui.window);
    
    if(ui.fonts.len > 0)
    {
        for(i = 0; i < ui.fonts.len; i++)
            TTF_CloseFont(ui.fonts.data[i]);
        
        ui.fonts.len = 0;

        gfree(ui.fonts.data);
    }

    SDL_Quit();
}

int
sdl_set_window_size( int width, int height )
{
    SDL_SetWindowSize(ui.window, width, height);
    if(ui.renderer)
        SDL_DestroyRenderer(ui.renderer);
    
    ui.renderer = SDL_CreateRenderer( ui.window, -1, SDL_DEFAULT_RENDERER_FLAGS );
    ui.screen.width = width;
    ui.screen.height = height;

    return 0;
}

int
sdl_clear( void )
{
    SDL_RenderClear(ui.renderer);
    return 0;
}

int
sdl_render( void )
{
    SDL_RenderPresent(ui.renderer);
    return 0;
}

int
sdl_set_background_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(ui.renderer, r, g, b, a);
    return 0;
}

int 
main(int argc, char const *argv[])
{
    int ret = 0;

    gmema_init();

    clear(&ui);

    ret = sdl_initialize();
    if(ret < 0)
    {   
        err("cannot initilaize SDL2");
        return -1;
    }

    msleep(5000);
    sdl_terminate();

    return 0;
}
