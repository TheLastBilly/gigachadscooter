#include "engine/managers/animationsManager.hpp"

register_logger()

using namespace engine::managers;
using namespace graphics;

void animationsManager::loadAnimations(assetsManager* assetsManaterPtr)
{
    while(!animationQueue.empty())
    {
        const std::pair<std::string, framesRequest> &pair = animationQueue.front();

        animation* animationPtr = new animation(pair.first);
        animationPtr->addFrames(assetsManaterPtr, pair.second);
        registerElement(animationPtr);
        
        animationQueue.pop();
    }
}