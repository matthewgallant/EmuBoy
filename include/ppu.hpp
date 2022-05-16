#pragma once

#include <algorithm>
#include <vector>

#include "memory.hpp"
#include "lcd.hpp"

#define HORIZONTAL_BLANK_MODE   0
#define VERTICAL_BLANK_MODE     1
#define SPRITE_SCAN_MODE        2
#define RENDERING_MODE          3

#define LCD_SCROLL_X_REGISTER   0xff43
#define LCD_SCROLL_Y_REGISTER   0xff42
#define LCD_CONTROL_REGISTER    0xff40

#define VRAM_OFFSET             0x8000

class PPU {
    public:
        PPU(Memory *memory, LCD *lcd);
        void step();
    private:

        // Change ppu mode
        void changeMode(int newMode);

        // Change the current scanline
        void changeScanline();

        // Calculates and pushes pixels to buffer for a scanline
        void buildScanline();

        // Get pixel color data
        void getTileLineColors(uint8_t firstByte, uint8_t secondByte);

        // Get LCD power status (on/off)
        bool poweredOn();

        // Make access to memory class global
        Memory *memory;

        // Make access to lcd class global
        LCD *lcd;

        // Keep track of cycle ticks
        int ticks;

        // Create vector for screen buffer
        int buffer[160][144];

        // Temporarily store color values for an eight pixel row
        int colors[8];

        // Keep track of the ppu mode
        int mode = HORIZONTAL_BLANK_MODE;

        // Keep track of current scanline being handled
        int scanline = 0;
};
