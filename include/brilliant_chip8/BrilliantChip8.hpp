#ifndef BRILLIANT_CHIP8_H
#define BRILLIANT_CHIP8_H

#include <cstdint>
#include <string>
#include <array>
#include <filesystem>
#include <vector>

#include "brilliant_chip8/OpCodeExecutor.hpp"

namespace fs = std::filesystem;

class BrilliantChip8
{
    friend class OpCodeExecutor;
    friend class BrilliantChip8Tests;

public:
    inline static constexpr std::size_t CONST_MEMORY_SIZE = 4096;
    inline static constexpr std::size_t CONST_STACK_SIZE = 16;
    inline static constexpr std::size_t CONST_NUMBER_OF_REGISTERS = 16;
    inline static constexpr std::size_t CONST_DISPLAY_SIZE_X = 64;
    inline static constexpr std::size_t CONST_DISPLAY_SIZE_Y = 32;
    inline static constexpr std::size_t CONST_NUMBER_OF_KEYS = 16;
    inline static constexpr uint16_t CONST_PROGRAM_START = 0x200;
    inline static constexpr uint8_t CONST_SPRITE_START = 0x50;

    using DisplayBuffer = std::array<std::array<uint8_t, CONST_DISPLAY_SIZE_X>, CONST_DISPLAY_SIZE_Y>;

    struct Chip8StateSnapshot
    {
        uint16_t program_counter;
        uint16_t opcode;
        uint16_t I;
        uint8_t stack_pointer;
        uint8_t delay_timer;
        uint8_t sound_timer;
        std::array<uint8_t, 16> V;
        std::array<uint16_t, 16> stack;
        std::array<uint8_t, 16> key;
        BrilliantChip8::DisplayBuffer display;
        bool draw_flag;
    };

    Chip8StateSnapshot getStateSnapshot() const;

    BrilliantChip8();
    void initialize();
    bool loadROM(const fs::path &filepath);
    bool loadROM(const std::vector<uint8_t> &romData);
    void emulateCycle();

    const DisplayBuffer &getDisplay() const;
    void setKey(uint8_t key, bool pressed);
    uint8_t getDrawFlag() const;
    void resetDrawFlag();
    void updateTimers();

private:
    inline static constexpr std::array<uint8_t, 80> CONST_FONTSET = {/* ... */};

    std::array<uint8_t, CONST_MEMORY_SIZE> memory;
    std::array<uint8_t, CONST_NUMBER_OF_REGISTERS> V;
    uint16_t I;
    uint16_t program_counter;

    std::array<uint16_t, CONST_STACK_SIZE> stack;
    uint8_t stack_pointer;

    uint8_t delay_timer;
    uint8_t sound_timer;

    DisplayBuffer gfx;
    std::array<uint8_t, CONST_NUMBER_OF_KEYS> key;

    uint16_t opcode;
    bool draw_flag;

    void loadFontset();
    void clearScreen();
    void skipNextInstruction();

    OpCodeExecutor executor;
};

#endif // BRILLIANT_CHIP8_H