#pragma once
#include <vector>

class Memory {
    public:
        Memory();
        void load(std::vector<uint8_t> data, uint16_t address);
    private:
        std::vector<uint8_t> memory;
    friend class CPU;
};
