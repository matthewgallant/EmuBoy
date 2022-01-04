#pragma once

class Cartridge {
    public:
        Cartridge();
        std::vector<uint8_t> getCartridgeContents(int switchableBankStart, int switchableBankEnd);
    private:
        std::vector<uint8_t> fullCartridgeContents;
};
