#include <iostream>
#include <vector>

#include "memory.hpp"

Memory::Memory() {
    std::cout << "Hello from the Memory" << std::endl;
}

/**
 * Loads vector containing rom data at the beginning of our vmem
 * 
 * @param romData A vector containing the rom data
 */
void Memory::loadRomData(std::vector<uint8_t> romData) {
    memory.insert(memory.begin(), romData.begin(), romData.end());
}
