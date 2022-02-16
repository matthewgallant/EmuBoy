#pragma once

#include <SDL2/SDL.h>

#define COLOR_BLACK 0
#define COLOR_DARK_GREY 1
#define COLOR_LIGHT_GREY 2
#define COLOR_WHITE 3

class LCD {
    public:
        LCD();
        void drawFrame();
        void drawPixel(int pixelPosX, int pixelPosY, int pixelColor);
        void setPixelColor(int pixelColor);
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        // Create RGB variables
        int r, g, b;
};
