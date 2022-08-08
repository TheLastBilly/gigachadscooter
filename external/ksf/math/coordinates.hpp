#pragma once

#include "engine/node.hpp"

namespace math
{
    class coordinates
    {
    public:
        inline void setHorizontalRange(int leftLimit, int rightLimit)
        {
            this->leftLimit = leftLimit;
            this->rightLimit = rightLimit;
        }
        inline void setVerticalRange(int lowerLimit, int upperLimit)
        {
            this->upperLimit = upperLimit;
            this->lowerLimit = lowerLimit;
        }

        void moveHorizontally(int amount)
        {
            x += amount;
            if(x > rightLimit)
                x = rightLimit;
            if(x < leftLimit)
                x = leftLimit;
        }
        void moveVertically(int amount)
        {
            y += amount;
            if(y > upperLimit)
                y = upperLimit;
            if(y < lowerLimit)
                y = lowerLimit;
        }

        inline int getX() const
        { return x; }
        inline int getY() const
        { return y; }

        inline void setX(int x)
        { this->x = x; }
        inline void setY(int y)
        { this->y = y; }

        inline int getLeftLimit() const
        { return leftLimit; }
        inline int getRightLimit() const
        { return rightLimit; }
        inline int getUpperLimit() const
        { return upperLimit; }
        inline int getLowerLimit() const
        { return lowerLimit; }

    private:
        int x = 0, y = 0;

        int leftLimit = 0, rightLimit = 0, upperLimit = 0, lowerLimit = 0;
    };
}