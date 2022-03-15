#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

#include "chip8.hpp"
#include "display.hpp"

int main(void) {
    Chip8 chip8;
    SDLDisplay display;
    SDL_Event event;

    // initialize SDL
    display.init("CHIP-8", 1024, 512, 64, 32);

    // loading ROM
    chip8.load("roms/IBM_logo.ch8");

    // emulation cycle
    bool running = true;
    while (running) {
        // event loop
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            }
        }

        chip8.emulateCycle();
        display.drawPixels(chip8.getGfx(), GFX_LEN, 64);

        // we need the sleep function to slow down the emulation
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    }

    display.close();

    return 0;
}
