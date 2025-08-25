# BrilliantChip8
A Chip8 interpeter written in C++


# build on docs from https://github.com/mattmikolay/chip-8/tree/master?tab=readme-ov-file



# TODO

use logging library instead of stdout, stderr etc?


# DevDocs

Install Pakackes:
vcpkg install --x-install-root=./vcpkg-installed

## Dependencies

Install 

* vcpkg
    * windows: follow https://learn.microsoft.com/en-us/vcpkg/get_started/overview
    * linux: follow https://lindevs.com/install-vcpkg-on-ubuntu
* cmake
    * windows: install it systemwide with winget or unzip+add to path
* ninja
    * Windows: winget install Ninja-build.Ninja
* clang compiler
    * Windows: download, extract and add to path: https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8
    
Set VCPKG_ROOT to the root of your vcpkg install. Its read in CMakePresets.json

Build:  
cmake --preset=default && cmake --build --preset=default


or faster with ninja
cmake --preset=default-ninja && cmake --build --preset=default-ninja

Use Vscode:

Instal CMakeTools Extension. Build, run and debug shoulkd work ootb!

Get roms from submodules

git submodule update --init --recursive