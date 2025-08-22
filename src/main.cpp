#include <spdlog/spdlog.h>

#include <chrono>
#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <thread>

#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/Renderer.hpp"
#include "brilliant_chip8/utils.hpp"

spdlog::level::level_enum parseLogLevel(const std::string &level) {
    static const std::unordered_map<std::string, spdlog::level::level_enum>
        map = {{"off", spdlog::level::off},
               {"critical", spdlog::level::critical},
               {"error", spdlog::level::err},
               {"warn", spdlog::level::warn},
               {"info", spdlog::level::info},
               {"debug", spdlog::level::debug},
               {"trace", spdlog::level::trace}};
    auto it = map.find(level);
    if (it != map.end()) {
        return it->second;
    }
    spdlog::warn("Unknown log level '{}', defaulting to 'info'", level);
    return spdlog::level::info;
}

int main(int argc, char *argv[]) {
    init_console();

    // Set up CLI options
    cxxopts::Options options("chip8", "A simple CHIP-8 emulator");
    options.add_options()("r,rom", "Path to ROM file",
                          cxxopts::value<std::string>())(
        "l,loglevel", "Log level (off, info, debug, error)",
        cxxopts::value<std::string>()->default_value("off"))("h,help",
                                                             "Print usage");

    auto result = options.parse(argc, argv);

    // Help or missing required
    if (result.count("help") || !result.count("rom")) {
        std::cout << options.help() << std::endl;
        return result.count("help") ? 0 : 1;
    }

    // Extract values
    std::string romPath = result["rom"].as<std::string>();
    std::string logLevelStr = result["loglevel"].as<std::string>();

    // Configure logging
    spdlog::set_level(parseLogLevel(logLevelStr));

    using clock = std::chrono::steady_clock;
    using ns = std::chrono::nanoseconds;

    // Constants for CHIP-8 timing

    constexpr double CPU_HZ = 700.0;  // pick what your ROMs expect
    constexpr ns CYCLE_NS = ns{static_cast<long long>(1e9 / CPU_HZ)};
    constexpr ns TICK_NS = ns{16'666'667};  // 1/60 s
    constexpr ns FRAME_NS = TICK_NS;        // same for video

    clock::time_point last_frame_end = clock::now();
    ns cycle_accumulator = ns::zero();
    ns timer_accumulator = ns::zero();
    ns frame_accumulator = ns::zero();

    // Load ROM
    BrilliantChip8 chip8;
    if (!chip8.loadROM(std::filesystem::path(romPath))) {
        spdlog::error("Failed to load ROM: {}", romPath);
        return 1;
    }
    spdlog::info("Loaded ROM: {}", romPath);

    // Renderer loop
    Renderer renderer;
    while (true) {
        clock::time_point frame_start = clock::now();
        ns elapsed = frame_start - last_frame_end;
        last_frame_end = frame_start;

        cycle_accumulator += elapsed;
        timer_accumulator += elapsed;
        frame_accumulator += elapsed;

        while (cycle_accumulator >= CYCLE_NS) {
            chip8.emulateCycle();
            cycle_accumulator -= CYCLE_NS;
        }

        while (timer_accumulator >= TICK_NS) {
            chip8.updateTimers();
            timer_accumulator -= TICK_NS;
        }

        while (frame_accumulator >= FRAME_NS) {
            if (chip8.getDrawFlag()) {
                renderer.render(chip8.getDisplay());
                std::cout << std::flush;
            } else {
                spdlog::debug("No draw flag set, skipping render");
            }
            chip8.resetDrawFlag();

            frame_accumulator -= FRAME_NS;
        }
    }

    return 0;
}
