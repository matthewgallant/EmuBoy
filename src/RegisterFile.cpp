#include "RegisterFile.hpp"

RegisterFile::RegisterFile(){
    this->AF = INIT_AF;
    this->BC = INIT_BC;
    this->DE = INIT_DE;
    this->HL = INIT_HL;
    this->SP = INIT_SP;
    this->PC = 0;
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
        default:
            break;
    }
    return val;
}

uint16_t RegisterFile::writeReg(uint8_t sr, uint16_t val, bool is_SP) {
    switch(sr) {
        case REG_BC:
            this->BC = val;
            break;
        case REG_DE:
            this->DE = val;
            break;
        case REG_HL:
            this->HL = val;
            break;
        case REG_AF: // OR REG_SP
            if(is_SP) this->SP = val;
            else this->AF = val;
            break;
        default:
            break;
    }
    return val;
}


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
            default:
                return 0;
        }
    }
    return 0;
}