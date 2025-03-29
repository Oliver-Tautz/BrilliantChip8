#include "brilliant_chip8/greeter.hpp"
#include <iostream>

Greeter::Greeter(const std::string &name) : name(name) {}

void Greeter::say_hello() const
{
    std::cout << "Hello, " << name << "!" << std::endl;
}
