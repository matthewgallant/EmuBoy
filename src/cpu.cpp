#include <iostream>

#include "cpu.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = *(new RegisterFile());
}


void CPU::step() { 
    rf.setPC(rf.getPC() + 1);
    next_instruction = memory->memory[rf.getPC()];
}

void CPU::setMemory(Memory *memory) {
    this->memory = memory;
}