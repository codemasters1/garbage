cmake_minimum_required(VERSION 3.5)
project(ConsoleUtils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp Buffer.cpp Buffer.hpp)
add_executable(ConsoleUtils ${SOURCE_FILES})