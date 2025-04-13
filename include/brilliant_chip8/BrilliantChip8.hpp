#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <array>
#include <filesystem>

#include "brilliant_chip8/OpCodeExecutor.hpp"

namespace fs = std::filesystem;

class Chip8
{

    friend class OpCodeExecutor;

public:
    inline static constexpr std::size_t CONST_DISPLAY_SIZE_X = 64;
    inline static constexpr std::size_t CONST_DISPLAY_SIZE_Y = 32;
    inline static constexpr std::size_t CONST_NUMBER_OF_KEYS = 16;

    using DisplayBuffer = std::array<std::array<uint8_t, CONST_DISPLAY_SIZE_X>, CONST_DISPLAY_SIZE_Y>;

    Chip8();
    void initialize();
    bool loadROM(const fs::path &filepath);
    void emulateCycle();

    // Accessors for display and input
    const DisplayBuffer &getDisplay() const;
    void setKey(uint8_t key, bool pressed);

    uint8_t getDrawFlag() const;

private:
    // Constants
    inline static constexpr std::size_t CONST_MEMORY_SIZE = 4096;
    inline static constexpr std::size_t CONST_STACK_SIZE = 16;
    inline static constexpr std::size_t CONST_NUMBER_OF_REGISTERS = 16;

    inline static constexpr std::array<uint8_t, 80> CONST_FONTSET = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Memory and registers
    std::array<uint8_t, CONST_MEMORY_SIZE> memory;
    std::array<uint8_t, CONST_NUMBER_OF_REGISTERS> V_general_purpose_register; // General purpose registers V0–VF
    uint16_t index_register_I;                                                 // Index register
    uint16_t program_counter;                                                  // Program counter

    // Stack
    std::array<uint16_t, CONST_NUMBER_OF_KEYS> stack;
    uint8_t stack_pointer; // Stack pointer

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Input and display
    DisplayBuffer gfx;                         // Monochrome display (on/off pixels)
    std::array<uint8_t, CONST_STACK_SIZE> key; // Keypad state (0 = released, 1 = pressed)

    // Current opcode
    uint16_t opcode;

    bool draw_flag;

    // Fontset (loaded into memory at 0x000–0x050)
    void loadFontset();
    void clearScreen();

    OpCodeExecutor executor;
};

#endif // CHIP8_H
