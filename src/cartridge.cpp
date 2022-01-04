#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "cartridge.hpp"

Cartridge::Cartridge() {
    
	// Read rom file and create a vector with all of the data
	std::ifstream romStream(std::getenv("ROM"), std::ios::in | std::ios::binary);
	std::vector<uint8_t> romContents((std::istreambuf_iterator<char>(romStream)), std::istreambuf_iterator<char>());

	// Copy local vector to global vector
	fullCartridgeContents = romContents;
}

/**
 * @brief Loads and returns the contents of a rom file in a vector
 * 
 * @param switchableBankStart starting memory address of the switchable rom bank requested
 * @param switchableBankEnd ending memory address of the switchable rom bank requested
 * @return std::vector<uint8_t> The rom data in a vector
 */
std::vector<uint8_t> Cartridge::getCartridgeContents(int switchableBankStart, int switchableBankEnd) {
    
	// Init cartridge contents vector with the rom from bank zero (16k)
	std::vector<uint8_t> readableCartridgeContents = {fullCartridgeContents.begin(), fullCartridgeContents.begin() + 0x4000}; 

	// Add content from switchable rom bank (16k)
	readableCartridgeContents.insert(readableCartridgeContents.end(), fullCartridgeContents.begin() + switchableBankStart, fullCartridgeContents.begin() + switchableBankEnd);

	return readableCartridgeContents;
}
