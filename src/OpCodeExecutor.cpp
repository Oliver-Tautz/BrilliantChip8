
#include <iostream>
#include "brilliant_chip8/OpCodeExecutor.hpp"
#include "brilliant_chip8/BrilliantChip8.hpp"

OpCodeExecutor::OpCodeExecutor(Chip8 &chipRef)
{
    this->chip_ref = &chipRef;
}

void OpCodeExecutor::wrongOpcode()
{
    std::cerr << "Unknown opcode: " << std::hex << chip_ref->opcode << std::endl;
}

void OpCodeExecutor::execute(uint16_t opcode)
{
    uint8_t first_nibble = (opcode & 0xF000) >> 12;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;

    switch (first_nibble)
    {
    case 0x0:
        switch (nnn)
        {
        case 0x0E0:
            this->OPCODE_00E0_ClearScreen();
            break;
        case 0x0EE:
            this->OPCODE_00EE_ReturnFromSubroutine();
            break;
        default:
            this->wrongOpcode();
            break;
        }
        break;

    case 0x1:
        this->OPCODE_1NNN_JumpToAddress(nnn);
        break;

    case 0x2:
        this->OPCODE_2NNN_ExecuteSubroutine(nnn);
        break;

    case 0x3:
        this->OPCODE_3XNN_SkipIfVXEqualsNN(x, nn);
        break;

    case 0x4:
        this->OPCODE_4XNN_SkipIfVXNotEqualsNN(x, nn);
        break;

    case 0x5:
        if (n == 0x0)
        {
            this->OPCODE_5XY0_SkipIfVXEqualsVY(x, y);
        }
        else
        {
            this->wrongOpcode();
        }
        break;

    case 0x6:
        this->OPCODE_6XNN_StoreNNinVX(x, nn);
        break;

    case 0x7:
        this->OPCODE_7XNN_AddNNtoVX(x, nn);
        break;

    case 0x8:
        switch (n)
        {
        case 0x0:
            this->OPCODE_8XY0_SetVXEqualVY(x, y);
            break;
        case 0x1:
            this->OPCODE_8XY1_SetVXOrVY(x, y);
            break;
        case 0x2:
            this->OPCODE_8XY2_SetVXAndVY(x, y);
            break;
        case 0x3:
            this->OPCODE_8XY3_SetVXXorVY(x, y);
            break;
        case 0x4:
            this->OPCODE_8XY4_AddVYToVXWithCarry(x, y);
            break;
        case 0x5:
            this->OPCODE_8XY5_SubtractVYFromVXWithBorrow(x, y);
            break;
        case 0x6:
            this->OPCODE_8XY6_ShiftVYRightStoreInVX(x, y);
            break;
        case 0x7:
            this->OPCODE_8XY7_SubtractVXFromVYWithBorrow(x, y);
            break;
        case 0xE:
            this->OPCODE_8XYE_ShiftVYLeftStoreInVX(x, y);
            break;
        default:
            this->wrongOpcode();
            break;
        }
        break;

    case 0x9:
        if (n == 0x0)
        {
            this->OPCODE_9XY0_SkipIfVXNotEqualsVY(x, y);
        }
        else
        {
            this->wrongOpcode();
        }
        break;

    case 0xA:
        this->OPCODE_ANNN_StoreNNNInI(nnn);
        break;

    case 0xB:
        this->OPCODE_BNNN_JumpToAddressNNPlusV0(nnn);
        break;

    case 0xC:
        this->OPCODE_CXNN_SetVXRandomNN(x, nn);
        break;

    case 0xD:
        this->OPCODE_DXYN_DrawSprite(x, y, n);
        break;

    case 0xE:
        switch (nn)
        {
        case 0x9E:
            this->OPCODE_EX9E_SkipIfVXPressed(x);
            break;
        case 0xA1:
            this->OPCODE_EXA1_SkipIfVXNotPressed(x);
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
            this->OPCODE_FX07_StoreDelayInVX(x);
            break;
        case 0x0A:
            this->OPCODE_FX0A_WaitForKeyStoreVX(x);
            break;
        case 0x15:
            this->OPCODE_FX15_SetDelayWithVX(x);
            break;
        case 0x18:
            this->OPCODE_FX18_SetSoundTimerWithVX(x);
            break;
        case 0x1E:
            this->OPCODE_FX1E_AddVXToI(x);
            break;
        case 0x29:
            this->OPCODE_FX29_SetSpriteDataVXToI(x);
            break;
        case 0x33:
            this->OPCODE_FX33_StoreBinaryVXI(x);
            break;
        case 0x55:
            this->OPCODE_FX55_StoreV(x);
            break;
        case 0x65:
            this->OPCODE_FX65_FillV(x);
            break;
        default:
            this->wrongOpcode();
            break;
        }
        break;

    default:
        this->wrongOpcode();
        break;
    }
}
