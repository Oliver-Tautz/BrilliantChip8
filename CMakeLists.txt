cmake_minimum_required(VERSION 3.10)
project(BrilliantChip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find logging and testing libraries
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(cxxopts REQUIRED)

# Print compiler details
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")



# ========== Emulator Core Library ==========
add_library(brilliant_chip8
    src/BrilliantChip8.cpp
    src/OpCodeExecutor.cpp
    src/OpCodes.cpp
    src/Renderer.cpp
    src/utils.cpp
)

target_include_directories(brilliant_chip8 PUBLIC include)

target_sources(brilliant_chip8
    PRIVATE
        src/utils.cpp
        $<$<BOOL:${WIN32}>:src/utils_win.cpp>
        $<$<NOT:$<BOOL:${WIN32}>>:src/utils_posix.cpp>
)

# Link spdlog to core library so it's available to all dependents
target_link_libraries(brilliant_chip8 PUBLIC spdlog::spdlog)

# ========== Main Executable ==========
add_executable(brilliant_chip8_main
    src/main.cpp
)

target_link_libraries(brilliant_chip8_main PRIVATE brilliant_chip8)

# ========== Testing Setup ==========
enable_testing()

add_executable(BrilliantChip8Tests
    tests/BrilliantChip8Tests.cpp
)

target_include_directories(BrilliantChip8Tests PRIVATE include)
target_compile_definitions(BrilliantChip8Tests PRIVATE UNIT_TEST)

target_link_libraries(BrilliantChip8Tests
    PRIVATE
    brilliant_chip8
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME AllChip8Tests COMMAND BrilliantChip8Tests)
