#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "cartridge.hpp"

Cartridge::Cartridge() {
    std::cout << "Hello from the Cartridge" << std::endl;
}

std::vector<std::string> Cartridge::getCartridgeContents() {

	// Create vector for hex values
	std::vector<std::string> hexValues;
    
    // Read rom file from ROM env var
    unsigned char romContents;
	std::ifstream romFile(std::getenv("ROM"), std::ios::binary);
	romFile >> std::noskipws;

    // Go through file and add hex pairs to vector
	while (romFile >> romContents) {		
		std::stringstream hexValueRaw;
		hexValueRaw << std::hex << std::setw(2) << std::setfill('0') << (int)romContents;
		std::string hexValueString = hexValueRaw.str();
		hexValues.push_back(hexValueString);
	}

	return hexValues;
}
