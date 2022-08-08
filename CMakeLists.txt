cmake_minimum_required (VERSION 3.0)

project(GigaChad)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(FindPkgConfig)

include(cmake/utilities.cmake)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES
    "external/ksf/*.cpp"
	"source/*.cpp"
)

add_executable(${PROJECT_NAME} 
	${SOURCES}
)

list_subdirectories_recursive(include_dirs "external")
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})

list_subdirectories_recursive(include_dirs "source")
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})

target_link_directories(${PROJECT_NAME} 
	PRIVATE ${SDL2_INCLUDE_DIRS}
	PRIVATE ${SDL2IMAGE_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_NAME="${PROJECT_NAME}")

target_link_libraries(${PROJECT_NAME}  
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}

	stdc++fs
)