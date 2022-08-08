#include "engine/core.hpp"
#include "utilities/logger.hpp"

#include <thread>

register_logger();

using namespace graphics;
using namespace engine;

core::core(int width, int height):
    node("Root"),

    width(width), 
    height(height),

    assetsManagerPtr(std::make_shared<managers::assetsManager>("Assets Manager")),
    viewsManagerPtr(std::make_shared<managers::viewsManager>("Views Manager")),
    animationsManagerPtr(std::make_shared<managers::animationsManager>("Animations Manager")),
    collisionsManagerPtr(std::make_shared<managers::collisionsManager>("Collisions Manager")),
    nodesManagerPtr(std::make_shared<managers::nodesManager>("Generic Nodes Manager")),

    keyboardHandlerPtr(std::make_shared<keyboardHandler>("Keyboard Handler"))
{
    appendChild<managers::assetsManager>(assetsManagerPtr);
    appendChild<managers::viewsManager>(viewsManagerPtr);
    appendChild<managers::animationsManager>(animationsManagerPtr);
    appendChild<managers::collisionsManager>(collisionsManagerPtr);
    appendChild<managers::nodesManager>(nodesManagerPtr);

    appendChild<keyboardHandler>(keyboardHandlerPtr);
}

core::~core()
{
    terminate();
}

void core::initialize()
{
    terminate();

    if(SDL_Init(SDL_DEFAULT_FLAGS) != 0)
        throw_exception_with_msg(sdl_initialization_error, SDL_GetError());
    if(IMG_Init(SDL_IMAGE_DEFAULT_FLAGS) != SDL_IMAGE_DEFAULT_FLAGS)
        throw_exception_with_msg(sdl_initialization_error, IMG_GetError());

    window = SDL_CreateWindow(
        getAppName().c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_DEFAULT_WINDOW_FLAGS
    );
    if(!window)
        throw_exception_with_msg(sdl_window_creation_error, SDL_GetError());
    
    renderer = SDL_CreateRenderer(
        window, -1, SDL_DEFAULT_RENDERER_FLAGS
    );
    if(!renderer)
        throw_exception_with_msg(sdl_renderer_creation_error, SDL_GetError());

    initialized = true;

    assetsManagerPtr->loadSprites(assetsRootPath, renderer);
    animationsManagerPtr->loadAnimations(getAssetsManager());
    setupViews();
}

void core::terminate()
{
    if(renderer)
        SDL_DestroyRenderer(renderer);

    if(window)
        SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    initialized = false;
}

bool core::isInitialized() const
{
    return initialized;
}

bool core::windowShouldClose(const SDL_Event &event) const
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        switch (event.window.event) {

            case SDL_WINDOWEVENT_CLOSE:
                return true;

            default:
                break;
        }
        break;
    case SDL_QUIT:
        return true;
    default:
        break;
    }

    return false;
}

void core::setupViews()
{
    for(size_t i = 0; i < getViewsManager()->getChildrenCount(); i++)
    {
        graphics::view* viewPtr = getViewsManager()->getChild<view>(i);

        viewPtr->setRenderer(renderer);
        viewPtr->setWindow(window);

        viewPtr->setShouldCloseCallback([this](bool shouldClose){this->shouldClose = true;});
        viewPtr->setChangeActiveViewCallback(
            [this](const std::string &name) {
                this->getViewsManager()->setActiveView(name);
            }
        );

        viewPtr->initialize();
    }
}

void core::execute()
{
    bool shouldRun = true;
    int receivedEvent = 0;

    SDL_Event event = {};
    size_t beforeFrame = 0, afterFrame = 0;
    size_t delta = 0;

    SDL_RenderClear(renderer);

    size_t counter =0;

    while(shouldRun && !this->shouldClose)
    {
        if((receivedEvent = SDL_PollEvent(&event)) < 1)
            event = {};
        else
            keyboardHandlerPtr->updateInputs(event);

        SDL_RenderClear(renderer);
        beforeFrame = utilities::getCurrentTimeInMilliseconds();

        if(currentView != viewsManagerPtr->getActiveView())
        {
            if (currentView != nullptr)
            {
                currentView->cleannup();
                currentView->clearRender();
                if(unloadTextureAfterCleannup)
                    animationsManagerPtr->unloadAll();
            }

            unloadTextureAfterCleannup = true;
            collisionsManagerPtr->setAllCollisionsEnable(false);
            currentView = viewsManagerPtr->getActiveView();

            currentView->clearRender();
            SDL_RenderPresent(renderer);

            keyboardHandlerPtr->clearKeys();
            currentView->setup();
            afterFrame = beforeFrame;
        }

        if(!currentView)
            throw_exception_without_msg(active_view_missing_error);

        delta = beforeFrame - afterFrame;
        currentView->update(delta);
        collisionsManagerPtr->runCollisions();
        afterFrame = utilities::getCurrentTimeInMilliseconds();
        
#ifdef PRINT_DELTA
        info(std::to_string(counter++) + " delta: " + std::to_string(delta));
#endif
        counter += delta;

        if (counter >= 10)
        {
            SDL_RenderPresent(renderer);
            counter = 0;
        }

        if(receivedEvent && windowShouldClose(event))
            break;
        
        if (delta < 1)
            SDL_Delay(1);
    }

    info("exiting...");
}