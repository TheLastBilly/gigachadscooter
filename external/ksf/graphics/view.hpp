#ifndef view_hpp__
#define view_hpp__

#include <memory>
#include <vector>
#include <functional>

#include <SDL2/SDL.h>

#include <engine/managers/assetsManager.hpp>
#include <engine/managers/animationsManager.hpp>

#include "engine/node.hpp"
#include "engine/entity.hpp"
#include "utilities/generics.hpp"

namespace graphics
{
    class view: public engine::node
    {
    public:
        struct resolution
        { int width, height; };

        struct backgroundColor
        { int r = 0, g = 0, b = 0, a = 0; };

    public:
        view(const std::string &name): node(name) {}

    public:
        virtual void initialize() = 0;
        virtual void setup() = 0;
        virtual void update(size_t delta) = 0;
        virtual void cleannup() = 0;

    public:        
        inline void setRenderer(SDL_Renderer *renderer)
        { this->renderer = renderer; }
        void setWindow(SDL_Window *window)
        {
            this->window = window;
            SDL_GetWindowSize(window, &width, &height);
        }

        inline SDL_Renderer* getRenderer() const
        { return renderer; }
        inline SDL_Window* getWindow() const
        { return window; }

        bool windowSizeHasChanged()
        {
            int newHeight = 0, newWidth = 0;
            SDL_GetWindowSize(window, &newHeight, &newWidth);

            return newHeight != height || newWidth != width;
        }

        inline int getWindowHeight()
        {
            SDL_GetWindowSize(window, NULL, &height);
            return height;
        } 
        inline int getWindowWidth()
        {
            SDL_GetWindowSize(window, &width, NULL);
            return width;
        }

        inline void setChangeActiveViewCallback(const std::function<void(const std::string &name)> &callback)
        { changeActiveViewCallback = callback; }
        void changeActiveNode(const std::string &name) const
        {
            if(!changeActiveViewCallback)
                throw_exception_with_msg(utilities::null_callback_error, "changeActiveNode callback was not set");
            
            changeActiveViewCallback(name);
        }

        inline void setShouldCloseCallback(const std::function<void(bool)> &callback)
        { shouldCloseCallback = callback; }
        void shouldClose(bool shouldClose) const
        {
            if(!shouldCloseCallback)
                throw_exception_with_msg(utilities::null_callback_error, "shouldClose callback was not set");
            
            shouldCloseCallback(shouldClose);
        }

        void moveEntityToCenter(engine::entity* entityPtr)
        { 
            sprite* spritePtr = entityPtr->getCurrentAnimation()->getCurrentFrame()->getSprite();
            
            entityPtr->getCoordinates()->setX(getWindowWidth()/2 - spritePtr->getWidth()/2);
            entityPtr->getCoordinates()->setY(getWindowHeight()/2 - spritePtr->getHeight()/2);
        }

        void setBackgroundColor(int r, int g, int b, int a)
        {
            currentBackgroundColor.r = r;
            currentBackgroundColor.g = g;
            currentBackgroundColor.b = b;
            currentBackgroundColor.a = a;

            SDL_SetRenderDrawColor(getRenderer(), r,g,b,a);
        }
        inline backgroundColor getBackgroundColor() const
        { return currentBackgroundColor; }

        inline void clearRender() const
        { SDL_RenderClear(renderer); }

    private:
        std::function<void(const std::string &name)> changeActiveViewCallback;
        std::function<void(bool)> shouldCloseCallback;

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        int height = 0, width = 0;

        backgroundColor currentBackgroundColor = {};
    };
}

#endif