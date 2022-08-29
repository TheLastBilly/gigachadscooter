cmake_minimum_required (VERSION 3.0)

project(GigaChadScooter)
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/..")

add_executable(gcui
    "gcui.c"
    "graphics.c"

    "../log.c"
    "../mema.c"
    "../comm.c"
)
target_link_libraries(gcui
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
)