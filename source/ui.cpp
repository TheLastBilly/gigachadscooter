#include "engine/core.hpp"
#include "utilities/generics.hpp"

#include "views/ui.hpp"

using namespace gcs::views;

register_logger()

#define XML_UI_TAG                          "ui"
#define XML_SPRITE_TAG                      "sprite"
#define XML_ANIMATION_TAG                   "animation"

ui* ui::loadUI(const engine::core *engineCore, const tinyxml2::XMLElement* element)
{
    ui* new_ui = nullptr;
    std::string name = "", path = "";
    const tinyxml2::XMLElement* child = nullptr;
    const tinyxml2::XMLElement *name_element = nullptr, *path_element = nullptr;

    if(element == nullptr || strcmp(element->Name(), XML_UI_TAG) != 0 )
        throw_exception_with_msg(utilities::invalid_argument, "element cannot be null");

    name_element = element->FirstChildElement("name");
    if(name_element == nullptr)
        throw_exception_with_msg(xml_parsing_error, "missing name");
    new_ui = new ui(name_element->GetText());

    if((child = element->FirstChildElement(XML_SPRITE_TAG)) != nullptr)
    {
        // Get sprites
        for(;child != nullptr; child = child->NextSiblingElement(XML_SPRITE_TAG))
        {
            name_element = child->FirstChildElement("name");
            path_element = child->FirstChildElement("path");

            if(name_element == nullptr || path_element == nullptr)
                throw_exception_with_msg(xml_parsing_error, "cannot parse sprite");

            engineCore->getAssetsManager()->registerSprite(
                name_element->GetText(),
                path_element->GetText()
            );
        }
    }

    return new_ui;
}

void ui::initialize()
{
    this->core = getRoot<engine::core>();

    this->animationsManager = this->core->getAnimationsManager();
    this->viewsManager = this->core->getViewsManager();
}

void ui::setup()
{
}

void ui::update(size_t delta)
{    
}

void ui::cleannup()
{
}