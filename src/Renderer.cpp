#include "brilliant_chip8/Renderer.hpp"
#include <iostream> // Just for a placeholder output
#include "brilliant_chip8/BrilliantChip8.hpp"

Renderer::Renderer()
{
    // Constructor: setup graphics context (SDL, terminal, etc.)
    std::cout << "Renderer initialized.\n";
}

void Renderer::render(BrilliantChip8::DisplayBuffer &)
{
    // For now, just a placeholder
    std::cout << "Rendering screen...\n";
}
