#pragma once
#include "RegisterFile.hpp"
#include "memory.hpp"
#include "utilities.hpp"

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

#define HALTED false

class CPU {
    public:
        CPU();
        void step();
        void setMemory(Memory *memory);
        void execute(uint8_t ins);
        void debug();
        void setFlag(uint8_t flag);
        void clearFlag(uint8_t flag);
        bool getFlag(uint8_t flag);
        void cbPrefixExecute(uint8_t ins);
        uint8_t getInstruction();
    private:
        RegisterFile rf;
        Memory *memory;
        uint8_t next_instruction;
        bool state;
};
