#include <iostream>

#include "ppu.hpp"
#include "memory.hpp"
#include "utilities.hpp"

PPU::PPU(Memory memory):memory(memory) {
    // TODO: Initialize LCD
}

void PPU::step() {

    // Check if PPU is powered off
    if (poweredOn() == false) return;

    // Increment PPU cycle ticks
    ticks++;

    // Execute functions based on current PPU mode
    if (mode == HORIZONTAL_BLANK_MODE) {
        
        // Emulate horizontal blank time
        if (ticks >= 102) {
            ticks = 0;
            changeScanline();

            // Check if it's time for a vblank or not
            if (scanline == 143) {
                changeMode(VERTICAL_BLANK_MODE);
                // TODO: Request interrupts
                // TODO: Draw buffer to screen
            } else {
                changeMode(SPRITE_SCAN_MODE);
            }
        }
    } else if (mode == VERTICAL_BLANK_MODE) {

        if (ticks >= 228) {
            ticks = 0;
            
            // Don't change the scanline on the first one
            if (scanline != 0) {
                changeScanline();
            }

            changeMode(SPRITE_SCAN_MODE);
        }
    } else if (mode == SPRITE_SCAN_MODE) {
        
        // Emulate OAM (sprite) scan phase
        if (ticks >= 40) {
            ticks = 0;
            changeMode(RENDERING_MODE);
        }
    } else if (mode == RENDERING_MODE) {

        // Emulate scanline building time
        if (ticks >= 86) {
            ticks = 0;
            changeMode(HORIZONTAL_BLANK_MODE);
            buildScanline();
        }
    }
}

void PPU::changeMode(int newMode) {
    mode = newMode;
    // TODO: Update LCD status register
}

void PPU::changeScanline() {
    scanline++;
    // TODO: Update line register

    // Check to wrap scanline around to zero
    if (scanline > 153) {
        scanline = 0;
        // TODO: Update line register
    }
}

void PPU::buildScanline() {
    
}

int PPU::getColor() {
    
}

bool PPU::poweredOn() {

    // TODO: Read on/off value from LCDC register
    return true;
}
