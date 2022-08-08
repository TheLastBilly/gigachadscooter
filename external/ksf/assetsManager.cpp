#include "engine/managers/assetsManager.hpp"

register_logger();

#if WIN32
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>

#ifdef WIN32
#define validatePath(path)                                                                      \
    if(path.size() < 1)                                                                         \
        throw_exception_with_msg(utilities::invalid_path_error, "file path cannot be empty");   \
                                                                                                \
    if(path[path.size()-1] != '\\')                                                             \
        path += "\\";                                                                           \
    if(!std::experimental::filesystem::exists(path))                                            \
        throw_exception_with_msg(utilities::invalid_path_error, "\"" + path + "\" does not exists");
#else
#define validatePath(path)                                                                      \
    if(path.size() < 1)                                                                         \
        throw_exception_with_msg(utilities::invalid_path_error, "sprite's file path cannot be"  \
            "empty");                                                                           \
                                                                                                \
    if(path[path.size()-1] != '/')                                                              \
        path += "/";                                                                            \
    if (!std::experimental::filesystem::exists(path))                                           \
    throw_exception_with_msg(utilities::invalid_path_error, "sprites base path \"" + path + "\""\
        "does not exists");
#endif

#define loadSprite(basePath, name, path, renderer)                                              \
    registerElement(new sprite(name, basePath + path, renderer));

using namespace engine::managers;
using namespace graphics;

void assetsManager::loadSprites(std::string basePath, SDL_Renderer *renderer)
{
    validatePath(basePath);
    
    while(!spriteQueue.empty())
    {
        std::pair<std::string, std::string> pair = spriteQueue.front();
        loadSprite(basePath, pair.first, pair.second, renderer);
        spriteQueue.pop();
    }
}