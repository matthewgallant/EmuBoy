#pragma once

#include "memory.hpp"

class IO {
    public:
        IO(Memory memory);
        void step();
    private:

        // Make memory access global
        Memory memory;
};
