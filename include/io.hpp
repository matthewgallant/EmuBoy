#pragma once

#include "memory.hpp"
#include "controls.hpp"

class IO {
    public:
        IO(Memory memory);
        void step();
    private:

        // Make memory access global
        Memory memory;

        // Instantiate controls
        Controls controls;
};
