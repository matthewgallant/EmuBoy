#include <iostream>
#include <vector>
#include <string>

#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "utilities.hpp"

int main(int argc, char* args[]) {

	// Get file from environment
	if (!std::getenv("ROM")) {
		std::cout << "No ROM found in path. Please set 'ROM' in your path to your ROM file." << std::endl;
		return -1;
	}

	// Initialize cartridge
	Cartridge cartridge;
	std::vector<uint8_t> cartridgeContents = cartridge.getCartridgeContents();

	// Print rom hex for debugging
	debugRom(cartridgeContents);

	// Create virtual memory
	Memory memory;
	memory.loadRomData(cartridgeContents);
	
	// Create virtual cpu
    CPU cpu;

	// Safely quit program
	return 0;
}
