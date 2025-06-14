#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/Renderer.hpp"
#include <iostream>
#include <filesystem>
#include <spdlog/spdlog.h>
#include "utils.h"

int main()
{

    // TODO make this a input parameter
    spdlog::set_level(spdlog::level::info);

    BrilliantChip8 chip8;

    chip8.loadROM(std::filesystem::path("external/roms/roms/demos") / "Maze (alt) [David Winter, 199x].ch8");

    BrilliantChip8::DisplayBuffer display = chip8.getDisplay();

    Renderer renderer = Renderer();

    while (true)
    {
        // Get the current state snapshot (for debugging or logging)

        chip8.emulateCycle();
        auto snapshot = chip8.getStateSnapshot();
        spdlog::info("{}", to_string(snapshot));
        if (chip8.getDrawFlag())
        {
            renderer.render(display);
            sleep(1); // Sleep for 100 milliseconds
            chip8.resetDrawFlag();
        }
        // return 0;
    }
}