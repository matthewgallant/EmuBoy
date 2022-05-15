#pragma once

#include <vector>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 144
#define WINDOW_SCALE 3

#define COLOR_WHITE 0
#define COLOR_LIGHT_GREY 1
#define COLOR_DARK_GREY 2
#define COLOR_BLACK 3

class LCD {
    public:
        LCD();
        void drawFrame(int pixels[160][144]);
        void drawPixel(int pixelPosX, int pixelPosY, int pixelColor);
        void setPixelColor(int pixelColor);
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        // Create RGB variables to store pixel color
        int r, g, b;
};
