#include "utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

void printStreamBytes(std::ifstream &stream, std::size_t maxBytes)
{
    auto originalPos = stream.tellg();

    if (!stream || !stream.good())
    {
        std::cerr << "Stream is not valid or not open\n";
        return;
    }

    unsigned char byte;
    std::size_t count = 0;

    while (stream.read(reinterpret_cast<char *>(&byte), 1))
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byte) << ' ';

        if (++count % 16 == 0)
            std::cout << '\n';

        if (maxBytes > 0 && count >= maxBytes)
            break;
    }

    std::cout << std::dec << std::endl;
    stream.clear();
    stream.seekg(originalPos);
}

std::string to_string(const BrilliantChip8::Chip8StateSnapshot &snapshot)
{
    std::ostringstream oss;
    oss << "=== Chip8 Snapshot ===\n";
    oss << "PC: " << std::hex << snapshot.program_counter << "\n";
    oss << "Opcode: " << std::hex << snapshot.opcode << "\n";
    oss << "I: " << std::hex << snapshot.I << "\n";
    oss << "SP: " << std::dec << static_cast<int>(snapshot.stack_pointer) << "\n";
    oss << "Delay Timer: " << static_cast<int>(snapshot.delay_timer)
        << ", Sound Timer: " << static_cast<int>(snapshot.sound_timer) << "\n";

    oss << "V: ";
    for (auto v : snapshot.V)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(v) << ' ';
    oss << "\n";

    oss << "Stack: ";
    for (auto s : snapshot.stack)
        oss << std::hex << s << ' ';
    oss << "\n";

    oss << "Keys: ";
    for (auto k : snapshot.key)
        oss << static_cast<int>(k) << ' ';
    oss << "\n";

    oss << "Draw flag: " << (snapshot.draw_flag ? "true" : "false") << "\n";
    return oss.str();
}
