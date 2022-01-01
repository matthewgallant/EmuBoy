
#include <iostream>
#include "RegisterFile.hpp"

int main(int argc, char* args[]) {
	RegisterFile *rf = new RegisterFile();
	std::cout << "REG_HL: " << std::hex << rf->readReg(REG_HL, true) << std::endl;
	rf->writeReg(REG_BC, (uint16_t) 0x3232, true);
	std::cout << "REG_C: " << std::hex << rf->readReg(REG_C, false) << std::endl;
	rf->writeReg(REG_C, 0x12);
	std::cout << "REG_C: " << std::hex << rf->readReg(REG_C, false) << std::endl;
	return 0;
}
