#include <iostream>
#include <vector>

#include "memory.hpp"

Memory::Memory() {
    
    // Create stack of memory and fill with zeroes
    for (int i = 0; i < 65536; i++) {
        memory.push_back(0x00);
    }

    // Initialize I/O register memory addresses on start
    // Below excludes addresses that are set to 0x00 since that's already done up above.
    // See page 18 of http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf for more info.
    write(0x80, 0xFF10); // NR10
    write(0xBF, 0xFF11); // NR11
    write(0xF3, 0xFF12); // NR12
    write(0xBF, 0xFF14); // NR14
    write(0x3F, 0xFF16); // NR21
    write(0xBF, 0xFF19); // NR24
    write(0x7F, 0xFF1A); // NR30
    write(0xFF, 0xFF1B); // NR31
    write(0x9F, 0xFF1C); // NR32
    write(0xBF, 0xFF1E); // NR33
    write(0xFF, 0xFF20); // NR41
    write(0xBF, 0xFF23); // NR30
    write(0x77, 0xFF24); // NR50
    write(0xF3, 0xFF25); // NR51
    write(0xF1, 0xFF26); // NR52
    write(0x91, 0xFF40); // LCDC
    write(0xFC, 0xFF47); // BGP
    write(0xFF, 0xFF48); // OBP0
    write(0xFF, 0xFF49); // OBP1
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
