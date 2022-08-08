#pragma once

#include <utility>
#include <vector>

#include "utilities/generics.hpp"

#include "engine/managers/genericManager.hpp"

namespace engine
{
    namespace managers
    {
        class nodesManager : public genericManager<node>
        {
        public:
            nodesManager(const std::string& name) : genericManager(name) {}

            inline void registerNode(node* request)
            {
                registerElement(request);
            }
        };
    }
}