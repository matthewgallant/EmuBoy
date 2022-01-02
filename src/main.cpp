#include <iostream>

#include <vector>
#include <string>

#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "utilities.hpp"

int main(int argc, char* args[]) {
	// Get file from environment
	// if (!std::getenv("ROM")) {
	// 	std::cout << "No ROM found in path. Please set 'ROM' in your path to your ROM file." << std::endl;
	// 	return -1;
	// }

	// Initialize cartridge
	// Cartridge cartridge;
	// std::vector<uint8_t> cartridgeContents = cartridge.getCartridgeContents();

	std::vector<uint8_t> cartridgeContents = {0x04, 0x05, 0x0C, 0x0D, 0x14, 
		0x15, 0x1C, 0x1D, 0x24, 0x25, 0x2C, 0x2D, 0x33, 0x3B, 0x3C, 0x3D};

	// Print rom hex for debugging
	debugRom(cartridgeContents);

	// Create virtual memory
	Memory memory;
	memory.loadRomData(cartridgeContents);
	
	// Create virtual cpu
    CPU cpu;
	cpu.setMemory(&memory);

	int steps = 0;
	while(true) {
		cpu.step();
	}

	// Safely quit program
	return 0;
}
