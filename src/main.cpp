#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/Renderer.hpp"
#include <iostream>
#include <filesystem>
#include <spdlog/spdlog.h>
#include "utils.h"
#include <cxxopts.hpp>

spdlog::level::level_enum parseLogLevel(const std::string &level)
{
    static const std::unordered_map<std::string, spdlog::level::level_enum> map = {
        {"off", spdlog::level::off},
        {"critical", spdlog::level::critical},
        {"error", spdlog::level::err},
        {"warn", spdlog::level::warn},
        {"info", spdlog::level::info},
        {"debug", spdlog::level::debug},
        {"trace", spdlog::level::trace}};
    auto it = map.find(level);
    if (it != map.end())
    {
        return it->second;
    }
    spdlog::warn("Unknown log level '{}', defaulting to 'info'", level);
    return spdlog::level::info;
}

int main(int argc, char *argv[])
{
    // Set up CLI options
    cxxopts::Options options("chip8", "A simple CHIP-8 emulator");
    options.add_options()("r,rom", "Path to ROM file", cxxopts::value<std::string>())("l,loglevel", "Log level (off, info, debug, error)",
                                                                                      cxxopts::value<std::string>()->default_value("off"))("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    // Help or missing required
    if (result.count("help") || !result.count("rom"))
    {
        std::cout << options.help() << std::endl;
        return result.count("help") ? 0 : 1;
    }

    // Extract values
    std::string romPath = result["rom"].as<std::string>();
    std::string logLevelStr = result["loglevel"].as<std::string>();

    // Configure logging
    spdlog::set_level(parseLogLevel(logLevelStr));

    // Load ROM
    BrilliantChip8 chip8;
    if (!chip8.loadROM(std::filesystem::path(romPath)))
    {
        spdlog::error("Failed to load ROM: {}", romPath);
        return 1;
    }
    spdlog::info("Loaded ROM: {}", romPath);

    // Renderer loop
    Renderer renderer;
    while (true)
    {
        chip8.emulateCycle();
        auto snapshot = chip8.getStateSnapshot();
        spdlog::info("{}", chip8_snapshot_to_string(snapshot));

        if (chip8.getDrawFlag())
        {
            renderer.render(chip8.getDisplay());
            sleep_ms(16); // ~60 FPS
            chip8.resetDrawFlag();
        }
    }

    return 0;
}
