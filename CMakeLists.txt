cmake_minimum_required(VERSION 3.5)
project(CG_Project)

include(ExternalProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)

add_subdirectory(libraries/fmt)
add_subdirectory(libraries/pugixml)
add_subdirectory(libraries/argparse)

add_subdirectory(src/generator)
add_subdirectory(src/common)
add_subdirectory(src/engine)

target_include_directories(generator PUBLIC includes)
target_include_directories(common PUBLIC includes)
target_include_directories(engine PUBLIC includes)
