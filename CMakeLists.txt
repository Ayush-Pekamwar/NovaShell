cmake_minimum_required(VERSION 3.13)

project(NovaShell)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(NovaShell ${SOURCE_FILES})

target_link_libraries(NovaShell PRIVATE readline)
