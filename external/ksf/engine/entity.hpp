#pragma once

#include <memory>

#include "engine/node.hpp"
#include "graphics/animation.hpp"
#include "engine/managers/nodesManager.hpp"
#include "math/coordinates.hpp"
#include "math/collisions.hpp"

namespace engine
{
    class entity: public node
    {
    public:
        register_exception(animation_not_defined_error, "animation has not been defined");

    public:
        entity(const std::string &name):  node(name) {}

        inline math::coordinates* getCoordinates()
        { return &coordinates; }
        inline void setCoordinates(math::coordinates* coordinates)
        { this->coordinates = *coordinates; }

        inline void setCurrentAnimation(graphics::animation* animation)
        { currentAnimation = animation; }
        inline graphics::animation* getCurrentAnimation() const 
        { return currentAnimation; }

        inline const math::collisions::hitbox* getHitbox() const
        { return &hitbox; }

        void updateHitbox(int width, int height)
        {
            hitbox.upperVector.a.setX(coordinates.getX());
            hitbox.upperVector.a.setY(coordinates.getY());

            hitbox.upperVector.b.setX(coordinates.getX() + width);
            hitbox.upperVector.b.setY(hitbox.upperVector.a.getY());

            hitbox.lowerVector = hitbox.upperVector;
            hitbox.lowerVector.a.setY(coordinates.getY() + height);
            hitbox.lowerVector.b.setY(hitbox.lowerVector.a.getY());
        }

        void setConstraints(int minX, int maxX, int minY, int maxY)
        {
            setHorizontalContraints(minX, maxX);
            setVerticalContraints(minY, maxY);
        }
        void setConstraintsBasedOnCurrentSprite(int minX, int maxX, int minY, int maxY)
        {
            graphics::sprite* sprite = getCurrentAnimation()->getCurrentFrame()->getSprite();
            setConstraints(minX, maxX - sprite->getWidth(), minY, maxY - sprite->getHeight());
        }

        void setHorizontalContraints(int minX, int maxX)
        {
            coordinates.setHorizontalRange(minX, maxX);
            coordinates.moveHorizontally(0);
        }
        void setVerticalContraints(int minY, int maxY)
        {
            coordinates.setVerticalRange(minY, maxY);
            coordinates.moveVertically(0);
        }

        void update(int delta)
        {
            if (currentAnimation)
            {
                if(animationEnabled)
                    currentAnimation->play(delta, coordinates.getX(), coordinates.getY());
                updateHitbox(currentAnimation->getCurrentFrame()->getSprite()->getWidth(), currentAnimation->getCurrentFrame()->getSprite()->getHeight());
            }
        }

        void centerToScreen(int screenWidth, int screenHeight)
        {
            if (!currentAnimation)
                throw_exception_without_msg(animation_not_defined_error);

            graphics::sprite* currentSprite = getCurrentAnimation()->getCurrentFrame()->getSprite();
            coordinates.setX(screenWidth / 2 - currentSprite->getWidth() / 2);
            coordinates.setY(screenHeight / 2 - currentSprite->getHeight() / 2);
        }

        inline void setAnimationEnabled(bool animationEnabled)
        {
            this->animationEnabled = animationEnabled;
        }

    protected:
        graphics::animation* currentAnimation = nullptr;

    protected:
        math::coordinates coordinates;
        math::collisions::hitbox hitbox;

        bool animationEnabled = true;
    };
}