#include <iostream>

#include "cpu.hpp"
#include "utilities.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = *(new RegisterFile());
}


/**
 * Perform a single stop in the CPU, following the 
 * von-neumman style fetch-decode-execute
 */
void CPU::step() { 
    // fetch
    next_instruction = memory->memory[rf.getPC()];
    // THIS IS ONLY FOR DEBUG REMOVE LATER!!!
    if(next_instruction == 0) {
        return;
    }
    debug();
    // decode & execute
    execute(next_instruction);
    // increment program counter
    rf.setPC(rf.getPC() + 1);
}

void CPU::debug() {
    printf("INS: %X \t PC: %X\n", memory->memory[rf.getPC() - 1], rf.getPC());
    printf("-------REGISTERS-------\n");
    printf("A: %X \n", rf.readReg(REG_A, IS_8_BIT));
    printf("B: %X \t\t C: %X\n", rf.readReg(REG_B, IS_8_BIT), rf.readReg(REG_C, IS_8_BIT));
    printf("D: %X \t\t E: %X\n", rf.readReg(REG_D, IS_8_BIT), rf.readReg(REG_E, IS_8_BIT));
    printf("H: %X \t\t L: %X\n", rf.readReg(REG_H, IS_8_BIT), rf.readReg(REG_L, IS_8_BIT));
    printf("SP: %X\n\n", rf.getSP());
}

/**
 * Set memory for cpu
 * 
 * @param memory Pointer to memory object
 */
void CPU::setMemory(Memory *memory) {
    this->memory = memory;
}

/**
 * Executes a single instruction on the CPU.
 * 
 * @param ins 8-bit encode instruction
 */
void CPU::execute(uint8_t ins){
    // begin decoding 
    uint8_t x = INS_GET_X(ins);
    uint8_t y = INS_GET_Y(ins);
    uint8_t z = INS_GET_Z(ins);
    uint8_t p = INS_GET_P(ins);
    uint8_t q = INS_GET_Q(ins);

    bool isDefined = false;

    switch(ins) {
        case 0x00: { // NOP
            break;
        } case 0x01: { // LD BC, u16 
            break;
        } case 0x02: { // LD (BC), A
            break;
        } case 0x03: { // INC BC
            break;
        } case 0x04: { // INC B
            rf.writeReg(REG_B, rf.readReg(REG_B, IS_8_BIT) + 1);
            break;
        } case 0x05: { // DEC B 
            rf.writeReg(REG_B, rf.readReg(REG_B, IS_8_BIT) - 1);
            break;
        } case 0x06: { // LD B, u8
            break;
        } case 0x07: { // RLCA 
            break;
        } case 0x08: { // LD (u16), SP
            break;
        } case 0x09: { // ADD HL, BC
            break;
        } case 0x0A: { // LD A, (BC)
            break;
        } case 0x0B: { // INC BC
            break;
        } case 0x0C: { // INC C
            rf.writeReg(REG_C, rf.readReg(REG_C, IS_8_BIT) + 1);
            break;
        } case 0x0D: { // DEC C
            rf.writeReg(REG_C, rf.readReg(REG_C, IS_8_BIT) - 1);
            break;
        } case 0x0E: { // LD C, u8
            break;
        } case 0x0F: { // RRCA
            break;
        } case 0x10: { // STOP
            break;
        } case 0x11: { // LD DE, u16
            break;
        } case 0x12: { // LD (DE), A
            break;
        } case 0x13: { // INC DE
            break;
        } case 0x14: { // INC D
            rf.writeReg(REG_D, rf.readReg(REG_D, IS_8_BIT) + 1);
            break;
        } case 0x15: { // DEC D
            rf.writeReg(REG_D, rf.readReg(REG_D, IS_8_BIT) - 1);
            break;
        } case 0x16: { // LD D, u8
            break;
        } case 0x17: { // RLA
            break;
        } case 0x18: { // JR i8
            break;
        } case 0x19: { // ADD HL, DE
            break;
        } case 0x1A: { // LD A, (DE)
            break;
        } case 0x1B: { // DEC DE
            break;
        } case 0x1C: { // INC E
            rf.writeReg(REG_E, rf.readReg(REG_E, IS_8_BIT) + 1);
            break;
        } case 0x1D: { // DEC E
            rf.writeReg(REG_E, rf.readReg(REG_E, IS_8_BIT) - 1);
            break;
        } case 0x1E: { // LD E, u8
            break;
        } case 0x1F: { // RRA
            break;
        } case 0x20: { // JR NZ, i8
            break;
        } case 0x21: { // LD HL, u16
            break;
        } case 0x22: { // LD (HL+), A
            break;
        } case 0x23: { // INC HL
            break;
        } case 0x24: { // INC H
            rf.writeReg(REG_H, rf.readReg(REG_H, IS_8_BIT) + 1);
            break;
        } case 0x25: { // DEC H
            rf.writeReg(REG_H, rf.readReg(REG_H, IS_8_BIT) - 1);
            break;
        } case 0x26: { // LD H, u8
            break;
        } case 0x27: { // DAA
            break;
        } case 0x28: { // JR Z, i8
            break;
        } case 0x29: { // ADD HL, HL
            break;
        } case 0x2A: { // LD A, (HL+)
            break;
        } case 0x2B: { // DEC HL
            break;
        } case 0x2C: { // INC L
            rf.writeReg(REG_L, rf.readReg(REG_L, IS_8_BIT) + 1);
            break;
        } case 0x2D: { // DEC L
            rf.writeReg(REG_L, rf.readReg(REG_L, IS_8_BIT) - 1);
            break;
        } case 0x2E: { // LD L, u8
            break;
        } case 0x2F: { // CPL
            break;
        } case 0x30: { // JR NC, i8
            break;
        } case 0x31: { // LD SP, u16
            break;
        } case 0x32: { // LD (HL-), A
            break;
        } case 0x33: { // INC SP
            rf.setSP(rf.getSP() + 1);
            break;
        } case 0x34: { // INC (HL)
            break;
        } case 0x35: { // DEC (HL)
            break;
        } case 0x36: { // LD (HL), u8
            break;
        } case 0x37: { // SCF
            break;
        } case 0x38: { // JR C, i8
            break;
        } case 0x39: { // ADD HL, SP
            break;
        } case 0x3A: { // LD A, (HL-)
            break;
        } case 0x3B: { // DEC SP
            rf.setSP(rf.getSP() - 1);
            break;
        } case 0x3C: { // INC A
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) + 1);
            break;
        } case 0x3D: { // DEC A
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - 1);
            break;
        } case 0x3E: { // LD A, u8
            break;
        } case 0x3F: { // CCF
            break;
        } case 0x40: { // LD B, B
            break;
        } case 0x41: { // LD B, C
            break;
        } case 0x42: { // LD B, D
            break;
        } case 0x43: { // LD B, E
            break;
        } case 0x44: { // LD B, H
            break;
        } case 0x45: { // LD B, L
            break;
        } case 0x46: { // LD B, (HL)
            break;
        } case 0x47: { // LD B, A
            break;
        } case 0x48: { // LD C, B
            break;
        } case 0x49: { // LD C, C
            break;
        } case 0x4A: { // LD C, D
            break;
        } case 0x4B: { // LD C, E
            break;
        } case 0x4C: { // LD C, H
            break;
        } case 0x4D: { // LD C, L
            break;
        } case 0x4E: { // LD C, (HL)
            break;
        } case 0x4F: { // LD C, A
            break;
        } case 0x50: { // LD D, B
            break;
        } case 0x51: { // LD D, C 
            break;
        } case 0x52: { // LD D, D
            break;
        } case 0x53: { // LD D, E
            break;
        } case 0x54: { // LD D, H
            break;
        } case 0x55: { // LD D, L
            break;
        } case 0x56: { // LD D, (HL)
            break;
        } case 0x57: { // LD D, A
            break;
        } case 0x58: { // LD E, B
            break;
        } case 0x59: { // LD E, C
            break;
        } case 0x5A: { // LD E, D
            break;
        } case 0x5B: { // LD E, E
            break;
        } case 0x5C: { // LD E, H
            break;
        } case 0x5D: { // LD E, L
            break;
        } case 0x5E: { // LD E, (HL)
            break;
        } case 0x5F: { // LD E, A
            break;      
        } case 0x60: { // LD H, B
            break;      
        } case 0x61: { // LD H, C 
            break;     
        } case 0x62: { // LD H, D
            break;     
        } case 0x63: { // LD H, E
            break;     
        } case 0x64: { // LD H, H
            break;     
        } case 0x65: { // LD H, L
            break;     
        } case 0x66: { // LD H, (HL)
            break;
        } case 0x67: { // LD H, A
            break;      
        } case 0x68: { // LD L, B
            break;      
        } case 0x69: { // LD L, C 
            break;     
        } case 0x6A: { // LD L, D
            break;     
        } case 0x6B: { // LD L, E
            break;     
        } case 0x6C: { // LD L, H
            break;     
        } case 0x6D: { // LD L, L
            break;     
        } case 0x6E: { // LD L, (HL)
            break;
        } case 0x6F: { // LD L, A
            break;      
        } case 0x70: { // LD (HL), B
            break;      
        } case 0x71: { // LD (HL), C 
            break;     
        } case 0x72: { // LD (HL), D
            break;     
        } case 0x73: { // LD (HL), E
            break;     
        } case 0x74: { // LD (HL), H
            break;     
        } case 0x75: { // LD (HL), L
            break;     
        } case 0x76: { // HALT
            break;
        } case 0x77: { // LD (HL), A
            break;
        } case 0x78: { // LD A, B
            break;
        } case 0x79: { // LD A, C
            break;
        } case 0x7A: { // LD A, D 
            break;
        } case 0x7B: { // LD A, E
            break;
        } case 0x7C: { // LD A, H
            break;
        } case 0x7D: { // LD A, L
            break;
        } case 0x7E: { // LD A, (HL)
            break;
        } case 0x7F: { // LD A, A
            break;
        } case 0x80: { // ADD A, B
            break;
        } case 0x81: { // ADD A, C
            break;
        } case 0x82: { // ADD A, D
            break;
        } case 0x83: { // ADD A, E
            break;
        } case 0x84: { // ADD A, H
            break;
        } case 0x85: { // ADD A, L
            break;
        } case 0x86: { // ADD A, (HL)
            break;
        } case 0x87: { // ADD A, A
            break;
        } case 0x88: { // ADC A, B
            break;
        } case 0x89: { // ADC A, C
            break;
        } case 0x8A: { // ADC A, D
            break;
        } case 0x8B: { // ADC A, E
            break;
        } case 0x8C: { // ADC A, H
            break;
        } case 0x8D: { // ADC A, L
            break;
        } case 0x8E: { // ADC A, (HL)
            break;
        } case 0x8F: { // ADC A, A
            break;
        } case 0x90: { // SUB A, B
            break;
        } case 0x91: { // SUB A, C
            break;
        } case 0x92: { // SUB A, D
            break;
        } case 0x93: { // SUB A, E
            break;
        } case 0x94: { // SUB A, H
            break;
        } case 0x95: { // SUB A, L
            break;
        } case 0x96: { // SUB A, (HL)
            break;
        } case 0x97: { // SUB A, A
            break;
        } case 0x98: { // SBC A, B
            break;
        } case 0x99: { // SBC A, C
            break;
        } case 0x9A: { // SBC A, D
            break;
        } case 0x9B: { // SBC A, E
            break;
        } case 0x9C: { // SBC A, H
            break;
        } case 0x9D: { // SBC A, L
            break;
        } case 0x9E: { // SBC A, (HL)
            break;
        } case 0x9F: { // SBC A, A
            break;
        } case 0xA0: { // AND A, B
            break;
        } case 0xA1: { // AND A, C
            break;
        } case 0xA2: { // AND A, D
            break;
        } case 0xA3: { // AND A, E
            break;
        } case 0xA4: { // AND A, H
            break;
        } case 0xA5: { // AND A, L
            break;
        } case 0xA6: { // AND A, (HL)
            break;
        } case 0xA7: { // AND A, A
            break;
        } case 0xA8: { // XOR A, B
            break;
        } case 0xA9: { // XOR A, C
            break;
        } case 0xAA: { // XOR A, D 
            break; 
        } case 0xAB: { // XOR A, E
            break;
        } case 0xAC: { // XOR A, H
            break;
        } case 0xAD: { // XOR A, L
            break;
        } case 0xAE: { // XOR A, (HL)
            break;
        } case 0xAF: { // XOR A, A
            break;
        } case 0xB0: { // OR A, B
            break;
        } case 0xB1: { // OR A, C
            break;
        } case 0xB2: { // OR A, D
            break;
        } case 0xB3: { // OR A, E
            break;
        } case 0xB4: { // OR A, H
            break;
        } case 0xB5: { // OR A, L
            break;
        } case 0xB6: { // OR A, (HL)
            break;
        } case 0xB7: { // OR A, A
            break;
        } case 0xB8: { // CP A, B
            break;
        } case 0xB9: { // CP A, C
            break;
        } case 0xBA: { // CP A, D
            break;
        } case 0xBB: { // CP A, E
            break;
        } case 0xBC: { // CP A, H
            break;
        } case 0xBD: { // CP A, L
            break;
        } case 0xBE: { // CP A, (HL)
            break;
        } case 0xBF: { // CP A, A
            break;
        } case 0xC0: { // RET NZ
            break;
        } case 0xC1: { // POP BC
            break;
        } case 0xC2: { // JP NZ, u16
            break;
        } case 0xC3: { // JP u16
            break;
        } case 0xC4: { // CALL NZ, u16
            break;
        } case 0xC5: { // PUSH BC
            break;
        } case 0xC6: { // ADD A, u8
            break;
        } case 0xC7: { // RST 00h
            break;
        } case 0xC8: { // RET Z
            break;
        } case 0xC9: { // RET
            break;
        } case 0xCA: { // JP Z, u16
            break;
        } case 0xCB: { // PREFIX CB
            break;
        } case 0xCC: { // CALL Z, u16
            break;
        } case 0xCD: { // CALL u16
            break;
        } case 0xCE: { // ADC A, u8
            break;
        } case 0xCF: { // RST 08h
            break;
        } case 0xD0: { // RET NC
            break;
        } case 0xD1: { // POP DE
            break;
        } case 0xD2: { // JP NC, u16
            break;
        } case 0xD3: { // N/A
            break;
        } case 0xD4: { // CALL NC, u16
            break;
        } case 0xD5: { // PUSH DE
            break;
        } case 0xD6: { // SUB A, u8
            break;
        } case 0xD7: { // RST 10h
            break;
        } case 0xD8: { // RET C
            break;
        } case 0xD9: { // RETI
            break;
        } case 0xDA: { // JP C, u16
            break;
        } case 0xDB: { // N/A
            break;
        } case 0xDC: { // CALL C, u16
            break;
        } case 0xDD: { // N/A
            break;
        } case 0xDE: { // SBC A, u8
            break;
        } case 0xDF: { // RST 18h
            break;
        } case 0xE0: { // LD (FF00 + u8), A
            break;
        } case 0xE1: { // POP HL
            break;
        } case 0xE2: { // LD (FF00 + C), A
            break;
        } case 0xE3: { // N/A
            break;
        } case 0xE4: { // N/A
            break;
        } case 0xE5: { // PUSH HL
            break;
        } case 0xE6: { // AND A, u8
            break;
        } case 0xE7: { // RST 20h
            break;
        } case 0xE8: { // ADD SP, i8
            break;
        } case 0xE9: { // JP HL
            break;
        } case 0xEA: { // LD (u16), A
            break;
        } case 0xEB: { // N/A
            break;
        } case 0xEC: { // N/A
            break;
        } case 0xED: { // N/A
            break;
        } case 0xEE: { // XOR A, u8
            break;
        } case 0xEF: { // RST 28h
            break;
        } case 0xF0: { // LD A, (FF00, u8)
            break;
        } case 0xF1: { // POP AF
            break;
        } case 0xF2: { // LD A, (FF00 + C)
            break;
        } case 0xF3: { // DI
            break;
        } case 0xF4: { //  N/A
            break;
        } case 0xF5: { // PUSH AF
            break;
        } case 0xF6: { // OR A, u8
            break;
        } case 0xF7: { // RST 30h
            break;
        } case 0xF8: { // LD HL, SP+i8
            break;
        } case 0xF9: { // LD SP, HL
            break;
        } case 0xFA: { // LD A, (u16)
            break;
        } case 0xFB: { // EI
            break;
        } case 0xFC: { // N/A
            break;
        } case 0xFD: { // N/A
            break;
        } case 0xFE: { // CP A, u8
            break;
        } case 0xFF: { // RST 38h
            break;
        }
    }

    if (isDefined == false) {
        unknownInstruction(ins);
    }
}