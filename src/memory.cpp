#include <iostream>
#include <vector>

#include "memory.hpp"

Memory::Memory() {
    
    // Create stack of memory and fill with zeroes
    for (int i = 0; i < 0xFFFF; i++) {
        memory.push_back(0x00);
    }
}

/**
 * Loads vector containing data at the specified memory address
 * 
 * @param data vector containing the data
 * @param address int representation of the memory address
 */
void Memory::load(std::vector<uint8_t> data, int address) {
    std::copy(data.begin(), data.end(), memory.begin() + address);
}
