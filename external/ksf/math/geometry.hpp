#pragma once

#include "engine/node.hpp"

#include "math/coordinates.hpp"

namespace math
{
    struct vector2d
    {
        vector2d(){}
        vector2d(const coordinates &a, const coordinates &b): a(a), b(b) {}
        
        inline bool isHorizontal() const
        { return a.getY() == b.getY(); }

        coordinates a, b;
    };
}