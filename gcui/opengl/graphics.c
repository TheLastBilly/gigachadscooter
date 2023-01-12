#include <GLFW/glfw3.h>

#include "graphics.h"
#include "geometry.h"
#include "log.h"

#include "log.h"
#include "util.h"

#define GL_HARDCODED_DISPLAY                                    ":1.0"

// #define GL_IMAGE_DEFAULT_FLAGS                                  (IMG_INIT_JPG | IMG_INIT_PNG)
#define GL_DEFAULT_FLAGS                                        (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH)


#define GL_DEFAULT_FONTS_SIZE                                   18

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

typedef struct gl_context_t
{
    GLFWwindow * window;
    // SDL_Renderer * renderer;
    rgba_t background_color;

    struct 
    {
        int width;
        int height;
        int index;
    } screen;

    // struct
    // {
    //     TTF_Font ** data;
    //     size_t len;
    // } fonts;

    // struct
    // {
    //     SDL_Texture * texture;

    //     int width;
    //     int height;
    // } text;
} gl_context_t;

typedef struct font_request_t
{
    const char *name;
    const char *path;
    int size;
} font_request_t;

static gl_context_t gl;

const font_request_t FONT_REQUESTS[] = {
    DEFINE_FONT(GRAPHICS_FONT_MONOID_12, "Monoid/Monoid-Regular.ttf", 12),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_18, "Monoid/Monoid-Regular.ttf", 18),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_22, "Monoid/Monoid-Regular.ttf", 22),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_26, "Monoid/Monoid-Regular.ttf", 26),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_28, "Monoid/Monoid-Regular.ttf", 28),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_64, "Monoid/Monoid-Regular.ttf", 64),
    DEFINE_FONT(GRAPHICS_FONT_MONOID_128, "Monoid/Monoid-Regular.ttf", 128)
};

int
graphics_init( void )
{
    int i = 0, ret = 0;
    const char * path = NULL;

    memset(&gl, 0, sizeof(gl));
    gmema_init();

#ifdef SDL_HARDCODED_DISPLAY
    putenv("DISPLAY=" SDL_HARDCODED_DISPLAY);
#endif

    if(!glfwInit())
        return -1;

    glEnable(GL_MULTISAMPLE); 
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gl.window = glfwCreateWindow(640, 480, "gcui", NULL, NULL);
    if(!gl.window)
        return -1;
    glfwGetWindowSize(gl.window, &gl.screen.width, &gl.screen.height);
    glfwMakeContextCurrent(gl.window);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // gl.fonts.len = arrlen(FONT_REQUESTS);
    // if(gl.fonts.len > 0)
    // {
    //     gl.fonts.data = gcalloc(sizeof(gl.fonts.data[0]), gl.fonts.len);

    //     for(i = 0; i < gl.fonts.len; i++)
    //     {
    //         path = get_assets_global_path(FONT_REQUESTS[i].path);

    //         gl.fonts.data[i] = TTF_OpenFont(path, FONT_REQUESTS[i].size);
    //         if(gl.fonts.data[i] == NULL)
    //         {
    //             err("cannot load font \"%s\" from \"%s\": %s", FONT_REQUESTS[i].name, path,
    //                 TTF_GetError());
    //             return -1;
    //         }
    //     }
    // }

    return 0;
}

int
graphics_terminate( void )
{
    glfwTerminate();

    return 0;
    // if(gl.text.texture)
    // {
    //     SDL_DestroyTexture(gl.text.texture);
    //     gl.text.texture = NULL;
    // }

    // if(gl.fonts.len > 0)
    // {
    //     for(i = 0; i < gl.fonts.len; i++)
    //         TTF_CloseFont(gl.fonts.data[i]);
        
    //     gl.fonts.len = 0;

    //     gfree(gl.fonts.data);
    // }

    // TTF_Quit();

    // if(gl.renderer)
    //     SDL_DestroyRenderer(gl.renderer);

    // if(gl.window)
    //     SDL_DestroyWindow(gl.window);

    // SDL_Quit();
}

int
graphics_load_sprite( const char * path, sprite_t * sprite )
{
    // static char * buf = NULL;

    // size_t size = 0;
    // SDL_Surface * surface = NULL;
    // SDL_Texture * texture = NULL;
    // const char *workdir = NULL;

    // if(buf)
    // {
    //     gfree(buf);
    //     buf = NULL;
    // }

    // workdir = get_working_directory();
    // size = strlen(GRAPHICS_ASSETS_PATH) + strlen(path) + 
    //     strlen(workdir) + 3;
    // buf = gcalloc(1, size);
    // snprintf(buf, size, "%s/%s/%s", workdir, GRAPHICS_ASSETS_PATH, path);

    // surface = IMG_Load(buf);
    // if(surface == NULL)
    // {
    //     err("cannot load sprite from path \"%s\": %s", buf, SDL_GetError());
    //     return -1;
    // }

    // texture = SDL_CreateTextureFromSurface(gl.renderer, surface);
    // if(texture == NULL)
    // {
    //     err("cannot create texture from surface: %s", SDL_GetError());
    //     return -1;
    // }
    // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // SDL_FreeSurface(surface);
    // sprite->texture = (void *)texture;

    return 0;
}

int
graphics_free_sprite( sprite_t * sprite )
{
    // if(sprite->texture)
    //     SDL_DestroyTexture((SDL_Texture *)sprite->texture);

    return 0;
}


int 
graphics_set_sprite_alpha( sprite_t * sprite, uint8_t alpha )
{
    return 0;
    // return SDL_SetTextureAlphaMod((SDL_Texture*)sprite->texture, alpha);
}

int
graphics_draw_sprite( sprite_t * sprite, float w, float h, float x, float y, 
    float t, bool flipx, bool flipy )
{
    // SDL_Rect rect = {0};
    // SDL_RendererFlip flip = 0;

    // if(flipx)
    //     flip |= SDL_FLIP_HORIZONTAL;
    // if(flipy)
    //     flip |= SDL_FLIP_VERTICAL;

    // rect = (SDL_Rect){ 
    //     .w = (int)(w * (float)gl.screen.width),
    //     .h = (int)(h * (float)gl.screen.height),

    //     .x = (int)(x * (float)gl.screen.width),
    //     .y = (int)(y * (float)gl.screen.height)
    // };

    // if(sprite->texture)
    //     return SDL_RenderCopyEx(gl.renderer, (SDL_Texture *)sprite->texture, NULL,
    //         &rect, t, NULL, flip);
    
    return 0;
}

int
graphics_set_window_size( int width, int height )
{
    glfwSetWindowSize(gl.window, 640, 480);
    graphics_get_window_size(&gl.screen.width, &gl.screen.height);

    return 0;
}

int 
graphics_get_window_size( int *width, int *height )
{
    glfwGetWindowSize(gl.window, width, height);
    return 0;
}

int
graphics_draw_text( font_id_t font, float xp, float yp, rgba_t rgba, const char * text )
{
    int ret = 0;
    // SDL_Rect rect = {0};
    // SDL_Surface * surface = NULL;
    // SDL_Color color = {0};
    // int x = 0, y = 0;

    // color = RGBA2SDLCOLOR(rgba);

    // if(strlen(text) < 1)
    //     return 0;

    // if(font >= _GRAPHICS_FONT_END || font < 0)
    // {
    //     err("unknown font (%d)", font);
    //     return -1;
    // }

    // surface = TTF_RenderUTF8_Blended(gl.fonts.data[font], text, color);
    // if(!surface)
    // {
    //     err("cannot create text surface: %s", TTF_GetError());
    //     return -1;
    // }

    // if(gl.text.texture)
    //     SDL_DestroyTexture(gl.text.texture);
    
    // gl.text.texture = SDL_CreateTextureFromSurface(gl.renderer, surface);
    // if(!gl.text.texture)
    // {
    //     err("cannot create text texture: %s", SDL_GetError());
    //     return -1;
    // }
    // SDL_SetTextureBlendMode(gl.text.texture, SDL_BLENDMODE_BLEND);

    // gl.text.width = surface->w;
    // gl.text.height = surface->h;

    // SDL_FreeSurface(surface);

    // rect.h = gl.text.height;
    // rect.w = gl.text.width;

    // x = xp;
    // y = yp;
    
    // rect.x = x < 0 ? gl.screen.width/2 - rect.w/2 : x;
    // rect.y = y < 0 ? gl.screen.height/2 - rect.h/2 : y;
    
    // SDL_RenderCopyEx(gl.renderer, gl.text.texture, NULL, &rect, 0, NULL, 0);

    return ret;
}

int
graphics_clear( void )
{
    float ratio = 0.0;

    ratio = (float)gl.screen.width / (float)gl.screen.height;

    glViewport(0, 0, gl.screen.width, gl.screen.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    graphics_update_background();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 1.f, -0.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);

    return 0;
}

int
graphics_listen_for_events( void )
{
    glfwPollEvents();
    return 0;
}

int
graphics_render( void )
{
    glfwSwapBuffers(gl.window);
    return 0;
}

int
graphics_set_background_color( rgba_t rgba )
{
    gl.background_color = rgba;
    glClearColor(rgba.r/255.f, rgba.g/255.f, rgba.b/255.f, rgba.a/255.f);
    return 0;
}

int
graphics_update_background()
{
    graphics_set_background_color(gl.background_color);
    return 0;
}

int
graphics_draw_polygons( polygon_t * polygons, int len, rgba_t color )
{
    int i = 0;
    float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    GLfloat points[9] = {0};

    if(len < 0)
        return -1;

    r = color.r/255.0f;
    g = color.g/255.0f;
    b = color.b/255.0f;
    a = color.a/255.0f;

    for(i = 0; i < len; i++)
    {
        points[0] = polygons[i].vertices[0].x;
        points[1] = polygons[i].vertices[0].y;
        points[2] = 0.0f;

        points[3] = polygons[i].vertices[1].x;
        points[4] = polygons[i].vertices[1].y;
        points[5] = 0.0f;

        points[6] = polygons[i].vertices[2].x;
        points[7] = polygons[i].vertices[2].y;
        points[8] = 0.0f;

        glBegin(GL_TRIANGLES);
        glColor4f(r, g, b, a);
        glVertex3f(points[0], points[1], points[2]);
        glColor4f(r, g, b, a);
        glVertex3f(points[3], points[4], points[5]);
        glColor4f(r, g, b, a);
        glVertex3f(points[6], points[7], points[9]);
        glEnd();
    }

    // graphics_update_background();

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
    return (uint32_t)(glfwGetTime()/1000.0);
}

void
graphics_msleep( uint32_t sleep )
{
    msleep(sleep);
}