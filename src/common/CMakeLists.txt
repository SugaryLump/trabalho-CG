cmake_minimum_required(VERSION 3.5)

    file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

        add_library(common ${sources})

            target_link_libraries(common)
