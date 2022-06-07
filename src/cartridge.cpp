#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "cartridge.hpp"

Cartridge::Cartridge(char *romFile) {
    
	// Read rom file and create a vector with all of the data
	std::ifstream romStream(romFile, std::ios::in | std::ios::binary);
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
std::vector<uint8_t> Cartridge::getCartridgeContents(uint16_t switchableBankStart, uint16_t switchableBankEnd) {
    
	// Init cartridge contents vector with the rom from bank zero (16k)
	std::vector<uint8_t> readableCartridgeContents = {fullCartridgeContents.begin(), fullCartridgeContents.begin() + 0x4000}; 

	// Add content from switchable rom bank (16k)
	readableCartridgeContents.insert(readableCartridgeContents.end(), fullCartridgeContents.begin() + switchableBankStart, fullCartridgeContents.begin() + switchableBankEnd);

	return readableCartridgeContents;
}

/**
 * @brief Returns the title from the cartridge header
 * 
 * @return std::string cartridge title read from the header
 */
std::string Cartridge::getHeaderTitle() {
	std::vector<uint8_t> headerTitleBytes = {fullCartridgeContents.begin() + 0x134, fullCartridgeContents.begin() + 0x143};
	std::string headerTitle;

	// Read the 16 byte title from the cartridge
	for (int i = 0; i < 16; i++) {
		char c = (char) headerTitleBytes[i];
		if (c == 0) break;
		headerTitle.push_back(c);
	}

	return headerTitle;
}
