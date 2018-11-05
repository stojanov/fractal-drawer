cmake_minimum_required(VERSION 3.0)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose debug or release" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)

project(FractalDrawer)

## Set Module Path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

## Get the files
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(FractalDrawer ${SOURCES})

## Add and link libraries
find_package(SFML 2.5.1 REQUIRED graphics network audio system)
target_link_libraries(FractalDrawer sfml-graphics sfml-audio sfml-network)