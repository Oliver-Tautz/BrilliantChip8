
// #include "brilliant_chip8/BrilliantChip8.hpp"
//  #include "brilliant_chip8/OpCodeExecutor.hpp"

#include <fstream>
#include <iostream>
#include "brilliant_chip8/BrilliantChip8.hpp"

Chip8::Chip8() : executor(*this)
{
    initialize();
}

void Chip8::initialize()
{

    this->draw_flag = false;

    this->program_counter = 0x200;
    this->index_register_I = 0;
    this->stack_pointer = 0;
    this->opcode = 0;

    this->delay_timer = 0;
    this->sound_timer = 0;

    std::fill(this->V_general_purpose_register.begin(), this->V_general_purpose_register.end(), 0);
    std::fill(this->memory.begin(), this->memory.end(), 0);
    std::fill(this->stack.begin(), this->stack.end(), 0);
    std::fill(this->key.begin(), this->key.end(), 0);

    for (std::array<uint8_t, 64> &row : gfx)
    {
        std::fill(row.begin(), row.end(), 0);
    }

    this->loadFontset();
};

void Chip8::loadFontset()
{

    std::copy(this->CONST_FONTSET.begin(), this->CONST_FONTSET.end(), this->memory.begin() + 0x50);
}

bool Chip8::loadROM(const fs::path &filepath)
{

    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // ate = start at end
    if (!file.is_open())
    {
        std::cerr << "Failed to open ROM: " << filepath << "\n";
        return false;
    }

    std::streamsize size = file.tellg(); // get file size
    if (size > (4096 - 0x200))
    {
        std::cerr << "ROM too large to fit in memory!\n";
        return false;
    }

    file.seekg(0, std::ios::beg); // rewind to beginning
    file.read(reinterpret_cast<char *>(&memory[0x200]), size);

    if (!file)
    {
        std::cerr << "Failed to read ROM contents!\n";
        return false;
    }

    return true;
}

const Chip8::DisplayBuffer &Chip8::getDisplay() const
{
    return this->gfx;
}

uint8_t Chip8::getDrawFlag() const
{
    return this->draw_flag;
}

void Chip8::emulateCycle()
{
    // 1. Fetch
    this->opcode = (memory[program_counter] << 8) | memory[program_counter + 1];
    this->program_counter += 2;

    // 3. Execute
    this->executor.execute(opcode);

    // 4. Update timers
}
