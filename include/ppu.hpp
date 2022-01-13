#pragma once

#include "memory.hpp"

class PPU {
    public:
        PPU(Memory memory);
        void step();
    private:

        // Change ppu mode
        void changeMode();

        // Change the current scanline
        void changeScanline();

        // Calculates and pushes pixels to buffer for a scanline
        void buildScanline();

        // Get pixel color data
        int getColor();

        // Get LCD power status (on/off)
        bool poweredOn();

        // Make access to memory class global
        Memory memory;

        // Keep track of clock cycles
        int clock;

        // Create array for screen buffer
        int pixels[160][144];

        // Keep track of the ppu mode
        int mode;

        // Keep track of current scanline being handled
        int scanline;
};
