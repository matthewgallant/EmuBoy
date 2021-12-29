#include <iostream>

#include "cpu.hpp"
#include "utilities.hpp"

int main(int argc, char* args[]) {

	// Get file from environment
	if (!std::getenv("ROM")) {
		return -1;
	}

	debugRom();
	
	// Create virtual cpu
    CPU cpu;

	// Safely quit program
	return 0;
}
