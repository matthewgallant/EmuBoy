#pragma once
#include "RegisterFile.hpp"
#include "memory.hpp"

class CPU {
    public:
        CPU();
        void step();
        void setMemory(Memory *memory);
    private:
        RegisterFile rf;
        Memory *memory;
        uint8_t next_instruction;
};
