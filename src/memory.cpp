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
 * @brief Writes a vector of data to a specified starting memory address
 * 
 * @param data vector of data to write
 * @param address specified memory address
 */
void Memory::write(std::vector<uint8_t> data, uint16_t address) {
    std::copy(data.begin(), data.end(), memory.begin() + address);
}

/**
 * @brief Writes a byte of data to a specified memory address
 * 
 * @param data byte of data to write
 * @param address specified memory address
 */
void Memory::write(uint8_t data, uint16_t address) {
    memory[address] = data;
}

/**
 * @brief Returns a vector of data from a specified memory address
 * 
 * @param startAddress specified memory start address
 * @param endAddress specified memory end address
 * @return std::vector<uint8_t> data read from memory
 */
std::vector<uint8_t> Memory::read(uint16_t startAddress, uint16_t endAddress) {
    std::vector<uint8_t> readData = {memory.begin() + startAddress, memory.begin() + endAddress}; 
    return readData;
}

/**
 * @brief Returns a byte of data from a specified memory address
 * 
 * @param address specified memory address
 * @return uint8_t byte of data from memory
 */
uint8_t Memory::read(uint16_t address) {
    return memory[address];
}
