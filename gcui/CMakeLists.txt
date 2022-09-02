cmake_minimum_required (VERSION 3.0)

project(GigaChadScooter)
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2GFX REQUIRED SDL2_gfx)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(${SDL2TTF_INCLUDE_DIRS})
include_directories(${SDL2GFX_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/..")

add_executable(gcui
    "gcui.c"
    "graphics.c"

    "../log.c"
    "../mema.c"
    "../comm.c"

    "visuals/speedometer.c"
    "visuals/battery.c"
    "visuals/time.c"
    "visuals/temperature.c"
    "visuals/blinkers.c"
)
target_link_libraries(gcui
    -lm
    
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
    ${SDL2GFX_LIBRARIES}
)