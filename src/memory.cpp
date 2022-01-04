#include <iostream>
#include <vector>

#include "memory.hpp"

Memory::Memory() {
    
    // Create stack of memory and fill with zeroes
    for (int i = 0; i < 65536; i++) {
        memory.push_back(0x00);
    }
}

/**
 * @brief Loads a vector of data to a specified starting memory address
 * 
 * @param data vector of data to load
 * @param address specified memory address
 */
void Memory::load(std::vector<uint8_t> data, uint16_t address) {
    std::copy(data.begin(), data.end(), memory.begin() + address);
}

/**
 * @brief Loads a byte of data to a specified memory address
 * 
 * @param data byte of data to load
 * @param address specified memory address
 */
void Memory::load(uint8_t data, uint16_t address) {
    memory[address] = data;
}
