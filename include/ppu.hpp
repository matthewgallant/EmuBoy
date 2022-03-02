#pragma once

#include <algorithm>
#include <vector>

#include "memory.hpp"
#include "lcd.hpp"

#define HORIZONTAL_BLANK_MODE   0
#define VERTICAL_BLANK_MODE     1
#define SPRITE_SCAN_MODE        2
#define RENDERING_MODE          3

#define LCD_LINE_REGISTER       0xff44
#define LCD_STATUS_REGISTER     0xff41
#define LCD_CONTROL_REGISTER    0xff40
#define COLOR_PALETTE_REGISTER  0xff47

#define VRAM_OFFSET             0x8000
#define CHARACTER_DATA_BEGIN    VRAM_OFFSET 
#define BG_DISPLAY_DATA_1_BEGIN 0x9800
#define BG_DISPLAY_DATA_2_BEGIN 0x9C00

class PPU {
    public:
        PPU(Memory *memory);
        void step();
    private:

        // Change ppu mode
        void changeMode(int newMode);

        // Change the current scanline
        void changeScanline();

        // Calculates and pushes pixels to buffer for a scanline
        void buildScanline();

        // Draws tiles to LCD
        void debugTiles(std::vector<uint8_t> vram);

        // Get pixel color data
        std::vector<int> *getTileLineColors(uint8_t firstByte, uint8_t secondByte);

        // Get LCD power status (on/off)
        bool poweredOn();

        // Make access to memory class global
        Memory memory;

        // Keep track of cycle ticks
        int ticks;

        // Create vector for screen buffer
        std::vector<int> buffer = std::vector<int>(WINDOW_WIDTH * WINDOW_HEIGHT);

        // Keep track of the ppu mode
        int mode;

        // Keep track of current scanline being handled
        int scanline;
};
