cmake_minimum_required(VERSION 3.6)
project(OOL)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Class.cpp Class.hpp Member.hpp Object.hpp ObjectHelper.cpp ObjectHelper.hpp)
add_executable(OOL ${SOURCE_FILES})