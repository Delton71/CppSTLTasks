cmake_minimum_required(VERSION 3.8)

project(CppSTLTasks)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(.)
add_subdirectory(tasks)
add_subdirectory(playground)

