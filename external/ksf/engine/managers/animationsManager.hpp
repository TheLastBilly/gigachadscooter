#pragma once

#include <utility>
#include <vector>

#include "utilities/generics.hpp"

#include "engine/node.hpp"
#include "graphics/animation.hpp"

#include "engine/managers/genericManager.hpp"
#include "engine/managers/assetsManager.hpp"

namespace engine
{
    namespace managers
    {
        class animationsManager : public genericManager<graphics::animation>
        {
        private:
            typedef std::vector<std::pair<std::string, int>> framesRequest;

        public:
            animationsManager(const std::string& name) : genericManager(name) {}

            inline void requestAnimation(const std::string& name, const std::vector<std::pair<std::string, int>>& frames)
            {
                animationQueue.push({ name, frames });
            }
            void loadAnimations(assetsManager* assetsManaterPtr);

            graphics::animation* getAnimation(const std::string &name) const
            { return getObject(name); }

            void loadAll() const
            {
                for (const std::shared_ptr<graphics::animation>& animation : elements)
                    animation->load();
            }
            void unloadAll() const
            {
                for (const std::shared_ptr<graphics::animation>& animation : elements)
                    animation->unload();
            }

        private:
            std::queue<std::pair<std::string, framesRequest>> animationQueue;
        };
    }
}