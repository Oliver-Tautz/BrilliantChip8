#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <array>



class Chip8
{
public:
    Chip8();
    void Initialize();
    bool LoadROM(const std::string &filename);
    void EmulateCycle();

    // Accessors for display and input
    const uint8_t *GetDisplay() const;
    void SetKey(uint8_t key, bool pressed);

private:
    // Memory and registers
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, 16> V;           // General purpose registers V0–VF
    uint16_t I;                          // Index register
    uint16_t programCounter;             // Program counter

    // Stack
    std::array<uint16_t, 16> stack;
    uint8_t sp;                          // Stack pointer

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Input and display
    std::array<std::array<uint8_t, 64>, 32> gfx;   // Monochrome display (on/off pixels)
    std::array<uint8_t, 16> key;                   // Keypad state (0 = released, 1 = pressed)

    // Current opcode
    uint16_t opcode;

    // Fontset (loaded into memory at 0x000–0x050)
    void LoadFontset();
    bool drawFlag;

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


};

#endif // CHIP8_H
