#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/Renderer.hpp"
#include <iostream>
#include <filesystem>

int main()
{

    Chip8 chip8;
    chip8.initialize();

    chip8.loadROM(std::filesystem::path("external/roms/roms/demos") / "Maze (alt) [David Winter, 199x].ch8");

    Chip8::DisplayBuffer display = chip8.getDisplay();

    Renderer renderer = Renderer();

    while (true)
    {
        chip8.emulateCycle();
        if (chip8.getDrawFlag())
        {
            renderer.render(display);
        }
        return 0;
    }
}