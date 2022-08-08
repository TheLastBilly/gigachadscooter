#ifndef core_hpp__
#define core_hpp__

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "graphics/view.hpp"

#include "engine/inputs.hpp"

#include "engine/managers/assetsManager.hpp"
#include "engine/managers/viewsManager.hpp"
#include "engine/managers/animationsManager.hpp"
#include "engine/managers/collisionsManager.hpp"
#include "engine/managers/nodesManager.hpp"

#include "utilities/exceptions.hpp"
#include "utilities/generics.hpp"

#define SDL_IMAGE_DEFAULT_FLAGS (IMG_INIT_JPG | IMG_INIT_PNG)
#define SDL_DEFAULT_FLAGS SDL_INIT_VIDEO
#define SDL_DEFAULT_WINDOW_FLAGS 0
#define SDL_DEFAULT_RENDERER_FLAGS SDL_RENDERER_ACCELERATED

namespace engine
{
    class core: public engine::node
    {
    public:
        register_exception(sdl_initialization_error, "cannot initialize SDL");
        register_exception(sdl_window_creation_error, "cannot create sdl window");
        register_exception(sdl_renderer_creation_error, "cannot create sdl renderer");
        register_exception(active_view_missing_error, "no active view has been specified");
        register_exception(core_not_initialized_error, "engine core has not been initialized");

    public:
        core(int width = 800, int height = 600);
        ~core();

    public:
        inline void setAssetsRootPath(const std::string &path)
        { assetsRootPath = path; }
        inline std::string getAssetsRootPath() const
        { return assetsRootPath; }

        inline engine::managers::assetsManager* getAssetsManager() const
        { return assetsManagerPtr.get(); }
        inline engine::managers::viewsManager* getViewsManager() const
        { return viewsManagerPtr.get(); }
        inline engine::managers::animationsManager* getAnimationsManager() const
        { return animationsManagerPtr.get(); }
        inline engine::managers::collisionsManager* getCollisionsManager() const
        { return collisionsManagerPtr.get(); }
        inline engine::managers::nodesManager* getNodesManager() const
        { return nodesManagerPtr.get(); }

        inline void setAppName(const std::string &name)
        { appName = name; }
        inline std::string getAppName() const
        { return appName; }

        inline void setWindowHeight(int height)
        { this->height = height; }
        inline void setWindowWidth(int width)
        { this->width = width; }

        inline void setWindowResizeEnabled(bool enabled) const
        {
            if (!initialized)
                throw_exception_without_msg(core_not_initialized_error);

            SDL_SetWindowResizable(window, enabled ? SDL_TRUE : SDL_FALSE);
        }
        bool getWindowResizeEnabled() const
        {
            if (!initialized)
                throw_exception_without_msg(core_not_initialized_error);
            
            SDL_bool resizeable = SDL_FALSE;
            SDL_SetWindowResizable(window, resizeable);
            return resizeable == SDL_TRUE ? true : false;
        }

        inline void setUnloadTextureAfterCleannup(bool unloadTextureAfterCleannup)
        { unloadTextureAfterCleannup = unloadTextureAfterCleannup; }
        inline bool getUnloadTextureAfterCleannup() const
        { return unloadTextureAfterCleannup; }

        static inline std::string getBasePath()
        { return std::string(SDL_GetBasePath()); }

    protected:
        void initialize();
        void execute();
        void terminate();

    private:
        bool isInitialized() const;
        bool windowShouldClose(const SDL_Event &event) const;

        void setupViews();
    
    private:
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        int width, height;

        graphics::view *currentView = nullptr;

        bool initialized = false;
        bool shouldClose = false;
        bool unloadTextureAfterCleannup = true;
    
    private:
        std::shared_ptr<engine::managers::assetsManager> assetsManagerPtr;
        std::shared_ptr<engine::managers::viewsManager> viewsManagerPtr;
        std::shared_ptr<engine::managers::animationsManager> animationsManagerPtr;
        std::shared_ptr<engine::managers::collisionsManager> collisionsManagerPtr;
        std::shared_ptr<engine::managers::nodesManager> nodesManagerPtr;

        std::shared_ptr<keyboardHandler> keyboardHandlerPtr;

        std::string appName = "";

        std::string assetsRootPath = "";
    };

    class _coreLauncher: public core
    {
    public: 
        _coreLauncher(int width = 800, int height = 600): core(width, height) {}

        inline void initialize()
        { this->core::initialize(); }
        inline void execute()
        { this->core::execute(); }
        inline void terminate()
        { this->core::terminate(); }
    };
}

void setupCore(engine::core* gameCore);

#endif