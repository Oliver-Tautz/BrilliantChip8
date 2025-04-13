// Moved Opcodes to another file, because it became too much in OpCodeExecutor

#include "brilliant_chip8/OpCodeExecutor.hpp"

void OpCodeExecutor::OPCODE_00E0_ClearScreen() {}
void OpCodeExecutor::OPCODE_00EE_ReturnFromSubroutine() {}
void OpCodeExecutor::OPCODE_1NNN_JumpToAddress(uint16_t address) {}
void OpCodeExecutor::OPCODE_2NNN_ExecuteSubroutine(uint16_t address) {}
void OpCodeExecutor::OPCODE_3XNN_SkipIfVXEqualsNN(uint8_t x, uint8_t nn) {}
void OpCodeExecutor::OPCODE_4XNN_SkipIfVXNotEqualsNN(uint8_t x, uint8_t nn) {}
void OpCodeExecutor::OPCODE_5XY0_SkipIfVXEqualsVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_6XNN_StoreNNinVX(uint8_t x, uint8_t nn) {}
void OpCodeExecutor::OPCODE_7XNN_AddNNtoVX(uint8_t x, uint8_t nn) {}
void OpCodeExecutor::OPCODE_8XY0_SetVXEqualVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY1_SetVXOrVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY2_SetVXAndVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY3_SetVXXorVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY4_AddVYToVXWithCarry(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY5_SubtractVYFromVXWithBorrow(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY6_ShiftVYRightStoreInVX(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XY7_SubtractVXFromVYWithBorrow(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_8XYE_ShiftVYLeftStoreInVX(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_9XY0_SkipIfVXNotEqualsVY(uint8_t x, uint8_t y) {}
void OpCodeExecutor::OPCODE_ANNN_StoreNNNInI(uint16_t nnn) {}
void OpCodeExecutor::OPCODE_BNNN_JumpToAddressNNPlusV0(uint16_t nnn) {}
void OpCodeExecutor::OPCODE_CXNN_SetVXRandomNN(uint8_t x, uint8_t nn) {}
void OpCodeExecutor::OPCODE_DXYN_DrawSprite(uint8_t x, uint8_t y, uint8_t n) {}
void OpCodeExecutor::OPCODE_EX9E_SkipIfVXPressed(uint8_t x) {}
void OpCodeExecutor::OPCODE_EXA1_SkipIfVXNotPressed(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX07_StoreDelayInVX(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX0A_WaitForKeyStoreVX(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX15_SetDelayWithVX(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX18_SetSoundTimerWithVX(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX1E_AddVXToI(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX29_SetSpriteDataVXToI(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX33_StoreBinaryVXI(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX55_StoreV(uint8_t x) {}
void OpCodeExecutor::OPCODE_FX65_FillV(uint8_t x) {}