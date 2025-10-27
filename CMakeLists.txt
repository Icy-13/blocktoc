cmake_minimum_required(VERSION 3.27)
project(BlockToC)

set(CMAKE_CXX_STANDARD 23)

find_package(pugixml REQUIRED)

add_executable(BlockToC src/main.cpp)

target_link_libraries(BlockToC PRIVATE pugixml::pugixml)