// Moved Opcodes to another file, because it became too much in OpCodeExecutor

#include "brilliant_chip8/OpCodeExecutor.hpp"
#include "brilliant_chip8/BrilliantChip8.hpp"
#include <array>
#include <iostream>
#include <algorithm>

#define CHIP this->chip_ref
#define GET_VX_VY            \
    uint8_t vx = CHIP->V[x]; \
    uint8_t vy = CHIP->V[y];

void OpCodeExecutor::OPCODE_00E0_ClearScreen()
{

    for (std::array<uint8_t, 64> &row : CHIP->gfx)
    {
        std::fill(row.begin(), row.end(), 0);
    }
}
void OpCodeExecutor::OPCODE_00EE_ReturnFromSubroutine()
{
    --CHIP->stack_pointer;
    CHIP->program_counter = CHIP->stack[CHIP->stack_pointer];
}

void OpCodeExecutor::OPCODE_1NNN_JumpToAddress(uint16_t address)
{
    CHIP->program_counter = address;
}

void OpCodeExecutor::OPCODE_2NNN_ExecuteSubroutine(uint16_t address)
{

    CHIP->stack[CHIP->stack_pointer] = CHIP->program_counter;
    ++CHIP->stack_pointer;
    CHIP->program_counter = address;
}

void OpCodeExecutor::OPCODE_3XNN_SkipIfVXEqualsNN(uint8_t x, uint8_t nn)
{

    if (CHIP->V[x] == nn)
    {
        CHIP->skipNextInstruction();
    }
}
void OpCodeExecutor::OPCODE_4XNN_SkipIfVXNotEqualsNN(uint8_t x, uint8_t nn)
{
    if (!CHIP->V[x] == nn)
    {
        CHIP->skipNextInstruction();
    }
}
void OpCodeExecutor::OPCODE_5XY0_SkipIfVXEqualsVY(uint8_t x, uint8_t y)
{

    if (CHIP->V[x] == CHIP->V[y])
    {
        CHIP->skipNextInstruction();
    }
}

void OpCodeExecutor::OPCODE_6XNN_StoreNNinVX(uint8_t x, uint8_t nn)
{

    CHIP->V[x] = nn;
}
void OpCodeExecutor::OPCODE_7XNN_AddNNtoVX(uint8_t x, uint8_t nn)
{

    CHIP->V[x] += nn;
}
void OpCodeExecutor::OPCODE_8XY0_SetVXEqualVY(uint8_t x, uint8_t y)
{

    CHIP->V[x] = CHIP->V[y];
}

void OpCodeExecutor::OPCODE_8XY1_SetVXOrVY(uint8_t x, uint8_t y)
{
    CHIP->V[x] = CHIP->V[x] | CHIP->V[y];
}
void OpCodeExecutor::OPCODE_8XY2_SetVXAndVY(uint8_t x, uint8_t y)
{
    CHIP->V[x] = CHIP->V[x] & CHIP->V[y];
}
void OpCodeExecutor::OPCODE_8XY3_SetVXXorVY(uint8_t x, uint8_t y)
{
    CHIP->V[x] = CHIP->V[x] ^ CHIP->V[y];
}

void OpCodeExecutor::OPCODE_8XY4_AddVYToVXWithCarry(uint8_t x, uint8_t y)
{
    uint16_t sum = CHIP->V[x] + CHIP->V[y];

    CHIP->V[0xF] = (sum > 0xFF) ? 1 : 0;
    CHIP->V[x] = static_cast<uint8_t>(sum & 0xFF);
}

void OpCodeExecutor::OPCODE_8XY5_SubtractVYFromVXWithBorrow(uint8_t x, uint8_t y)
{
    GET_VX_VY

    if (vy > vx)
    {
        CHIP->V[0xF] = 0; // borrow occurred
    }
    else
    {
        CHIP->V[0xF] = 1; // no borrow
    }

    CHIP->V[x] = vx - vy;
}

void OpCodeExecutor::OPCODE_8XY6_ShiftVYRightStoreInVX(uint8_t x, uint8_t y)
{
    uint8_t vy = CHIP->V[y];
    CHIP->V[0xF] = vy & 0x1;
    CHIP->V[x] = vy >> 1;
}
void OpCodeExecutor::OPCODE_8XY7_SubtractVXFromVYWithBorrow(uint8_t x, uint8_t y)
{
    GET_VX_VY

    if (vx > vy)
    {
        CHIP->V[0xF] = 0; // borrow occurred
    }
    else
    {
        CHIP->V[0xF] = 1; // no borrow
    }

    CHIP->V[x] = vy - vx;
}
void OpCodeExecutor::OPCODE_8XYE_ShiftVYLeftStoreInVX(uint8_t x, uint8_t y)

{
    GET_VX_VY
    CHIP->V[0xF] = (vy & 0x80) >> 7;
    CHIP->V[x] = vy << 1;
}

void OpCodeExecutor::OPCODE_9XY0_SkipIfVXNotEqualsVY(uint8_t x, uint8_t y)
{

    GET_VX_VY

    if (vx != vy)
    {
        CHIP->skipNextInstruction();
    }
}
void OpCodeExecutor::OPCODE_ANNN_StoreNNNInI(uint16_t nnn)
{

    CHIP->I = nnn;
}
void OpCodeExecutor::OPCODE_BNNN_JumpToAddressNNPlusV0(uint16_t nnn)
{

    CHIP->program_counter = nnn + CHIP->V[0x0];
}
void OpCodeExecutor::OPCODE_CXNN_SetVXRandomNN(uint8_t x, uint8_t nn)
{
    CHIP->V[x] = rand() & nn;
}

void OpCodeExecutor::OPCODE_DXYN_DrawSprite(uint8_t x, uint8_t y, uint8_t n)
{
    GET_VX_VY
    uint16_t I = CHIP->I;

    // Reset collision flag
    CHIP->V[0xF] = 0;

    for (int i = 0; i < static_cast<int>(n); ++i)
    {
        uint8_t sprite = CHIP->memory[I + i];

        for (int j = 0; j < 8; ++j)
        {
            int pixel = (sprite >> (7 - j)) & 1;

            // Calculate wrapped screen coordinates
            uint8_t screen_x = (vx + j) % 64;
            uint8_t screen_y = (vy + i) % 32;

            // Index into display buffer (assuming 2D array: gfx[64][32])
            if (pixel == 1)
            {
                if (CHIP->gfx[screen_x][screen_y] == 1)
                {
                    CHIP->V[0xF] = 1; // Collision detected
                }

                CHIP->gfx[screen_x][screen_y] ^= 1;
            }
        }
    }
}

void OpCodeExecutor::OPCODE_EX9E_SkipIfVXPressed(uint8_t x)
{

    uint8_t vx = CHIP->V[x];

    if (vx < 16)
    {

        if (CHIP->key[vx])
        {

            CHIP->skipNextInstruction();
        }
    }
    else
    {
        std::cerr << "Warning: Invalid key index in OPCODE_EX9E: " << static_cast<int>(vx) << std::endl;
    }
}
void OpCodeExecutor::OPCODE_EXA1_SkipIfVXNotPressed(uint8_t x)
{

    uint8_t vx = CHIP->V[x];

    if (vx < 16)
    {

        if (!CHIP->key[vx])
        {
            CHIP->skipNextInstruction();
        }
    }
    else
    {
        std::cerr << "Warning: Invalid key index in OPCODE_EX9E: " << static_cast<int>(vx) << std::endl;
    }
}
void OpCodeExecutor::OPCODE_FX07_StoreDelayInVX(uint8_t x)
{

    CHIP->V[x] = CHIP->delay_timer;
}
void OpCodeExecutor::OPCODE_FX0A_WaitForKeyStoreVX(uint8_t x)
{

    // If a key is pressed go on with the Program.
    for (uint8_t i = 0; i < CHIP->CONST_NUMBER_OF_KEYS; ++i)
    {
        if (CHIP->key[i])
        {
            CHIP->V[x] = i;
            return;
        }
    }
    // Stall otherwise
    CHIP->program_counter -= 2;
}
void OpCodeExecutor::OPCODE_FX15_SetDelayWithVX(uint8_t x)
{
    CHIP->delay_timer = CHIP->V[x];
}

void OpCodeExecutor::OPCODE_FX18_SetSoundTimerWithVX(uint8_t x)
{
    CHIP->sound_timer = CHIP->V[x];
}
void OpCodeExecutor::OPCODE_FX1E_AddVXToI(uint8_t x)
{
    CHIP->I += CHIP->V[x];
}
void OpCodeExecutor::OPCODE_FX29_SetSpriteDataVXToI(uint8_t x)
{
    uint8_t digit = CHIP->V[x];

    // Each digit sprite is 5 bytes long, and fontset starts at 0x050
    CHIP->I = CHIP->CONST_SPRITE_START + (digit * 5);
}
void OpCodeExecutor::OPCODE_FX33_StoreBinaryVXI(uint8_t x)
{
    uint8_t vx = CHIP->V[x];

    uint8_t ones = (vx % 10);
    uint8_t tens = (vx / 10) % 10;
    uint8_t hundreds = vx / 100;

    CHIP->memory[CHIP->I] = ones;
    CHIP->memory[CHIP->I + 1] = tens;
    CHIP->memory[CHIP->I + 2] = hundreds;
}
void OpCodeExecutor::OPCODE_FX55_StoreV(uint8_t x)
{

    std::copy_n(CHIP->V.begin(), x + 1, CHIP->memory.begin() + CHIP->I);
    CHIP->I += x + 1;
}
void OpCodeExecutor::OPCODE_FX65_FillV(uint8_t x)
{

    std::copy_n(CHIP->memory.begin() + CHIP->I, x + 1, CHIP->V.begin());
    CHIP->I += x + 1;
}
