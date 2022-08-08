#pragma once

#include <vector>

#include "utilities/logger.hpp"
#include "engine/managers/genericManager.hpp"
#include "graphics/view.hpp"

namespace engine
{
    namespace managers
    {
        class viewsManager : public genericManager<graphics::view>
        {
        public:
            viewsManager(const std::string& name) : genericManager(name) {}

            inline void registerView(graphics::view* request)
            {
                registerElement(request);
            }

            inline void setActiveView(const std::string& name)
            {
                activeView = getChild<graphics::view>(name);
            }
            inline graphics::view* getActiveView() const
            {
                return activeView;
            }

        private:
            graphics::view* activeView = nullptr;
        };
    }
}