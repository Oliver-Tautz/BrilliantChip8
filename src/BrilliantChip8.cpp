
#include "brilliant_chip8/BrilliantChip8.hpp"
#include <fstream>
#include <iostream>

Chip8::Chip8()
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

void Chip8::wrongOpcode()
{
    std::cerr << "Unknown opcode: " << std::hex << this->opcode << std::endl;
}

void Chip8::emulateCycle()
{
    // 1. Fetch
    this->opcode = (memory[program_counter] << 8) | memory[program_counter + 1];
    this->program_counter += 2;

    // 2. Decode

    uint8_t first_nibble = (opcode & 0xF000) >> 12; // top 4 bits (instruction category)
    uint8_t x = (opcode & 0x0F00) >> 8;             // 2nd nibble (register X)
    uint8_t y = (opcode & 0x00F0) >> 4;             // 3rd nibble (register Y)
    uint8_t n = opcode & 0x000F;                    // 4th nibble (often a size or low nibble) n
    uint8_t nn = opcode & 0x00FF;                   // lowest byte (immediate value)
    uint16_t nnn = opcode & 0x0FFF;                 // lowest 12 bits (address)

    switch (first_nibble)
    {
    case 0x0:
        switch (nnn)
        {
        case 0x0E0:
            // 00E0
            this->clearScreen();
            break;
        case 0x0EE:
            // 00EE
            this->returnFromSubroutine();
            break;
        default:
            this->wrongOpcode();
            break;
        }

        break;
    case 0x1:
        // 1NNN
        this->jumpToAddress(nnn);
        break;
    case 0x2:
        // 2NNN
        this->executeSubroutine(nnn);
        break;
    case 0x3:
        // 3XNN
        this->skipIfVXequalsNN(x, nn);
        break;
    case 0x4:
        // 4XNN
        this->skipIfVXequalsNNNot(x, nn);
        break;
    case 0x5:
        // 5XY0
        if (n != 0x0)
        {
            this->wrongOpcode();
        }
        else
        {
            this->skipIfVXEqualsVY(x, y);
        }
        break;
    case 0x6:
        // 6XNN
        this->storeNNinVX(x, nn);
        break;
    case 0x7:
        // 7XNN
        this->addNNtoVX(x, nn);
        break;
    case 0x8:
        switch (n)
        {
        case 0x0:
            // 8XY0: Set VX = VY
            this->V_general_purpose_register[x] = V_general_purpose_register[y];
            break;
        case 0x2:
            // 8XY2: Set VX = VX AND VY
            break;
        case 0x3:
            // 8XY3: Set VX = VX XOR VY
            break;
        case 0x4:
            // 8XY4: Set VX = VX + VY, set VF = carry
            break;
        case 0x5:
            // 8XY5: Set VX = VX - VY, set VF = NOT borrow
            break;
        case 0x6:
            // 8XY6: Set VX = VY >> 1, VF = least significant bit before shift
            break;
        case 0x7:
            // 8XY7: Set VX = VY - VX, set VF = NOT borrow
            break;
        case 0xE:
            // 8XYE: Set VX = VY << 1, VF = most significant bit before shift
            break;
        default:
            // Unknown 8XY_ opcode
            break;
        }
    case 0x9:
        if (n == 0)
        {
            // 9XY0
            this->skipIfVXEqualsVYNot(x, y);
        }
        else
        {
            this->wrongOpcode();
        }
        break;
    case 0xA:
        // ANNN
        this->storeNNNInI(nnn);
        break;
    case 0xB:
        // BNNN
        this->jumpToAddressNNPlusV0(nn);
        break;
    case 0xC:
        // CXNN
        this->setVXRandomNN(x, nn);
        break;
    case 0xD:
        this->drawSprite(x, y, n);
        break;
    case 0xE:
        switch (nn)
        {
        case 0x9E:
            // EX9E
            this->skipIfVXPressed();
            break;

        case 0xA1:
            // EXA1
            this->skipIfVXPressedNot();
            break;
        default:
            this->wrongOpcode();
            break;
        }
        break;
    case 0xF:
        switch (nn)
        {
        case 0x07:
            // FX07
            break;
            this->storeDelayInVX();
        case 0x0A:
            // FX0A
            this->waitForKeyStoreVX();
            break;
        case 0x15:
            // FX15
            this->setDelayWithVX();
            break;
        case 0x18:
            // FX18
            this->setSoundTimerWithVX();
            break;
        case 0x1E:
            // FX1E
            this->addVXtoI();
            break;
        case 0x29:
            // FX29
            this->setSpriteDataVXtoI();
            break;
        case 0x33:
            // FX33
            this->storeBinaryVXI();
            break;
        case 0x55:
            // FX55
            this->storeV();
            break;
        case 0x65:
            // FX65
            this->fillV();
            break;
        default:
            this->wrongOpcode();
        }
        break;

    default:
        // code if no case matches
        this->wrongOpcode();
        break;
    }

    // 3. Execute

    // 4. Update timers
}
