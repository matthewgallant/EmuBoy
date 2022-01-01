#pragma once
#include "RegisterFile.hpp"
#include "memory.hpp"
#include "utilities.hpp"

class CPU {
    public:
        CPU();
        void step();
        void setMemory(Memory *memory);
        void execute(uint8_t ins);
    private:
        RegisterFile rf;
        Memory *memory;
        uint8_t next_instruction;
};
