#include <iostream>
#include <SDL2/SDL.h>

#include "ppu.hpp"

#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 144
#define WINDOW_SCALE 3

PPU::PPU() {

    // Could not initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "Could not initialize SDL2: " << SDL_GetError() << std::endl;

    // Initialize window
    window = SDL_CreateWindow("EmuBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) std::cout << "Could not create SDL2 window: " << SDL_GetError() << std::endl;

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) std::cout << "Could not create SDL2 renderer: " << SDL_GetError() << std::endl;
    
    // Simple control loop
    SDL_Event event;
    bool quit = false;
    while(quit == false) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Exit the program
    SDL_DestroyWindow(window);
    SDL_Quit();
}
