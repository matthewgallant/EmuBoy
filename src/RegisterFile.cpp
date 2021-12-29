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
