#include <iostream>

#include "io.hpp"
#include "memory.hpp"

IO::IO(Memory memory):memory(memory) {}

/**
 * @brief Checks for input and updates the appropriate memory register
 */
void IO::step() {
    // TODO: Update register 0xFF00 with joypad data
}
