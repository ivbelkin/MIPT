cmake_minimum_required(VERSION 3.8)
project(task_2)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -fno-exceptions)

set(SOURCE_FILES main.cpp TryCatchBlock.cpp ExceptionManager.cpp Object.cpp tests.cpp)
add_executable(task_2 ${SOURCE_FILES})