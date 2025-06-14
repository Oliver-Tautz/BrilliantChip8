#include "brilliant_chip8/Renderer.hpp"
#include <iostream> // Just for a placeholder output
#include "brilliant_chip8/BrilliantChip8.hpp"

Renderer::Renderer()
{
    // Constructor: setup graphics context (SDL, terminal, etc.)
    std::cout << "Renderer initialized.\n";
}

void Renderer::render(BrilliantChip8::DisplayBuffer &display)
{
    std::cout << "\033[H"; // ANSI escape code: move cursor to top-left

    for (const auto &row : display)
    {
        for (const auto &pixel : row)
        {
            std::cout << (pixel ? "█" : "x");
        }
        std::cout << '\n';
    }

    std::cout << std::flush;
}