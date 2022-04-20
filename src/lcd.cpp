#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>

#include "lcd.hpp"
#include "memory.hpp"
#include "utilities.hpp"

LCD::LCD() {

    // Could not initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "Could not initialize SDL2: " << SDL_GetError() << std::endl;

    // Initialize window
    window = SDL_CreateWindow("EmuBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) std::cout << "Could not create SDL2 window: " << SDL_GetError() << std::endl;

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) std::cout << "Could not create SDL2 renderer: " << SDL_GetError() << std::endl;
}

/**
 * Prepares and renders a frame to the window
 */
void LCD::drawFrame(std::vector<int> buffer) {
    // Clear render buffer
    SDL_RenderClear(renderer);

    // Loop through each pixel
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++) {

            unsigned int position = (i * WINDOW_WIDTH) - WINDOW_WIDTH + j;
            if(position >= buffer.size()) continue;

            // Draw pixel at position
            drawPixel(j, i, buffer[position]);

            // if (i % 2 == 0 && j % 2 != 0) {
            //     drawPixel(j, i, COLOR_WHITE);
            // } else {
            //     drawPixel(j, i, COLOR_BLACK);
            // }
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
 * @param pixelColor int representation of the pixel color
 */
void LCD::drawPixel(int pixelPosX, int pixelPosY, int pixelColor) {

    // Set pixel color
    setPixelColor(pixelColor);

    // Create the 'pixel' rect
    SDL_Rect pixelRect;
    pixelRect.x = pixelPosX * WINDOW_SCALE - WINDOW_SCALE;
    pixelRect.y = pixelPosY * WINDOW_SCALE - WINDOW_SCALE;
    pixelRect.w = WINDOW_SCALE;
    pixelRect.h = WINDOW_SCALE;

    // Render the 'pixel'
    SDL_RenderFillRect(renderer, &pixelRect);
}

/**
 * @brief Sets the current SDL render draw color
 * 
 * @param pixelColor int representation of the pixel color
 */
void LCD::setPixelColor(int pixelColor) {

    // Define color for pixel
    if (pixelColor == COLOR_BLACK) {
        r = g = b = 0;
    } else if (pixelColor == COLOR_DARK_GREY) {
        r = g = b = 119;
    } else if (pixelColor == COLOR_LIGHT_GREY) {
        r = g = b = 204;
    } else {
        r = g = b = 255;
    }

    // Set pixel color
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}


/**
 * @brief check for event, and handle if its a quit
 */
void LCD::handle_quit() {
    static SDL_Event event;
    static bool quit = false;
    if(quit == false) { 
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            SDL_DestroyWindow(this->window);
            SDL_Quit();
        }
    }
}