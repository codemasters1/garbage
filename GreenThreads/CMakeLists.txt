cmake_minimum_required(VERSION 3.6)
project(GreenThreads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -m64")

enable_language(ASM-ATT)
set(SOURCE_FILES main.cpp switch.s routine.cpp routine.hpp)
add_executable(GreenThreads ${SOURCE_FILES})