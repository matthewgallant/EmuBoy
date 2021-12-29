#include <iostream>
#include <fstream>

#include "utilities.hpp"

void debugRom() {

    // Read rom file from ROM env var
    unsigned char romContents;
	std::ifstream romFile(std::getenv("ROM"), std::ios::binary);
	romFile >> std::noskipws;

    // Go through file and output hex pairs
	int lineItems = 0;
	while (romFile >> romContents) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)romContents << " ";

        // Limit hex pairs per line to 16 per line like hexdump
		lineItems++;
		if (lineItems == 16) {
			lineItems = 0;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
