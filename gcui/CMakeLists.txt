project(gcui)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

include(FindPkgConfig)

if(DEFINED TOOLCHAIN_PATH)
	include(cmake/toolchain.cmake)
endif()

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2GFX REQUIRED SDL2_gfx)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(${SDL2TTF_INCLUDE_DIRS})
include_directories(${SDL2GFX_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}")

file(GLOB sources "*.c" "visuals/*.c")
add_executable(gcui
    ${sources}
)
target_link_libraries(gcui
    -lm
    
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
    ${SDL2GFX_LIBRARIES}
)
