cmake_minimum_required(VERSION 3.5)

project(CG_Project)

add_subdirectory(src/generator)
add_subdirectory(src/common)
add_subdirectory(src/engine)

target_include_directories(generator PUBLIC includes)
target_include_directories(common PUBLIC includes)
target_include_directories(engine PUBLIC includes)
