#pragma once

class Cartridge {
    public:
        Cartridge();
        std::vector<uint8_t> getCartridgeContents(uint16_t switchableBankStart, uint16_t switchableBankEnd);
    private:
        std::vector<uint8_t> fullCartridgeContents;
};
