#include <iostream>

#include "cpu.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = *(new RegisterFile());
}


/**
 * Perform a single stop in the CPU, following the 
 * von-neumman style fetch-decode-execute
 */
void CPU::step() { 
    rf.setPC(rf.getPC() + 1);
    // fetch
    next_instruction = memory->memory[rf.getPC()];
    // decode & execute
    execute(next_instruction);
}

/**
 * Set memory for cpu
 * 
 * @param memory Pointer to memory object
 */
void CPU::setMemory(Memory *memory) {
    this->memory = memory;
}

/**
 * Executes a single instruction on the CPU.
 * 
 * @param ins 8-bit encode instruction
 */
void CPU::execute(uint8_t ins){
    // begin decoding 
    uint8_t x = INS_GET_X(ins);
    uint8_t y = INS_GET_Y(ins);
    uint8_t z = INS_GET_Z(ins);
    uint8_t p = INS_GET_P(ins);
    uint8_t q = INS_GET_Q(ins);
}
