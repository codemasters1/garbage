cmake_minimum_required(VERSION 3.6)
project(untitled2)

include_directories("eigen-eigen-67e894c6cd8f")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -march=native")
set(SOURCE_FILES main.cpp Network.cpp Network.hpp)
add_executable(untitled2 ${SOURCE_FILES})