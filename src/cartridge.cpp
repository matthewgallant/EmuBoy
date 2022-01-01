#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "cartridge.hpp"

Cartridge::Cartridge() {
    std::cout << "Hello from the Cartridge" << std::endl;
}

/**
 * Loads and returns the contents of a rom file in a vector
 * 
 * @return std::vector<uint8_t> The rom data in a vector
 */
std::vector<uint8_t> Cartridge::getCartridgeContents() {
    
	// Read rom file and create a vector with all of the data
	std::ifstream romStream(std::getenv("ROM"), std::ios::in | std::ios::binary);
	std::vector<uint8_t> romContents((std::istreambuf_iterator<char>(romStream)), std::istreambuf_iterator<char>());

	return romContents;
}
