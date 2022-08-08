#include "utilities/logger.hpp"

#include "engine/core.hpp"

#ifdef WINDOWS
#include <windows.h>
#endif

register_logger();

#ifdef WINDOWS
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    std::shared_ptr<engine::_coreLauncher> launcher = std::make_shared<engine::_coreLauncher>();
    setupCore(static_cast<engine::core*>(launcher.get()));
    try
    {
        launcher->initialize();
        launcher->execute();
    }
    catch(const std::exception &e)
    {
        error(e.what());
        return -1;
    }
    return 0;
}
