#pragma once
#include <vector>
#include <cstdint>

class Cartridge {
    public:
        Cartridge(char *romFile);
        std::vector<uint8_t> getCartridgeContents(uint16_t switchableBankStart, uint16_t switchableBankEnd);
        std::string getHeaderTitle();
    private:
        std::vector<uint8_t> fullCartridgeContents;
};
