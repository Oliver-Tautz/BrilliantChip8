
// #include "brilliant_chip8/BrilliantChip8.hpp"
//  #include "brilliant_chip8/OpCodeExecutor.hpp"

#include <fstream>
#include <iostream>
#include "brilliant_chip8/BrilliantChip8.hpp"

BrilliantChip8::BrilliantChip8() : executor(*this)
{
    this->initialize();
}

void BrilliantChip8::initialize()
{

    this->draw_flag = false;

    this->program_counter = 0x200;
    this->I = 0;
    this->stack_pointer = 0;
    this->opcode = 0;

    this->delay_timer = 0;
    this->sound_timer = 0;

    std::fill(this->V.begin(), this->V.end(), 0);
    std::fill(this->memory.begin(), this->memory.end(), 0);
    std::fill(this->stack.begin(), this->stack.end(), 0);
    std::fill(this->key.begin(), this->key.end(), 0);

    this->clearScreen();
    this->loadFontset();
};

void BrilliantChip8::loadFontset()
{

    std::copy(this->CONST_FONTSET.begin(), this->CONST_FONTSET.end(), this->memory.begin() + 0x50);
}

bool BrilliantChip8::loadROM(const fs::path &filepath)
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

bool BrilliantChip8::loadROM(const std::vector<uint8_t> &romData)
{
    if (romData.size() + 0x200 > sizeof(memory))
        return false; // too large

    for (size_t i = 0; i < romData.size(); ++i)
    {
        memory[0x200 + i] = romData[i];
    }
    return true;
}

const BrilliantChip8::DisplayBuffer &BrilliantChip8::getDisplay() const
{
    return this->gfx;
}

uint8_t BrilliantChip8::getDrawFlag() const
{
    return this->draw_flag;
}

void BrilliantChip8::resetDrawFlag()
{
    this->draw_flag = 0;
}

void BrilliantChip8::updateTimers()
{
    if (this->delay_timer > 0)
        --this->delay_timer;
    if (this->sound_timer > 0)
        --this->sound_timer;
}

void BrilliantChip8::emulateCycle()
{
    // 1. Fetch
    this->opcode = (memory[program_counter] << 8) | memory[program_counter + 1];
    this->program_counter += 2;

    // 2. Execute
    this->executor.execute(opcode);
}

void BrilliantChip8::clearScreen()
{
    for (auto &row : this->gfx)
        std::fill(row.begin(), row.end(), 0);

    this->draw_flag = true;
}

BrilliantChip8::Chip8StateSnapshot BrilliantChip8::getStateSnapshot() const
{
    Chip8StateSnapshot snapshot;

    snapshot.program_counter = this->program_counter;
    snapshot.opcode = this->opcode;
    snapshot.I = this->I;
    snapshot.stack_pointer = this->stack_pointer;
    snapshot.delay_timer = this->delay_timer;
    snapshot.sound_timer = this->sound_timer;
    snapshot.V = this->V;
    snapshot.stack = this->stack;
    snapshot.key = this->key;
    snapshot.display = this->gfx;
    snapshot.draw_flag = this->draw_flag;

    return snapshot;
}
