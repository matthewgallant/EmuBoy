#include <iostream>
#include <fstream>
#include <vector>

#include "utilities.hpp"

#define DEBUG_LINE_ITEMS 16

/**
 * Outputs a vector of bytes in a format similar to hexdump
 * 
 * @param bytes vector of bytes
 */
void debugBytes(std::vector<uint8_t> bytes) {
	int lineItems = 0;
	for(auto i: bytes) {
		int value = i;
		std::cout << std::hex << std::setw(2) << std::setfill('0') << value << " ";

		// Limit hex pairs per line to 16 per line like hexdump
		lineItems++;
		if (lineItems == DEBUG_LINE_ITEMS) {
			lineItems = 0;
			std::cout << std::endl;
		}
	}
	if (lineItems % DEBUG_LINE_ITEMS != 0) std::cout << std::endl;
}

/**
 * @brief Output a byte in a format that is command line friendly
 * 
 * @param byte byte to debug/output
 */
void debugByte(uint8_t byte) {
	std::cout << "Debug Byte: " << std::hex << std::setw(2) << std::setfill('0') << (int)byte << std::endl;
}

/**
 * Outputs a red colorized instruction error
 * 
 * @param instr instruction opcode
 */
int unknownInstruction(int instr) {
	std::cout << "\033[31m" << "Unknown Instruction: " << std::setw(2) << std::hex << instr << "\033[0m" << std::endl;
	return -1;
}
