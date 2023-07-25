#include "emulation/chip8.hpp"
#include "graphics/display.hpp"

// screen width
#define WINDOW_WIDTH 1024
// screen height
#define WINDOW_HEIGHT 512
// frame per second = fps
#define FPS 60
// millisecond / fps
#define MS_PER_CYCLE (1000.f / FPS)

int main(void) {
  Chip8 chip8;
  SDLDisplay display;
  SDL_Event event;

  // initialize SDL
  display.init("CHIP-8", WINDOW_WIDTH, WINDOW_HEIGHT, GFX_WIDTH, GFX_HEIGHT);

  // loading ROM
  chip8.load("roms/IBM_logo.ch8");

  // emulation cycle
  bool running = true;
  double startTick, endTick, frameSpeed;
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

    // timer start
    startTick = SDL_GetTicks();
    // do one emulator cycle
    chip8.emulateCycle();
    // draw flag is set
    if (chip8.drawFlag) {
      // unset flag
      chip8.drawFlag = false;
      // update screen
      display.drawPixels(chip8.getGfx(), GFX_LEN, GFX_WIDTH);
    }
    // timer end
    endTick = SDL_GetTicks();
    // frame speed = time delta (end - start)
    frameSpeed = endTick - startTick;

    // we need the sleep function to slow down the emulation
    if (frameSpeed < MS_PER_CYCLE) {
      SDL_Delay(MS_PER_CYCLE - frameSpeed);
    }
  }

  display.close();

  return 0;
}
