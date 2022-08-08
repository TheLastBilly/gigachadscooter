#pragma once

#include <utility>
#include <vector>
#include <functional>

#include "utilities/generics.hpp"

#include "engine/entity.hpp"
#include "engine/managers/genericManager.hpp"
#include "math/collisions.hpp"

#define collisionEventCallback [=](engine::entity *entityA, engine::entity *entityB)

namespace engine
{
    namespace managers
    {
        class collisionEvent : public node
        {
        public:
            typedef std::function<void(engine::entity* entityA, engine::entity* entityB)> callback;

        public:
            collisionEvent(const std::string& name, entity* entityA, entity* entityB, const callback& onCollisionCallback, const callback& outOfCollisionCallback = nullptr) :
                node(name), entityA(entityA), entityB(entityB), onCollisionCallback(onCollisionCallback), outOfCollisionCallback(outOfCollisionCallback) {}

            inline bool collisionHappened() const
            {
                return active && math::collisions::hitboxesCollide(*entityA->getHitbox(), *entityB->getHitbox());
            }
            inline void runCallbacks()
            {
                if (getActive())
                {
                    if (collisionHappened() && onCollisionCallback)
                    {
                        onCollisionCallback(entityA, entityB);
                        return;
                    }
                    
                    if (outOfCollisionCallback)
                        outOfCollisionCallback(entityA, entityB);
                }
            }

            inline bool getActive() const
            {
                return active;
            }
            inline void setActive(bool active)
            {
                this->active = active;
            }

        private:
            bool active = false;

            entity* entityA, * entityB;
            callback onCollisionCallback, outOfCollisionCallback;
        };

        class collisionsManager : public genericManager<collisionEvent>
        {
        public:
            collisionsManager(const std::string& name) : genericManager(name) {}

            inline void regsiterCollisionEvent(collisionEvent* request)
            {
                registerElement(request);
            }

            inline collisionEvent* getCollision(const std::string& name)
            {
                return getObject(name);
            }

            void setAllCollisionsEnable(bool enable)
            {
                for (const auto& event : elements)
                    event->setActive(enable);
            }

            void runCollisions()
            {
                for (const auto& event : elements)
                    event->runCallbacks();
            }
        };
    }
}