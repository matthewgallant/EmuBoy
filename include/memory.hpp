#include <vector>
#include <cstdint>
#pragma once

class Memory {
    public:
        Memory();
        
        void write(std::vector<uint8_t> data, uint16_t address);
        void write(uint8_t data, uint16_t address);

        std::vector<uint8_t> read(uint16_t startAddress, uint16_t endAddress);
        uint8_t read(uint16_t address);
        std::vector<uint8_t> memory;
    friend class CPU;
};
