#include <iostream>
#include <fstream>
#include <vector>

#include "utilities.hpp"

/**
 * Outputs a vector of rom data in a format similar to hexdump
 * 
 * @param romData A vector of rom data
 */
void debugRom(std::vector<uint8_t> romData) {
	int lineItems = 0;
	std::ios init(NULL);
	init.copyfmt(std::cout);
	for(auto i: romData) {
		int value = i;
		std::cout << std::hex << std::setw(2) << std::setfill('0') << value << " ";

		// Limit hex pairs per line to 16 per line like hexdump
		lineItems++;
		if (lineItems == 16) {
			lineItems = 0;
			std::cout << std::endl;
		}
	}
	std::cout.copyfmt(init);
}
