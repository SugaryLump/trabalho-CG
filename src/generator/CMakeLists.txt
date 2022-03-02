cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

add_executable(generator ${sources})

target_link_libraries(
    generator
    common
)
