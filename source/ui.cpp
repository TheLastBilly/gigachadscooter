#include "engine/core.hpp"

#include "views/ui.hpp"

using namespace gcs::views;

register_logger()

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