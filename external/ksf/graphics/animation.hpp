#pragma once

#include <vector>
#include <utility>

#include "engine/node.hpp"
#include "engine/managers/assetsManager.hpp"

#include "graphics/sprite.hpp"

#ifndef ssize_t
#define ssize_t signed long long 
#endif

namespace graphics
{
    class animation: public engine::node
    {
    public:
        class frame
        {
        public:
            frame(const sprite* spritePtr, int duration): spritePtr(spritePtr), duration(duration) {}
            frame(const frame& f): spritePtr(f.spritePtr), duration(f.duration) {}

            inline sprite* getSprite() const
            { return (sprite*)spritePtr; }

            inline int getDuration() const
            { return duration; }
            inline void setDuration(int duration)
            { this->duration = duration; }
        
        private:
            const sprite* spritePtr = nullptr; 
            int duration = 0;
        };

    public:
        animation(const std::string &name): node(name) {}

        inline void addFrame(engine::managers::assetsManager* manager, const std::string &name, int duration)
        { frames.push_back(frame(manager->getChild<sprite>(name), duration)); }
        inline void addFrames(engine::managers::assetsManager* manager, const std::vector<std::pair<std::string, int>> &frames)
        { for(const auto& f: frames) addFrame(manager, f.first, f.second); }

        frame* getFrame(size_t i)
        {
            if(i >= frames.size())
                throw_exception_without_msg(utilities::index_out_of_bounds_error);
            
            return &frames.data()[i];
        }
        inline size_t getFrameCount() const
        { return frames.size(); }

        inline frame* getCurrentFrame()
        { return &frames.data()[currentFrame]; }

        void setCurrentFrame(size_t i)
        {
            if (i >= frames.size())
                throw_exception_without_msg(utilities::index_out_of_bounds_error);
            
            currentFrame = i;
        }

        void load()
        {
            for (size_t i = 0; i < frames.size(); i++)
                frames[i].getSprite()->load();
        }
        void unload()
        {
            for (size_t i = 0; i < frames.size(); i++)
                frames[i].getSprite()->unload();
        }

        void reset()
        {
            timer = 0;
            currentFrame = 0;
            donePlaying = false;
        }

        inline void setRepeat(bool repeat)
        { this->repeat = repeat; }
        inline bool getRepeat() const
        { return repeat; }

        inline bool isDonePlaying() const
        { return donePlaying; }

        void play(ssize_t delta, int x, int y)
        {
            if(frames.size() < 1)
                return;

            ssize_t counter = timer;
            if((timer += delta) >= frames[currentFrame].getDuration() && shouldTransition)
            {
                donePlaying = currentFrame >= frames.size() - 1;
                currentFrame = !donePlaying ? currentFrame + 1: (repeat ? 0 : currentFrame);
                timer = 0;
            }

            timer += delta;
            frames[currentFrame].getSprite()->render(x, y);
        }

        void setSpritesSize(int width, int height)
        {
            for (size_t i = 0; i < frames.size(); i++)
            {
                frames[i].getSprite()->setWidth(width);
                frames[i].getSprite()->setHeight(height);
            }
        }

        inline void flipSprites(sprite::flip f)
        {
            currentFlip = f;

            for (size_t i = 0; i < frames.size(); i++)
            { frames[i].getSprite()->setFlip(f); }
        }
        inline sprite::flip getSpritesFlip() const
        { return currentFlip; }

        inline void setShouldTransition(bool shouldTransition)
        { this->shouldTransition = shouldTransition; }
        inline bool getShouldTransition() const
        { return shouldTransition; }

    private:
        std::vector<frame> frames;

        size_t timer = 0;
        size_t currentFrame = 0;

        sprite::flip currentFlip = sprite::flip::none;

        bool repeat = true, shouldTransition = true, donePlaying = false;
    };
}