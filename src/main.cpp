#include <iostream>

#include "cpu.hpp"
#include "cartridge.hpp"
#include "utilities.hpp"

int main(int argc, char* args[]) {

	// Get file from environment
	if (!std::getenv("ROM")) {
		return -1;
	}

	debugRom();

	// Initialize cartridge
	Cartridge cartridge;
	
	// Create virtual cpu
    CPU cpu;

	// Safely quit program
	return 0;
}
