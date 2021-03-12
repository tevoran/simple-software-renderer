cmake_minimum_required(VERSION 3.16)
project("simple-software-renderer")

find_library(SDL2 SDL2)
find_package(SDL2 REQUIRED)

#creating the executable of the simple software renderer
file(GLOB_RECURSE "ssr" SRC src/*.cpp)
add_executable(ssr ${ssr})

target_link_libraries(ssr LINK_PRIVATE "${SDL2}")
