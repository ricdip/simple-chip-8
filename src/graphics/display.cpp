#include "display.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

void SDLDisplay::throwError(std::string error) {
  throw std::runtime_error("SDL Error: " + error);
}

void SDLDisplay::initSDLWindow(std::string title, int windowWidth,
                               int windowHeight) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throwError(SDL_GetError());
  } else {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);
    width = windowWidth;
    height = windowHeight;
    if (window == NULL) {
      throwError(SDL_GetError());
    }
  }
}

void SDLDisplay::initSDLRenderer() {
  // renderer creation:
  //  - window: window where rendering is displayed
  //  - index: index of the rendering driver to inizialize or -1 to inizialize
  //  the first one supporting the flags
  //  - flags: 0 or one or mode flags
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    throwError(SDL_GetError());
  } else {
    if (SDL_RenderSetLogicalSize(renderer, width, height) < 0) {
      throwError(SDL_GetError());
    }
  }
}

void SDLDisplay::initSDLTexture(int textureWidth, int textureHeight) {
  // ARGB8888: 4 channels (alpha, red, green, blue), each channel has 8 bit
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, textureWidth,
                              textureHeight);
  if (texture == NULL) {
    throwError(SDL_GetError());
  }
}

void SDLDisplay::init(std::string title, int windowWidth, int windowHeight,
                      int textureWidth, int textureHeight) {
  initSDLWindow(title, windowWidth, windowHeight);
  initSDLRenderer();
  initSDLTexture(textureWidth, textureHeight);
}

void SDLDisplay::drawPixels(const bool *gfx, unsigned int size,
                            int textureWidth) {
  unsigned int *pixels = new unsigned int[size];
  for (unsigned int i = 0; i < size; i++) {
    if (gfx[i]) {
      pixels[i] = 0xFFFFFFFF;
    } else {
      pixels[i] = 0xFF000000;
    }
  }

  // update texture
  SDL_UpdateTexture(texture, NULL, pixels, textureWidth * sizeof(unsigned int));
  // clear screen and render
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  delete[] pixels;
}

void SDLDisplay::close() {
  SDL_DestroyTexture(texture);
  texture = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}
