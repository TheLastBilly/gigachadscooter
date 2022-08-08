#pragma once

#include "math/geometry.hpp"

namespace math
{
    class collisions
    {
    public:
        struct hitbox
        {
            inline bool isValid() const
            { return upperVector.isHorizontal() && lowerVector.isHorizontal(); }

            vector2d upperVector, lowerVector; 
        };
    
    public:
        static bool hitboxesCollide(const hitbox &a, const hitbox &b)
        {
            if(!a.isValid() ||!b.isValid()) return false;

            if(a.lowerVector.b.getX() < b.upperVector.a.getX() || a.lowerVector.a.getX() > b.upperVector.b.getX()) return false;
            if(a.lowerVector.a.getY() < b.upperVector.a.getY() || a.upperVector.a.getY() > b.lowerVector.a.getY()) return false;
        
            return true;
        }
    };
}