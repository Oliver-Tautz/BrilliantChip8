#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "brilliant_chip8/utils.hpp"

static void enableVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}

void sleep_ms(unsigned ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void clearScreen() {
    static bool vtOnce = (enableVirtualTerminal(), true);
    (void)vtOnce;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hOut, &mode) &&
        (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cout << "\033[2J\033[H";
    } else {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
        const DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        DWORD count;
        COORD home = {0, 0};
        FillConsoleOutputCharacter(hOut, ' ', cellCount, home, &count);
        FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, home,
                                   &count);
        SetConsoleCursorPosition(hOut, home);
    }
}

void init_console() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) return;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, mode);

    // If you use block Unicode chars (█ ░ ▒ ▓), set UTF-8:
    SetConsoleOutputCP(CP_UTF8);
}

void moveCursor(int x, int y) {
    static bool vtOnce = (enableVirtualTerminal(), true);
    (void)vtOnce;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hOut, &mode) &&
        (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    } else {
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hOut, pos);
    }
}
