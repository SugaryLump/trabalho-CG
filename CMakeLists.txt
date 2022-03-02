cmake_minimum_required(VERSION 3.5)

project(CG_Project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall)

add_subdirectory(src/generator)
add_subdirectory(src/common)
add_subdirectory(src/engine)

target_include_directories(generator PUBLIC includes)
target_include_directories(common PUBLIC includes)
target_include_directories(engine PUBLIC includes)
