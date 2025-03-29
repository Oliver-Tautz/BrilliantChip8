
#include "brilliant_chip8/BrilliantChip8.hpp"



Chip8::Chip8()
{
    Initialize();
}
void Chip8::Initialize()
{
    

    this->drawFlag = false;
    
    this->programCounter = 0x200;
    this->I =0;
    this->sp = 0;
    this->opcode=0;

    this->delay_timer=0;
    this->sound_timer=0;




    std::fill(this->V.begin(), this->V.end(), 0);
    std::fill(this->memory.begin(), this->memory.end(), 0);
    std::fill(this->stack.begin(), this->stack.end(), 0);
    std::fill(this->key.begin(), this->key.end(), 0);
    
    
    for (std::array<uint8_t, 64>&  row : gfx) {
        std::fill(row.begin(), row.end(), 0);
    }

    
    this->LoadFontset();

};

void Chip8::LoadFontset() {


    std::copy(this->CONST_FONTSET.begin(), this->CONST_FONTSET.end(), this->memory.begin() + 0x50);
    

}
