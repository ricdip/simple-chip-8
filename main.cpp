#include <chrono>
#include <thread>

#include "chip8.hpp"

int main(void) {
    Chip8 chip8;

    // loading ROM
    chip8.load("roms/IBM_logo.ch8");
    // emulation cycle
    while (true) {
        chip8.emulateCycle();

        // we need the sleep function to slow down the emulation
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    }

    return 0;
}
