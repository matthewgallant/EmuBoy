#include <iostream>

#include "lcd.hpp"
#include "memory.hpp"
#include "utilities.hpp"

#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 144
#define WINDOW_SCALE 3

LCD::LCD() {

    // Could not initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "Could not initialize SDL2: " << SDL_GetError() << std::endl;

    // Initialize window
    window = SDL_CreateWindow("EmuBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) std::cout << "Could not create SDL2 window: " << SDL_GetError() << std::endl;

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) std::cout << "Could not create SDL2 renderer: " << SDL_GetError() << std::endl;

    // Draw frame
    drawFrame();
    
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

/**
 * Prepares and renders a frame to the window
 */
void LCD::drawFrame() {

    // Clear render buffer
    SDL_RenderClear(renderer);

    // Loop through each pixel
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++) {

            // Draw pixel at position
            drawPixel(j, i);
        }
    }

    // Load buffered frame
    SDL_RenderPresent(renderer);
}

/**
 * Renders a rect representing a pixel to the window
 * 
 * @param pixelPosX horizonal position of the pixel
 * @param pixelPosY vertical position of the pixel
 */
void LCD::drawPixel(int pixelPosX, int pixelPosY) {

    // Set pixel color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Create the 'pixel' rect
    SDL_Rect pixelRect;
    pixelRect.x = pixelPosX * WINDOW_SCALE - WINDOW_SCALE;
    pixelRect.y = pixelPosY * WINDOW_SCALE - WINDOW_SCALE;
    pixelRect.w = WINDOW_SCALE;
    pixelRect.h = WINDOW_SCALE;

    // Render the 'pixel'
    SDL_RenderFillRect(renderer, &pixelRect);
}
