cmake_minimum_required(VERSION 3.8)
project(task_4)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -fno-rtti)

set(SOURCE_FILES main.cpp)
add_executable(task_4 ${SOURCE_FILES})