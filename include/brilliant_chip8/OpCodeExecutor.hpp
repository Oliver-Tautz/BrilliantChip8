#ifndef OPCODEEXECUTOR_H
#define OPCODEEXECUTOR_H

#include <cstdint>
// #include "brilliant_chip8/BrilliantChip8.hpp"
class BrilliantChip8;

class OpCodeExecutor
{

public:
    OpCodeExecutor(BrilliantChip8 &chipRef);

    void execute(uint16_t opcode);

private:
    // Chip8 with loaded program
    BrilliantChip8 *chip_ref;

    // Decoded instruction fields
    uint16_t current_opcode;
    uint8_t x, y, n, nn;
    uint16_t nnn;
    uint8_t first_nibble;

    // Opcodes
    // Note: Opcode implementations are defined in OpCodes.cpp for clarity.

    void wrongOpcode();
    void OPCODE_00E0_ClearScreen();
    void OPCODE_00EE_ReturnFromSubroutine();
    void OPCODE_1NNN_JumpToAddress(uint16_t address);
    void OPCODE_2NNN_ExecuteSubroutine(uint16_t address);
    void OPCODE_3XNN_SkipIfVXEqualsNN(uint8_t x, uint8_t nn);
    void OPCODE_4XNN_SkipIfVXNotEqualsNN(uint8_t x, uint8_t nn);
    void OPCODE_5XY0_SkipIfVXEqualsVY(uint8_t x, uint8_t y);
    void OPCODE_6XNN_StoreNNinVX(uint8_t x, uint8_t nn);
    void OPCODE_7XNN_AddNNtoVX(uint8_t x, uint8_t nn);
    void OPCODE_8XY0_SetVXEqualVY(uint8_t x, uint8_t y);
    void OPCODE_8XY1_SetVXOrVY(uint8_t x, uint8_t y);
    void OPCODE_8XY2_SetVXAndVY(uint8_t x, uint8_t y);
    void OPCODE_8XY3_SetVXXorVY(uint8_t x, uint8_t y);
    void OPCODE_8XY4_AddVYToVXWithCarry(uint8_t x, uint8_t y);
    void OPCODE_8XY5_SubtractVYFromVXWithBorrow(uint8_t x, uint8_t y);
    void OPCODE_8XY6_ShiftVYRightStoreInVX(uint8_t x, uint8_t y);
    void OPCODE_8XY7_SubtractVXFromVYWithBorrow(uint8_t x, uint8_t y);
    void OPCODE_8XYE_ShiftVYLeftStoreInVX(uint8_t x, uint8_t y);
    void OPCODE_9XY0_SkipIfVXNotEqualsVY(uint8_t x, uint8_t y);
    void OPCODE_ANNN_StoreNNNInI(uint16_t nnn);
    void OPCODE_BNNN_JumpToAddressNNPlusV0(uint16_t nnn);
    void OPCODE_CXNN_SetVXRandomNN(uint8_t x, uint8_t nn);
    void OPCODE_DXYN_DrawSprite(uint8_t x, uint8_t y, uint8_t n);
    void OPCODE_EX9E_SkipIfVXPressed(uint8_t x);
    void OPCODE_EXA1_SkipIfVXNotPressed(uint8_t x);
    void OPCODE_FX07_StoreDelayInVX(uint8_t x);
    void OPCODE_FX0A_WaitForKeyStoreVX(uint8_t x);
    void OPCODE_FX15_SetDelayWithVX(uint8_t x);
    void OPCODE_FX18_SetSoundTimerWithVX(uint8_t x);
    void OPCODE_FX1E_AddVXToI(uint8_t x);
    void OPCODE_FX29_SetSpriteDataVXToI(uint8_t x);
    void OPCODE_FX33_StoreBinaryVXI(uint8_t x);
    void OPCODE_FX55_StoreV(uint8_t x);
    void OPCODE_FX65_FillV(uint8_t x);
};

#endif // OPCODEEXECUTOR_H
