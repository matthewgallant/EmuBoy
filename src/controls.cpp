#include <iostream>
#include <vector>
#include "SDL.h"

#include "controls.hpp"

/**
 * @brief Polls for events and returns a vector of inputs as ints
 * 
 * @return std::vector<int> inputs represented as integers
 */
std::vector<int> Controls::pollEvents() {

    // Temporarily store events in a vector
    std::vector<int> events;

    // Poll for events
    if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        events.push_back(D_PAD_UP);
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        events.push_back(D_PAD_LEFT);
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        events.push_back(D_PAD_DOWN);
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        events.push_back(D_PAD_RIGHT);
                        break;
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_RETURN2:
                        events.push_back(START_BUTTON);
                        break;
                    case SDL_SCANCODE_LSHIFT:
                    case SDL_SCANCODE_RSHIFT:
                        events.push_back(SELECT_BUTTON);
                        break;
                    case SDL_SCANCODE_X:
                        events.push_back(A_BUTTON);
                        break;
                    case SDL_SCANCODE_Z:
                        events.push_back(B_BUTTON);
                        break;
                }
                break;
		}
	}

    return events;
}
