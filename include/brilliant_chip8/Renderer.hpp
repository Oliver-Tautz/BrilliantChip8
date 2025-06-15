#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "brilliant_chip8/BrilliantChip8.hpp"

class Renderer
{
public:
    Renderer(); // Constructor
    void render(const BrilliantChip8::DisplayBuffer &);
};

#endif // RENDERER_HPP
