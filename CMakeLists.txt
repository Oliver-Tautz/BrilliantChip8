cmake_minimum_required(VERSION 3.10)
project(BrilliantChip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")





# Source files
add_executable(brilliant_chip8
    src/main.cpp
    src/BrilliantChip8.cpp
    src/OpCodeExecutor.cpp
    src/OpCodes.cpp
    src/Renderer.cpp
)

# Include headers
#include_directories(include)
# this is more modular and minimizes include problems
target_include_directories(brilliant_chip8 PRIVATE include)
