#pragma once

#include <SDL2/SDL.h>

class PPU {
    public:
        PPU();
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};
