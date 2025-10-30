find_package(Catch2 REQUIRED)

add_executable(BlockToC_tests
    full_codegen_test.cpp
)

target_link_libraries(BlockToC_tests PRIVATE 
    BlockToC_lib
    Catch2::Catch2WithMain
)

add_custom_command(TARGET BlockToC_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
)

include(CTest)
include(Catch)
catch_discover_tests(BlockToC_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
