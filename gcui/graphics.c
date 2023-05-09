#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "graphics.h"
#include "geometry.h"
#include "log.h"

#include "log.h"
#include "util.h"

//#define SDL_HARDCODED_DISPLAY                                   ":1.0"

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
    SDL_Event last_event;

	bool vflip;

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

static sdl_context_t sdl = {0};

const char * _graphics_root = NULL;

const font_request_t FONT_REQUESTS[] = {
    DEFINE_FONT(GRAPHICS_FONT_MONOID_2, "Monoid/Monoid-Regular.ttf", 2),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_4, "Monoid/Monoid-Regular.ttf", 4),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_6, "Monoid/Monoid-Regular.ttf", 6),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_12, "Monoid/Monoid-Regular.ttf", 12),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_18, "Monoid/Monoid-Regular.ttf", 18),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_22, "Monoid/Monoid-Regular.ttf", 22),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_32, "Monoid/Monoid-Regular.ttf", 32),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_48, "Monoid/Monoid-Regular.ttf", 48),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_26, "Monoid/Monoid-Regular.ttf", 26),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_28, "Monoid/Monoid-Regular.ttf", 28),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_64, "Monoid/Monoid-Regular.ttf", 64),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_128, "Monoid/Monoid-Regular.ttf", 128)
};

int
graphics_init( void )
{
    const char * path = NULL;
    int i = 0;
    SDL_DisplayMode dm = {0};

    memset(&sdl, 0, sizeof(sdl));
    gmema_init();

#ifdef SDL_HARDCODED_DISPLAY
    putenv("DISPLAY=" SDL_HARDCODED_DISPLAY);
#endif

    if(SDL_Init(SDL_DEFAULT_FLAGS) != 0)
    {
        err("failed to cinitialize SDL: %s", SDL_GetError());
        return -1;
    }
    if(IMG_Init(SDL_IMAGE_DEFAULT_FLAGS) != SDL_IMAGE_DEFAULT_FLAGS)
    {
        err("failed to cinitialize SDL_Image: %s", SDL_GetError());
        return -1;
    }

    sdl.window = SDL_CreateWindow( APP_NAME, 0, 0,
        640, 480, SDL_DEFAULT_WINDOW_FLAGS);
	
	SDL_ShowCursor(0);
    sdl.screen.index = SDL_GetWindowDisplayIndex(sdl.window);
    SDL_GetCurrentDisplayMode(sdl.screen.index, &dm);

    //sdl.screen.height = dm.h;
    //sdl.screen.width = dm.iw;
    
    //SDL_SetWindowSize(sdl.window, sdl.screen.width, sdl.screen.height);
    //SDL_SetWindowSize(sdl.window, 640, 480);
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN);
    
    SDL_GetWindowSize(sdl.window, &sdl.screen.width, &sdl.screen.height);
    
    sdl.renderer = SDL_CreateRenderer( sdl.window, -1, SDL_DEFAULT_RENDERER_FLAGS );
    if(!sdl.renderer)
    {
        err("failed to create renderer: %s", SDL_GetError());
        return -1;
    }

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
            path = graphics_get_assets_global_path(GRAPHICS_FONTS_PATH, FONT_REQUESTS[i].path);

            sdl.fonts.data[i] = TTF_OpenFont(path, FONT_REQUESTS[i].size);
            if(sdl.fonts.data[i] == NULL)
            {
                err("cannot load font \"%s\" from \"%s\": %s", FONT_REQUESTS[i].name, path,
                    TTF_GetError());
                return -1;
            }
        }
    }

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 

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
graphics_vertical_flip( bool value )
{
	sdl.vflip = value;
	return 0;
}

int
graphics_load_sprite( const char * path, sprite_t * sprite )
{
    static char * buf = NULL;

    size_t size = 0;
    SDL_Surface * surface = NULL;
    SDL_Texture * texture = NULL;
    const char *workdir = NULL;

    if(buf)
    {
        gfree(buf);
        buf = NULL;
    }
	
	if(_graphics_root == NULL)
    	workdir = get_working_directory();
    else
    	workdir = _graphics_root;
    size = 200;
    buf = gcalloc(1, size);
    snprintf(buf, size, "%s/%s/%s", workdir, GRAPHICS_ASSETS_PATH, path);

    surface = IMG_Load(buf);
    if(surface == NULL)
    {
        err("cannot load sprite from path \"%s\": %s", buf, SDL_GetError());
        return -1;
    }

    texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    if(texture == NULL)
    {
        err("cannot create texture from surface: %s", SDL_GetError());
        return -1;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(surface);
    sprite->texture = (void *)texture;

    return 0;
}

int
graphics_free_sprite( sprite_t * sprite )
{
    if(sprite->texture)
        SDL_DestroyTexture((SDL_Texture *)sprite->texture);

    return 0;
}

bool 
graphics_should_close()
{
	return sdl.last_event.type == SDL_QUIT ||
		(sdl.last_event.type == SDL_KEYDOWN && sdl.last_event.key.keysym.sym == SDLK_q);
}

int 
graphics_set_sprite_alpha( sprite_t * sprite, uint8_t alpha )
{
    return SDL_SetTextureAlphaMod((SDL_Texture*)sprite->texture, alpha);
}

int
graphics_draw_sprite( sprite_t * sprite, float w, float h, float x, float y, 
    float t, bool flipx, bool flipy )
{
    SDL_Rect rect = {0};
    SDL_RendererFlip flip = 0;

    if(flipx)
        flip |= SDL_FLIP_HORIZONTAL;
    if(flipy)
        flip |= SDL_FLIP_VERTICAL;

    rect = (SDL_Rect){ 
        .w = (int)(w * (float)sdl.screen.width),
        .h = (int)(h * (float)sdl.screen.height),

        .x = (int)(x * (float)sdl.screen.width),
        .y = (int)(y * (float)sdl.screen.height)
    };

    if(sprite->texture)
        return SDL_RenderCopyEx(sdl.renderer, (SDL_Texture *)sprite->texture, NULL,
            &rect, t, NULL, flip);
    
    return 0;
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

    surface = TTF_RenderUTF8_Blended(sdl.fonts.data[font], text, color);
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
    SDL_SetTextureBlendMode(sdl.text.texture, SDL_BLENDMODE_BLEND);

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
graphics_listen_for_events( void )
{
    SDL_Event event = {};

    SDL_PollEvent(&event);
    sdl.last_event = event;
    return 0;
}

int
graphics_render( void )
{    
	if(sdl.vflip)
	{
		SDL_Surface *surface = SDL_CreateRGBSurface(0, sdl.screen.width, sdl.screen.height, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(sdl.renderer, NULL, SDL_PIXELFORMAT_ARGB8888, 
			surface->pixels, surface->pitch);
		
    	SDL_Texture * texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    	if(texture == NULL)
    		return 1;
    	
    	SDL_FreeSurface(surface);
    	
    	SDL_Rect rect = {0};
	    rect.h = sdl.screen.height;
	    rect.w = sdl.screen.width;
	    rect.y = rect.x = 0;
    	SDL_RenderCopyEx(sdl.renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);
    	SDL_DestroyTexture(texture);
	}
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

int
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
        aapolygonRGBA(sdl.renderer, vx, vy, 3, color.r, 
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
