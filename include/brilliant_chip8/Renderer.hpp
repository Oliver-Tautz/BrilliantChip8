#pragma once

#include "brilliant_chip8/BrilliantChip8.hpp"

class Renderer {
   public:
    Renderer();  // Constructor
    void render(const BrilliantChip8::DisplayBuffer &);
};
