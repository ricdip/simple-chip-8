#include <SDL2/SDL.h>

#include "chip8.hpp"
#include "display.hpp"

#define FPS 60
#define MS_PER_CYCLE 1000 / FPS

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
    unsigned int startTick, endTick, frameSpeed;
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

        startTick = SDL_GetTicks();
        chip8.emulateCycle();
        if (chip8.drawFlag) {
            chip8.drawFlag = false;
            display.drawPixels(chip8.getGfx(), GFX_LEN, 64);
        }
        endTick = SDL_GetTicks();
        frameSpeed = endTick - startTick;

        // we need the sleep function to slow down the emulation
        if (frameSpeed < MS_PER_CYCLE) {
            SDL_Delay(MS_PER_CYCLE - frameSpeed);
        }
    }

    display.close();

    return 0;
}
