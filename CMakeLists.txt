cmake_minimum_required(VERSION 3.27)
project(BlockToC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build the tests" ON)

find_package(pugixml REQUIRED)
find_package(cxxopts REQUIRED)

add_library(BlockToC_lib STATIC
    src/diagram_parser.cpp
    src/diagram_parser.h
    src/models/graph_model.h
    src/models/codegen_model.h
    src/codegen.cpp
    src/codegen.h
)

target_link_libraries(BlockToC_lib PRIVATE pugixml::pugixml)
target_include_directories(BlockToC_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(BlockToC 
    src/block_to_c.cpp
)

target_link_libraries(BlockToC PRIVATE BlockToC_lib cxxopts::cxxopts)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
