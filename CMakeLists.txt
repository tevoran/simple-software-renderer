cmake_minimum_required(VERSION 3.16)
project("simple-software-renderer")

#BASICALLY REALEASE MODE
SET(CMAKE_CXX_FLAGS  "-m64 -Ofast -flto -march=native -funroll-loops")

#DEBUGMODE
#SET(CMAKE_CXX_FLAGS "-g -pg")

find_library(SDL2 SDL2)
find_package(SDL2 REQUIRED)

include_directories(vendor/glm/)
include_directories(vendor/fast_obj/)
include_directories(vendor/LoadBMP/)

#creating the executable of the simple software renderer
include_directories(src/)
file(GLOB_RECURSE "ssr" SRC src/*.cpp)
add_executable(ssr ${ssr})

target_link_libraries(ssr LINK_PRIVATE "${SDL2}")
