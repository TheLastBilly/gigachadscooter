#pragma once

#include <utility>
#include <queue>

#include "utilities/generics.hpp"
#include "utilities/logger.hpp"

#include "engine/node.hpp"
#include "graphics/sprite.hpp"

#include "engine/managers/genericManager.hpp"

namespace engine
{
    namespace managers
    {
        class assetsManager : public genericManager<graphics::sprite>
        {
        public:
            assetsManager(const std::string& name) : genericManager(name) {}

            inline void requestSprite(const std::string& name, const std::string& path)
            {
                spriteQueue.push(std::pair<std::string, std::string>(name, path));
            }
            void loadSprites(std::string basePath, SDL_Renderer* renderer);

        private:
            std::queue<std::pair<std::string, std::string>> spriteQueue;
        };
    }
}