#include "brilliant_chip8/Renderer.hpp"

#include <spdlog/spdlog.h>

#include <iostream>  // Just for output

#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/utils.hpp"
Renderer::Renderer() {
    // Constructor: setup graphics context (SDL, terminal, etc.)
    spdlog::info("Renderer initialized.\n");
}

// Render the current display buffer to the output, clearing the console first
void Renderer::render(const BrilliantChip8::DisplayBuffer &display) {
    // ANSI escapes: clear screen and move cursor to top-left
    std::cout << "\033[2J\033[H";

    for (const auto &row : display) {
        for (const auto &pixel : row) {
            std::cout << (pixel ? "x" : " ");
        }
        std::cout << '\n';
    }

    std::cout << std::flush;
}
