cmake_minimum_required (VERSION 3.0)

project(GigaChadScooter)
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

include(FindPkgConfig)

include_directories(${X11_INCLUDE_DIR})

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/..")

add_executable(gcwm
    "gcwm.c"
    "atoms.c"
    "wm.c"
    "xerror.c"
    
    "../*.c"
)
target_link_libraries(gcwm
    ${X11_LIBRARIES}
)