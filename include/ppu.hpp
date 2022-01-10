#pragma once

#include <SDL2/SDL.h>

class PPU {
    public:
        PPU();
        void drawFrame();
        void drawPixel(int pixelPosX, int pixelPosY);
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};
