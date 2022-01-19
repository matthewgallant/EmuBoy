#pragma once

#include "memory.hpp"

#define HORIZONTAL_BLANK_MODE 0
#define VERTICAL_BLANK_MODE 1
#define SPRITE_SCAN_MODE 2
#define RENDERING_MODE 3

#define LCD_LINE_REGISTER 0xff44
#define LCD_STATUS_REGISTER 0xff41
#define LCD_CONTROL_REGISTER 0xff40
#define COLOR_PALETTE_REGISTER 0xff47

class PPU {
    public:
        PPU(Memory memory);
        void step();
    private:

        // Change ppu mode
        void changeMode(int newMode);

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

        // Keep track of cycle ticks
        int ticks;

        // Create array for screen buffer
        int buffer[160][144];

        // Keep track of the ppu mode
        int mode;

        // Keep track of current scanline being handled
        int scanline;
};
