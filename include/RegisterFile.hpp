
// EmuBoy
// File: RegisterFile.hpp
// Authors: Jacob Mealey & Matt Gallant
// See LICENSE.md for license info
// Description:
//      the purpose of this file is to provide an
//      abstraction representation of the Gameboys
//      register file. 
#pragma once
#include <cstdint>
#include <iostream>

// regsiter lookup values are taken from:
// http://www.z80.info/decoding.htm

// Single register lookup values
#define REG_B   0
#define REG_C   1
#define REG_D   2
#define REG_E   3
#define REG_H   4
#define REG_L   5
#define REG_A   7

// double register lookup values
#define REG_BC  0
#define REG_DE  1
#define REG_HL  2
#define REG_SP  3
#define REG_AF  3

// Initial register values
#define INIT_AF 0x01B0
#define INIT_BC 0x0013
#define INIT_DE 0x00D8
#define INIT_HL 0x014D
#define INIT_SP 0xFFFE


// RegisterFile
// 
// This class organises all the functions and 
// data for each register. The registers are 
// grouped into there 16-bit pairs by default 
// using the SplitRegister struct. there is 
// most likely a *cleaner* way to do this but
// whatever. 
class RegisterFile {
    public:
        RegisterFile();
        // there are two instances of writeReg, one for 16bit 
        // versoin nd one for 8-bit values. 
        uint8_t writeReg(uint8_t sr, uint8_t val);
        uint16_t writeReg(uint8_t sr, uint16_t val, bool is_SP);
        uint16_t readReg(uint8_t sr, bool is_16_bit);
        uint16_t setPC(uint16_t pc);
        uint16_t getPC();
    private:
        // each register has a name like A or B and are 
        // organised in to 16-bit pairs as seen below.
        // for instances if one wanted to read from 
        // register A they would call AF.r1 because its
        // the first register in the AF pairing.
        struct SplitRegister{
            uint8_t r1;
            uint8_t r2;
        };

        // Beginning of general purpose regs.
        struct SplitRegister AF;
        struct SplitRegister BC;
        struct SplitRegister DE;
        struct SplitRegister HL;
        // SP -> stack pointer
        uint16_t SP;
        // PC -> program counter
        uint16_t PC;
};