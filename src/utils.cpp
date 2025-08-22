#include "brilliant_chip8/utils.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

void sleep_ms(unsigned ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void printStreamBytes(std::ifstream &stream, std::size_t maxBytes) {
    auto originalPos = stream.tellg();

    if (!stream || !stream.good()) {
        std::cerr << "Stream is not valid or not open\n";
        return;
    }

    unsigned char byte;
    std::size_t count = 0;

    while (stream.read(reinterpret_cast<char *>(&byte), 1)) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byte) << ' ';

        if (++count % 16 == 0) std::cout << '\n';

        if (maxBytes > 0 && count >= maxBytes) break;
    }

    std::cout << std::dec << std::endl;
    stream.clear();
    stream.seekg(originalPos);
}

std::string chip8_snapshot_to_string(
    const BrilliantChip8::Chip8StateSnapshot &snapshot) {
    std::ostringstream oss;
    oss << "=== Chip8 Snapshot ===\n";
    oss << "PC: " << std::hex << snapshot.program_counter << "\n";
    oss << "Opcode: " << std::hex << snapshot.opcode << "\n";
    oss << "I: " << std::hex << snapshot.I << "\n";
    oss << "SP: " << std::dec << static_cast<int>(snapshot.stack_pointer)
        << "\n";
    oss << "Delay Timer: " << static_cast<int>(snapshot.delay_timer)
        << ", Sound Timer: " << static_cast<int>(snapshot.sound_timer) << "\n";

    oss << "V: ";
    for (auto v : snapshot.V)
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(v) << ' ';
    oss << "\n";

    oss << "Stack: ";
    for (auto s : snapshot.stack) oss << std::hex << s << ' ';
    oss << "\n";

    oss << "Keys: ";
    for (auto k : snapshot.key) oss << static_cast<int>(k) << ' ';
    oss << "\n";

    oss << "Draw flag: " << (snapshot.draw_flag ? "true" : "false") << "\n";
    return oss.str();
}

#ifdef _WIN32
#include <windows.h>

void enableVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void clearScreen() {
    static bool vtEnabled = false;
    if (!vtEnabled) {
        enableVirtualTerminal();
        vtEnabled = true;
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    if (GetConsoleMode(hOut, &dwMode) &&
        (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cout << "\033[2J\033[H";
    } else {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hOut, ' ', cellCount, homeCoords, &count);
        FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount,
                                   homeCoords, &count);
        SetConsoleCursorPosition(hOut, homeCoords);
    }
}

void moveCursor(int x, int y) {
    static bool vtEnabled = false;
    if (!vtEnabled) {
        enableVirtualTerminal();
        vtEnabled = true;
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    if (GetConsoleMode(hOut, &dwMode) &&
        (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    } else {
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hOut, pos);
    }
}

#else  // Linux/macOS

void clearScreen() { std::cout << "\033[2J\033[H"; }

void moveCursor(int x, int y) {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}

#endif
