#include "chip8.hpp"

int main(void) {
    Chip8 chip8;

    chip8.load("roms/IBM_logo.ch8");

    return 0;
}
