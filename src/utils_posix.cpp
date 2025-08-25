#include <chrono>
#include <iostream>
#include <thread>

#include "brilliant_chip8/utils.hpp"

void sleep_ms(unsigned ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void clearScreen() { std::cout << "\033[2J\033[H"; }

void moveCursor(int x, int y) {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}
void init_console() {}