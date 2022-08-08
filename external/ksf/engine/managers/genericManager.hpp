#pragma once

#include <vector>

#include "utilities/exceptions.hpp"

#include "engine/node.hpp"

namespace engine
{
    namespace managers
    {
        template<typename type>
        class genericManager : public engine::node
        {
        public:
            register_exception(repeated_name_error, "cannot register child with repeating name");

        protected:
            genericManager(const std::string& name) : node(name) {}

            void registerElement(type* request)
            {
                try
                {
                    getChild<type>(request->getName());
                    throw_exception_with_msg(repeated_name_error, "child with name \"" + request->getName() + "\" already registered");
                }
                catch (...)
                {
                }

                appendChild(request);
                elements.push_back(std::shared_ptr<type>(request));
            }

            inline type* getObject(const std::string& name) const
            {
                return getChild<type>(name);
            }

        protected:
            std::vector<std::shared_ptr<type>> elements;
        };
    }
}