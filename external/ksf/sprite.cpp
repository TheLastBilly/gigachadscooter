#include "graphics/sprite.hpp"

using namespace graphics;

sprite::sprite(const std::string &name, const std::string &path, SDL_Renderer * renderer): 
    node(name), 
    path(path),
    renderer(renderer)
{}

void sprite::load()
{
    unload();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL)
        throw_exception_with_msg(texture_loading_error, IMG_GetError());

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL || SDL_QueryTexture(texture, NULL, NULL, &width, &height) != 0)
        throw_exception_with_msg(texture_loading_error, SDL_GetError());

    defaultHeight = height;
    defaultWidth = width;

}

void sprite::render(int x, int y) const
{
    SDL_Rect source, destination;

    if (texture == NULL)
        throw_exception_without_msg(null_texture_error);

    source.h = defaultWidth;
    source.w = defaultHeight;
    
    destination.h = height;
    destination.w = width;

    source.x = source.y = 0;
    destination.x = x;
    destination.y = y;

    if(SDL_RenderCopyEx((SDL_Renderer *)renderer, texture, &source, &destination, 0.0, NULL, currentFlip) != 0)
        throw_exception_with_msg(sprite_rendering_error, SDL_GetError());
}