cmake_minimum_required(VERSION 3.7)
project(stm32)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(stm32 ${SOURCE_FILES})