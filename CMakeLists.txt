cmake_minimum_required(VERSION 3.10)
project(BrilliantChip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# ========== Emulator Core Library ==========
add_library(brilliant_chip8
    src/BrilliantChip8.cpp
    src/OpCodeExecutor.cpp
    src/OpCodes.cpp
    src/Renderer.cpp
)

target_include_directories(brilliant_chip8 PUBLIC include)

# ========== Main Executable ==========
add_executable(brilliant_chip8_main
    src/main.cpp
)

target_link_libraries(brilliant_chip8_main brilliant_chip8)

# ========== Testing Setup ==========
enable_testing()



add_executable(BrilliantChip8Tests
    tests/BrilliantChip8Tests.cpp
)

target_include_directories(BrilliantChip8Tests PRIVATE include)

target_compile_definitions(BrilliantChip8Tests PRIVATE UNIT_TEST)

target_link_libraries(BrilliantChip8Tests
    brilliant_chip8
    gtest
    gtest_main
    pthread
)

add_test(NAME AllChip8Tests COMMAND BrilliantChip8Tests)
