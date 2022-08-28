#include "engine/core.hpp"

#include "views/ui.hpp"
#include "utilities/generics.hpp"

#ifndef APP_NAME
#define APP_NAME "gcs"
#endif

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

register_logger();

void setupCore(engine::core *engineCore)
{
    engineCore->setWindowHeight(720);
    engineCore->setWindowWidth(1080);

    engineCore->setAssetsRootPath(engine::core::getBasePath() + "/../../assets/");

    engineCore->getViewsManager()->registerView(new gcs::views::ui("intro"));

    tinyxml2::XMLDocument doc;
    if(doc.LoadFile(std::string(engineCore->getAssetsRootPath() + "/ui/main_ui.xml").c_str()))
    {
        error("cannot load xml file");
        throw_exception_without_msg(utilities::invalid_path_error);
    }
    
    engineCore->getViewsManager()->registerView(
        gcs::views::ui::loadUI(engineCore, doc.RootElement())
    );

    engineCore->getViewsManager()->setActiveView("intro");

    engineCore->setAppName(APP_NAME);
}