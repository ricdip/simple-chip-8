#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>

#include <string>

class SDLDisplay {
   private:
    int width = 0;
    int height = 0;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    void initSDLWindow(std::string title, int windowWidth, int windowHeight);
    void initSDLRenderer();
    void initSDLTexture(int textureWidth, int textureHeight);
    void throwError(std::string error);

   public:
    void init(std::string title, int windowWidth, int windowHeight,
              int textureWidth, int textureHeight);
    void drawPixels(const bool* gfx, unsigned int size, int textureWidth);
    void close();
};

#endif
