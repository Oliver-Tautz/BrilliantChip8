#include <string>
#include <cstdint>

class Chip8
{
public:
    void Initialize();
    void LoadROM(const std::string &filename);
    void EmulateCycle();
    void SetKeys();
    uint8_t gfx[64 * 32]; // Make accessible to your renderer

private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t key[16];
    uint16_t opcode;
};
