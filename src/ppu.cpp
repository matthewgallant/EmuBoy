#include <iostream>

#include "ppu.hpp"
#include "memory.hpp"
#include "utilities.hpp"

PPU::PPU(Memory memory):memory(memory) {

}

void PPU::step() {

    // Check if PPU is powered off
    if (poweredOn() == false) return;

    // Execute functions based on current PPU mode
    if (mode == HORIZONTAL_BLANK_MODE) {

    } else if (mode == VERTICAL_BLANK_MODE) {

    } else if (mode == SPRITE_SCAN_MODE) {

    } else if (mode == RENDERING_MODE) {

    }
}

void PPU::changeMode() {

}

void PPU::changeScanline() {

}

void PPU::buildScanline() {

}

int PPU::getColor() {

}

bool PPU::poweredOn() {

    // Always return true for testing purposes
    return true;
}
