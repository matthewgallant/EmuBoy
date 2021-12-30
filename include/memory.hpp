#pragma once

class Memory {
    public:
        Memory();
        void loadRomData(std::vector<uint8_t> romData);
    private:
        std::vector<uint8_t> memory;
};
