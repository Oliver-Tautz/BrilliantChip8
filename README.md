# BrilliantChip8
A Chip8 interpeter written in C++


# build on docs from https://github.com/mattmikolay/chip-8/tree/master?tab=readme-ov-file



# TODO

use logging library instead of stdout, stderr etc?


# DevDocs

Install Pakackes:
vcpkg install --x-install-root=./vcpkg-installed

Build:  
cmake --preset=default && cmake --build --preset=default


or faster with ninja
cmake --preset=default-ninja && cmake --build --preset=default-ninja

Use Vscode:

Instal CMakeTools Extension. Build, run and debug shoulkd work ootb!