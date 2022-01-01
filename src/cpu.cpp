#include <iostream>

#include "cpu.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = *(new RegisterFile());
}


void CPU::step() { 
    rf.setPC(rf.getPC() + 1);
    next_instruction = memory->memory[rf.getPC()];
    execute(next_instruction);
}

void CPU::setMemory(Memory *memory) {
    this->memory = memory;
}

void CPU::execute(uint8_t ins){
    uint8_t x = INS_GET_X(ins);
    uint8_t y = INS_GET_Y(ins);
    uint8_t z = INS_GET_Z(ins);
    uint8_t p = INS_GET_P(ins);
    uint8_t q = INS_GET_Q(ins);
}