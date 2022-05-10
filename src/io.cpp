#include <iostream>
#include <vector>

#include "io.hpp"
#include "controls.hpp"
#include "memory.hpp"

IO::IO(Memory *memory):memory(memory) {}

/**
 * @brief Checks for input and updates the appropriate memory register
 */
void IO::step() {

    // Poll for events from controls
    std::vector<int> events = controls.pollEvents();

    // TODO: Process events from controls
    // TODO: Update register 0xFF00 with joypad data
}
