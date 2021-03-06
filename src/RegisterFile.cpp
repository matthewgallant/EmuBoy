
// EmuBoy
// File: RegisterFile.cpp
// Authors: Jacob Mealey & Matt Gallant
// See LICENSE.md for license info
// Description:
//      the purpose of this file is to implement functions 
//      as described in include/RegisterFile.hpp
#include "RegisterFile.hpp"
RegisterFile::RegisterFile(){
    writeReg(REG_AF, INIT_AF, false);
    writeReg(REG_BC, INIT_BC, false);
    writeReg(REG_DE, INIT_DE, false);
    writeReg(REG_HL, INIT_HL, false);
    writeReg(REG_SP, INIT_SP, true);
    this->PC = 0x100;
}

uint16_t RegisterFile::setPC(uint16_t pc) {
    this->PC = pc;
    return this->PC;
}
uint16_t RegisterFile::getPC() {
    return this->PC;
}

uint16_t RegisterFile::setSP(uint16_t sp) {
    this->SP = sp;
    return this->SP;
}
uint16_t RegisterFile::getSP() {
    return this->SP;
}
// SR is an octal value which is mapped to a specific register,
// depending on the type of val we use different lookup tables. 
// Both tables can be found at this link:
// http://www.z80.info/decoding.htm
uint8_t RegisterFile::writeReg(uint8_t sr, uint8_t val) {
    switch(sr) {
        case REG_B:
            this->BC.r1 = val;
            break;
        case REG_C:
            this->BC.r2 = val;
            break;
        case REG_D:
            this->DE.r1 = val;
            break;
        case REG_E:
            this->DE.r2 = val;
            break;
        case REG_H:
            this->HL.r1 = val;
            break;
        case REG_L:
            this->HL.r2 = val; 
            break;
        case REG_A:
            this->AF.r1 = val;
            break;
        case REG_F:
            this->AF.r2 = val;
        default:
            break;
    }
    return val;
}


uint16_t RegisterFile::writeReg(uint8_t sr, uint16_t val, bool is_SP) {
    switch(sr) {
        case REG_BC:
            this->BC.r1 = val >> 8;
            this->BC.r2 = val & 0xFF;
            break;
        case REG_DE:
            this->DE.r1 = val >> 8;
            this->DE.r1 = val & 0xFF;
            break;
        case REG_HL:
            this->HL.r1 = val >> 8;
            this->HL.r1 = val & 0xFF;
            break;
        case REG_AF: // OR REG_SP
            if(is_SP) this->SP = val;
            else {
                this->AF.r1 = val >> 8;
                this->AF.r2 = val & 0xFF;
            }
            break;
        default:
            break;
    }
    return val;
}

// readReg takes two variables, sr and is_16_bit. 
// depending on whether you say something is 16-it 
// the function will use two different lookup tables
uint16_t RegisterFile::readReg(uint8_t sr, bool is_16_bit) { 
    if(is_16_bit){
        switch(sr){
            case REG_BC:
                return ((uint16_t) this->BC.r1 << 8) | this->BC.r2;
            case REG_DE:
                return ((uint16_t) this->DE.r1 << 8) | this->DE.r2;
            case REG_HL:
                return ((uint16_t) this->HL.r1 << 8) | this->HL.r2;
            case REG_AF: // OR REG_SP
                return ((uint16_t) this->AF.r1 << 8) | this->AF.r2;
            default:
                return 0;   
        }
    } else {
        switch(sr) {
            case REG_B:
                return this->BC.r1;
            case REG_C:
                return this->BC.r2;
            case REG_D:
                return this->DE.r1;
            case REG_E:
                return this->DE.r2;
            case REG_H:
                return this->HL.r1;
            case REG_L:
                return this->HL.r2; 
            case REG_A:
                return this->AF.r1;
            case REG_F:
                return this->AF.r2;
            default:
                return 0;
        }
    }
    return 0;
}