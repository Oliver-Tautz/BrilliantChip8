#pragma once

#include <fstream>
#include <string>
#include "brilliant_chip8/BrilliantChip8.hpp"

void printStreamBytes(std::ifstream &stream, std::size_t maxBytes);
void sleep_ms(unsigned ms);

// Convert a snapshot into a readable string
std::string chip8_snapshot_to_string(const BrilliantChip8::Chip8StateSnapshot &snapshot);
