#pragma once

#include <SDL2/SDL.h>

#include <cstdint>
#include <string>

class SDLDisplay {
private:
  int width = 0;
  int height = 0;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture = NULL;

  void initSDLWindow(std::string, int, int);
  void initSDLRenderer();
  void initSDLTexture(int, int);
  void throwError(std::string);

public:
  void init(std::string, int, int, int, int);
  void drawPixels(const bool *, uint16_t, int);
  void close();
};