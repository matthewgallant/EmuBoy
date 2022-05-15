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

#define LCD_SCROLL_X_REGISTER   0xff43
#define LCD_SCROLL_Y_REGISTER   0xff42

#define VRAM_SIZE               6144
#define VRAM_OFFSET             0x8000
#define CHARACTER_DATA_BEGIN    VRAM_OFFSET 
#define BG_DISPLAY_DATA_1_BEGIN 0x9800
#define BG_DISPLAY_DATA_2_BEGIN 0x9C00
#define OAM_START               0xFE00
#define OAM_END                 0xFE9F
#define OAM_SIZE                OAM_END - OAM_START

#define OAM_2_VRAM(oam) 0x8000 + (oam * 0x10)

class PPU {
    public:
        PPU(Memory *memory, LCD *lcd);
        void step();
        // std::vector<int> *debugTiles();
    private:

        // OAM member array
        struct OamMember{
            uint8_t LCD_y_coord;
            uint8_t LCD_x_coord;
            uint8_t CHR_code;
            uint8_t flags;
        };

        std::vector<OamMember> *oam;

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
        int mode;

        // Keep track of current scanline being handled
        int scanline;
};
