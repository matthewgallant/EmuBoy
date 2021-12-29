#include <iostream>
#include <vector>
#include <string>

#include "cpu.hpp"
#include "cartridge.hpp"
#include "utilities.hpp"

int main(int argc, char* args[]) {

	// Get file from environment
	if (!std::getenv("ROM")) {
		return -1;
	}

	// Initialize cartridge
	Cartridge cartridge;
	std::vector<std::string> cartridgeContents = cartridge.getCartridgeContents();
	
	// Create virtual cpu
    CPU cpu;

	// Safely quit program
	return 0;
}
