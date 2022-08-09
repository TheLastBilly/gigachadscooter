#include "engine/core.hpp"

#include "views/ui.hpp"

#ifndef APP_NAME
#define APP_NAME "gcs"
#endif

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

static void loadUI(engine::core *engineCore, const std::string& uiPath)
{
    
}

void setupCore(engine::core *engineCore)
{
    engineCore->setWindowHeight(720);
    engineCore->setWindowWidth(1080);

    engineCore->setAssetsRootPath(engine::core::getBasePath());

    engineCore->getViewsManager()->registerView(new gcs::views::ui("intro"));

    engineCore->getViewsManager()->setActiveView("intro");

    engineCore->setAppName(APP_NAME);
}