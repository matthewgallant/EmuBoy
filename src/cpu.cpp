#include <iostream>

#include "cpu.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = *(new RegisterFile());
}


/**
 * Perform a single stop in the CPU, following the 
 * von-neumman style fetch-decode-execute
 */
void CPU::step() { 
    rf.setPC(rf.getPC() + 1);
    // fetch
    next_instruction = memory->memory[rf.getPC()];
    // decode & execute
    execute(next_instruction);
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
            break;
        } case 0x05: { // DEC B 
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
            break;
        } case 0x0D: { // DEC C
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
            break;
        } case 0x15: { // DEC D
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
            break;
        } case 0x1D: { // DEC E
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
            break;
        } case 0x25: { // DEC H
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
            break;
        } case 0x2D: { // DEC L
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
            break;
        } case 0x3C: { // INC A
            break;
        } case 0x3D: { // DEC A
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
        } case 0x80: {
            break;
        } case 0x81: {
            break;
        } case 0x82: {
            break;
        } case 0x83: {
            break;
        } case 0x84: {
            break;
        } case 0x85: {
            break;
        } case 0x86: {
            break;
        } case 0x87: {
            break;
        } case 0x88: {
            break;
        } case 0x89: {
            break;
        } case 0x8A: {
            break;
        } case 0x8B: {
            break;
        } case 0x8C: {
            break;
        } case 0x8D: {
            break;
        } case 0x8E: {
            break;
        } case 0x8F: {
            break;
        } case 0x90: {
            break;
        } case 0x91: {
            break;
        } case 0x92: {
            break;
        } case 0x93: {
            break;
        } case 0x94: {
            break;
        } case 0x95: {
            break;
        } case 0x96: {
            break;
        } case 0x97: {
            break;
        } case 0x98: {
            break;
        } case 0x99: {
            break;
        } case 0x9A: {
            break;
        } case 0x9B: {
            break;
        } case 0x9C: {
            break;
        } case 0x9D: {
            break;
        } case 0x9E: {
            break;
        } case 0x9F: {
            break;
        } case 0xA0: {
            break;
        } case 0xA1: {
            break;
        } case 0xA2: {
            break;
        } case 0xA3: {
            break;
        } case 0xA4: {
            break;
        } case 0xA5: {
            break;
        } case 0xA6: {
            break;
        } case 0xA7: {
            break;
        } case 0xA8: {
            break;
        } case 0xA9: {
            break;
        } case 0xAA: {
            break;
        } case 0xAB: {
            break;
        } case 0xAC: {
            break;
        } case 0xAD: {
            break;
        } case 0xAE: {
            break;
        } case 0xAF: {
            break;
        } case 0xB0: {
            break;
        } case 0xB1: {
            break;
        } case 0xB2: {
            break;
        } case 0xB3: {
            break;
        } case 0xB4: {
            break;
        } case 0xB5: {
            break;
        } case 0xB6: {
            break;
        } case 0xB7: {
            break;
        } case 0xB8: {
            break;
        } case 0xB9: {
            break;
        } case 0xBA: {
            break;
        } case 0xBB: {
            break;
        } case 0xBC: {
            break;
        } case 0xBD: {
            break;
        } case 0xBE: {
            break;
        } case 0xBF: {
            break;
        } case 0xC0: {
            break;
        } case 0xC1: {
            break;
        } case 0xC2: {
            break;
        } case 0xC3: {
            break;
        } case 0xC4: {
            break;
        } case 0xC5: {
            break;
        } case 0xC6: {
            break;
        } case 0xC7: {
            break;
        } case 0xC8: {
            break;
        } case 0xC9: {
            break;
        } case 0xCA: {
            break;
        } case 0xCB: {
            break;
        } case 0xCC: {
            break;
        } case 0xCD: {
            break;
        } case 0xCE: {
            break;
        } case 0xCF: {
            break;
        } case 0xD0: {
            break;
        } case 0xD1: {
            break;
        } case 0xD2: {
            break;
        } case 0xD3: {
            break;
        } case 0xD4: {
            break;
        } case 0xD5: {
            break;
        } case 0xD6: {
            break;
        } case 0xD7: {
            break;
        } case 0xD8: {
            break;
        } case 0xD9: {
            break;
        } case 0xDA: {
            break;
        } case 0xDB: {
            break;
        } case 0xDC: {
            break;
        } case 0xDD: {
            break;
        } case 0xDE: {
            break;
        } case 0xDF: {
            break;
        } case 0xE0: {
            break;
        } case 0xE1: {
            break;
        } case 0xE2: {
            break;
        } case 0xE3: {
            break;
        } case 0xE4: {
            break;
        } case 0xE5: {
            break;
        } case 0xE6: {
            break;
        } case 0xE7: {
            break;
        } case 0xE8: {
            break;
        } case 0xE9: {
            break;
        } case 0xEA: {
            break;
        } case 0xEB: {
            break;
        } case 0xEC: {
            break;
        } case 0xED: {
            break;
        } case 0xEE: {
            break;
        } case 0xEF: {
            break;
        } case 0xF0: {
            break;
        } case 0xF1: {
            break;
        } case 0xF2: {
            break;
        } case 0xF3: {
            break;
        } case 0xF4: {
            break;
        } case 0xF5: {
            break;
        } case 0xF6: {
            break;
        } case 0xF7: {
            break;
        } case 0xF8: {
            break;
        } case 0xF9: {
            break;
        } case 0xFA: {
            break;
        } case 0xFB: {
            break;
        } case 0xFC: {
            break;
        } case 0xFD: {
            break;
        } case 0xFE: {
            break;
        } case 0xFF: {
            break;
        }
    }
}