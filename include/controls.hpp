#pragma once

#include <vector>
#include <SDL2/SDL.h>

#define D_PAD_UP 0
#define D_PAD_RIGHT 1
#define D_PAD_DOWN 2
#define D_PAD_LEFT 3
#define START_BUTTON 4
#define SELECT_BUTTON 5
#define A_BUTTON 6
#define B_BUTTON 7

class Controls {
    public:
        std::vector<int> pollEvents();
    private:

        // Store event
        SDL_Event event;
};
