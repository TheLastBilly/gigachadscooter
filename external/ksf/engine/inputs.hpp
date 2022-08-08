#pragma once

#include <SDL2/SDL.h>

#include "engine/node.hpp"

namespace engine
{
    class keyboardHandler: public node
    {
    public:
        enum key
        {
            up, down, left, right,
            q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,
            num0,num1,num2,num3,num4,num5,num6,num7,num9,
            enter, space, scape,

            dot, comma,

            none
        };

    private:
        struct input
        {
            input(const key &id, unsigned int code): id(id), code(code) {} 
            const key id; const unsigned int code;
        };
    
    public: 
        keyboardHandler(const std::string &name);

        void updateInputs(const SDL_Event &event);
        
        inline bool isKeyActive(key id) const
        { return inputState[id]; }

        inline void clearKeys()
        { for (size_t i = 0; i < sizeof(inputState) / sizeof(bool); i++) inputState[i] = false; }

    private:
        bool inputState[static_cast<unsigned int>(key::none) + 1] = {false};
    };
}