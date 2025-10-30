cmake_minimum_required(VERSION 3.27)
project(BlockToC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(pugixml REQUIRED)
find_package(cxxopts REQUIRED)

add_executable(BlockToC 
    src/block_to_c.cpp
    src/diagram_parser.cpp
    src/diagram_parser.h
    src/models/graph_model.h
    src/models/codegen_model.h
    src/codegen.cpp
    src/codegen.h
)

target_link_libraries(BlockToC PRIVATE pugixml::pugixml)
target_link_libraries(BlockToC PRIVATE cxxopts::cxxopts)
