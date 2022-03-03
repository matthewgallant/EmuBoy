#include <iostream>

#include "cpu.hpp"
#include "utilities.hpp"

CPU::CPU() {
    std::cout << "Hello from the CPU" << std::endl;
    this->rf = RegisterFile();
    std::cout << rf.readReg(REG_AF, IS_16_BIT) << std::endl;
}


/**
 * Perform a single stop in the CPU, following the 
 * von-neumman style fetch-decode-execute
 */
void CPU::step() { 
    // If we're halted don't step 
    if(state == HALTED) return;
    // THIS IS ONLY FOR DEBUG REMOVE LATER!!!
    debug();
    // fetch
    next_instruction = memory->memory[rf.getPC()];
    // decode & execute
    execute(next_instruction);
    // increment program counter
    rf.setPC(rf.getPC() + 1);
}

/**
 * Prints out various information of the CPU (registers, PC, SP, and Flag values)
 */
void CPU::debug() {
    printf("INS: %X \t PC: %X\n", memory->memory[rf.getPC()], rf.getPC());
    printf("-------REGISTERS-------\n");
    printf("A: %X \n", rf.readReg(REG_A, IS_8_BIT));
    printf("B: %X \t\t C: %X\n", rf.readReg(REG_B, IS_8_BIT), rf.readReg(REG_C, IS_8_BIT));
    printf("D: %X \t\t E: %X\n", rf.readReg(REG_D, IS_8_BIT), rf.readReg(REG_E, IS_8_BIT));
    printf("H: %X \t\t L: %X\n", rf.readReg(REG_H, IS_8_BIT), rf.readReg(REG_L, IS_8_BIT));
    printf("SP: %X\n", rf.getSP());
    printf("---------FLAGS---------\n");
    printf("%sZ      %sN       %sH      %sC%s\n\n", 
            getFlag(FLAG_Z) ? KRED:KNRM, 
            getFlag(FLAG_N) ? KRED:KNRM, 
            getFlag(FLAG_H) ? KRED:KNRM, 
            getFlag(FLAG_C) ? KRED:KNRM, 
            KNRM);
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
 * @param flag Flag value to set 
 */
void CPU::setFlag(uint8_t flag) { 
    uint8_t AF = rf.readReg(REG_AF, IS_16_BIT);
    AF |= 1 << flag;
    rf.writeReg(REG_AF, AF, false);
}

/**
 * 
 * @param flag Flag value to clear
 */
void CPU::clearFlag(uint8_t flag) {
    uint16_t AF = rf.readReg(REG_AF, IS_16_BIT);
    AF &= ~(1 << flag);
    rf.writeReg(REG_AF, AF, false);
}

/**
 * 
 * @param flag Flag value to get the value of
 * @return The value of the flag which is being probed
 */
bool CPU::getFlag(uint8_t flag) {
    return ((rf.readReg(REG_AF, IS_16_BIT) & (1 << flag)) == (1 << flag));
}

/**
 * Returns the current instruction
 */
uint8_t CPU::getInstruction() {
    return memory->memory[rf.getPC()];
}


/**
 * Executes a single instruction on the CPU.
 * 
 * @param ins 8-bit encode instruction
 */
void CPU::execute(uint8_t ins){
    // begin decoding 
    // there are decoded values not really needed but I'll leave it here :)
    uint8_t x = INS_GET_X(ins);
    uint8_t y = INS_GET_Y(ins);
    uint8_t z = INS_GET_Z(ins);
    uint8_t p = INS_GET_P(ins);
    uint8_t q = INS_GET_Q(ins);

    bool isDefined = false;

    switch(ins) {
        case 0x00: { // NOP
            isDefined = true;   
            break;
        } case 0x01: { // LD BC, u16 
            uint16_t new_bc_lower = memory->memory[rf.getPC() + 1];
            uint16_t new_bc_upper = memory->memory[rf.getPC() + 2];
            rf.writeReg(REG_BC, (new_bc_upper << 8) | (new_bc_lower & 0xFF), false);
            rf.setPC(rf.getPC() + 3);
            isDefined = true;
            break;
        } case 0x02: { // LD (BC), A
            memory->memory[rf.readReg(REG_BC, IS_16_BIT)] = rf.readReg(REG_A, IS_8_BIT);
            isDefined = true;
            break;
        } case 0x03: { // INC BC
            uint16_t old_bc = rf.readReg(REG_BC, IS_16_BIT);
            clearFlag(FLAG_N);
            rf.writeReg(REG_BC, --old_bc);
            if(old_bc == 0) {
                setFlag(FLAG_Z);
            }
            isDefined = true;
            break;
        } case 0x04: { // INC B
            bool z_tmp = rf.readReg(REG_B, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_B, rf.readReg(REG_B, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x05: { // DEC B 
            rf.writeReg(REG_B, rf.readReg(REG_B, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x06: { // LD B, u8
            break;
        } case 0x07: { // RLCA 
            break;
        } case 0x08: { // LD (u16), SP
            memory->memory[memory->memory[rf.getPC() + 1]] = rf.getSP();
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0x09: { // ADD HL, BC
            break;
        } case 0x0A: { // LD A, (BC)
            rf.writeReg(REG_A, memory->memory[rf.readReg(REG_BC, IS_16_BIT)]);
            isDefined = true;
            break;
        } case 0x0B: { // INC BC
            rf.writeReg(REG_BC, rf.readReg(REG_BC, IS_16_BIT) + 1, false);
            isDefined = true;
            break;
        } case 0x0C: { // INC C
            bool z_tmp = rf.readReg(REG_C, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_C, rf.readReg(REG_C, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x0D: { // DEC C
            rf.writeReg(REG_C, rf.readReg(REG_C, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x0E: { // LD C, u8
            rf.writeReg(REG_C, rf.getSP() + 1);
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
            break;
        } case 0x0F: { // RRCA
            break;
        } case 0x10: { // STOP
            // KILL THA CLOCK??
            isDefined = true;
            break;
        } case 0x11: { // LD DE, u16
            break;
        } case 0x12: { // LD (DE), A
            memory->memory[rf.readReg(REG_DE, IS_16_BIT)] = rf.readReg(REG_A, IS_8_BIT);
            isDefined = true;
            break;
        } case 0x13: { // INC DE
            rf.writeReg(REG_DE, rf.readReg(REG_DE, IS_16_BIT) + 1, false);
            if(rf.readReg(REG_DE, IS_16_BIT) == 0) {
                setFlag(FLAG_Z);
                setFlag(FLAG_C);
            } else {
                clearFlag(FLAG_Z);
            }
            setFlag(FLAG_H);
            isDefined = true;
            break;
        } case 0x14: { // INC D
            bool z_tmp = rf.readReg(REG_D, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_D, rf.readReg(REG_D, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x15: { // DEC D
            rf.writeReg(REG_D, rf.readReg(REG_D, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x16: { // LD D, u8
            uint8_t val = memory->memory[rf.getPC() + 1];
            rf.writeReg(REG_D, val);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0x17: { // RLA
            break;
        } case 0x18: { // JR i8
            int8_t jump = memory->memory[rf.getPC() + 1];
            rf.setPC(rf.getPC() + jump);
            isDefined = true;
            break;
        } case 0x19: { // ADD HL, DE
            uint16_t new_value = rf.readReg(REG_HL, IS_16_BIT) + rf.readReg(REG_DE, IS_16_BIT);
            rf.writeReg(REG_HL, new_value, false);
            isDefined = true;
            break;
        } case 0x1A: { // LD A, (DE)
            uint8_t value = memory->memory[rf.readReg(REG_DE, IS_16_BIT)];
            rf.writeReg(REG_A, value);
            isDefined = true;
            break;
        } case 0x1B: { // DEC DE
            rf.writeReg(REG_DE, rf.readReg(REG_DE, IS_16_BIT) - 1);    
            isDefined = true;
            break;
        } case 0x1C: { // INC E
            rf.writeReg(REG_E, rf.readReg(REG_E, IS_8_BIT) + 1);
            if(!rf.readReg(REG_E, IS_8_BIT)) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            isDefined = true;
            break;
        } case 0x1D: { // DEC E
            rf.writeReg(REG_E, rf.readReg(REG_E, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x1E: { // LD E, u8
            break;
        } case 0x1F: { // RRA
            break;
        } case 0x20: { // JR NZ, i8
            if(!getFlag(FLAG_Z)) {
                int8_t offset = memory->memory[rf.getPC() + 1];
                printf("offset %d\n", offset - 1);
                rf.setPC(rf.getPC() + offset - 1);
            }
            isDefined = true;
            break;
        } case 0x21: { // LD HL, u16
            break;
        } case 0x22: { // LD (HL+), A
            // THIS doesn't seem right do we need a + 1?
            memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = rf.readReg(REG_A, IS_8_BIT);
            rf.writeReg(REG_HL, rf.readReg(REG_HL, IS_16_BIT) + 1, false);
            isDefined = true;
            break;
        } case 0x23: { // INC HL
            rf.writeReg(REG_HL, rf.readReg(REG_HL, IS_16_BIT) + 1, false);
            isDefined = true;
            break;
        } case 0x24: { // INC H
            bool z_tmp = rf.readReg(REG_H, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_H, rf.readReg(REG_H, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x25: { // DEC H
            rf.writeReg(REG_H, rf.readReg(REG_H, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x26: { // LD H, u8
            break;
        } case 0x27: { // DAA
            break;
        } case 0x28: { // JR Z, i8
            if(getFlag(FLAG_Z)){
                int8_t offset = memory->memory[rf.getPC() + 1];
                printf("offset %d\n", offset - 2);
                rf.setPC(rf.getPC() + offset - 2);
            }
            isDefined = true;
            break;
        } case 0x29: { // ADD HL, HL
            break;
        } case 0x2A: { // LD A, (HL+)
            break;
        } case 0x2B: { // DEC HL
            break;
        } case 0x2C: { // INC L
            bool z_tmp = rf.readReg(REG_L, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_L, rf.readReg(REG_L, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x2D: { // DEC L
            rf.writeReg(REG_L, rf.readReg(REG_L, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x2E: { // LD L, u8
            break;
        } case 0x2F: { // CPL
            rf.writeReg(REG_A, ~rf.readReg(REG_A, IS_8_BIT));
            setFlag(FLAG_H);
            setFlag(FLAG_N);
            isDefined = true;
            break;
        } case 0x30: { // JR NC, i8
            if(!getFlag(FLAG_C)) {
                rf.setPC(rf.getPC() + (int8_t) memory->memory[rf.getPC() + 1]);
            }
            isDefined = true;;
            break;
        } case 0x31: { // LD SP, u16
            uint16_t new_sp_lower = memory->memory[rf.getPC()];
            uint16_t new_sp_upper = memory->memory[rf.getPC() + 1];
            rf.setSP((new_sp_upper << 8) | new_sp_lower);
            rf.setPC(rf.getPC() + 2);
            isDefined = true;
            break;
        } case 0x32: { // LD (HL-), A
            // similar to the HL+, should this be subtracting one?
            memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = rf.readReg(REG_A, IS_8_BIT);
            isDefined = true;
            break;
        } case 0x33: { // INC SP
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
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
            rf.writeReg(REG_HL, rf.readReg(REG_HL, IS_16_BIT) + rf.getSP(), false);
            isDefined = true;
            break;
        } case 0x3A: { // LD A, (HL-)
            break;
        } case 0x3B: { // DEC SP
            rf.setSP(rf.getSP() - 1);
            isDefined = true;
            break;
        } case 0x3C: { // INC A
            bool z_tmp = rf.readReg(REG_A, IS_8_BIT) == 0xFF;
            clearFlag(FLAG_N);
            if(z_tmp) {setFlag(FLAG_Z); setFlag(FLAG_H);}
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) + 1);
            isDefined = true;
            break;
        } case 0x3D: { // DEC A
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - 1);
            isDefined = true;
            break;
        } case 0x3E: { // LD A, u8
            uint8_t val = memory->memory[rf.getPC() + 1];
            rf.writeReg(REG_A, val);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0x3F: { // CCF
            clearFlag(FLAG_N);
            clearFlag(FLAG_H);
            if(getFlag(FLAG_C)){
                clearFlag(FLAG_C);
            } else {
                setFlag(FLAG_C);
            }
            isDefined = true;
            break;
        } case 0x40: { // LD B, B
        } case 0x41: { // LD B, C
        } case 0x42: { // LD B, D
        } case 0x43: { // LD B, E
        } case 0x44: { // LD B, H
        } case 0x45: { // LD B, L
        } case 0x46: { // LD B, (HL)
        } case 0x47: { // LD B, A
        } case 0x48: { // LD C, B
        } case 0x49: { // LD C, C
        } case 0x4A: { // LD C, D
        } case 0x4B: { // LD C, E
        } case 0x4C: { // LD C, H
        } case 0x4D: { // LD C, L
        } case 0x4E: { // LD C, (HL)
        } case 0x4F: { // LD C, A
        } case 0x50: { // LD D, B
        } case 0x51: { // LD D, C 
        } case 0x52: { // LD D, D
        } case 0x53: { // LD D, E
        } case 0x54: { // LD D, H
        } case 0x55: { // LD D, L
        } case 0x56: { // LD D, (HL)
        } case 0x57: { // LD D, A
        } case 0x58: { // LD E, B
        } case 0x59: { // LD E, C
        } case 0x5A: { // LD E, D
        } case 0x5B: { // LD E, E
        } case 0x5C: { // LD E, H
        } case 0x5D: { // LD E, L
        } case 0x5E: { // LD E, (HL)
        } case 0x5F: { // LD E, A
        } case 0x60: { // LD H, B
        } case 0x61: { // LD H, C 
        } case 0x62: { // LD H, D
        } case 0x63: { // LD H, E
        } case 0x64: { // LD H, H
        } case 0x66: { // LD H, (HL)
        } case 0x67: { // LD H, A
        } case 0x68: { // LD L, B
        } case 0x69: { // LD L, C 
        } case 0x6A: { // LD L, D
        } case 0x6B: { // LD L, E
        } case 0x6C: { // LD L, H
        } case 0x6D: { // LD L, L
        } case 0x6F: { // LD L, A
        } case 0x70: { // LD (HL), B
        } case 0x71: { // LD (HL), C 
        } case 0x72: { // LD (HL), D
        } case 0x73: { // LD (HL), E
        } case 0x74: { // LD (HL), H
        } case 0x75: { // LD (HL), L
            if(z == 0x6) {
                rf.writeReg(rf.readReg(y, IS_8_BIT), 
                    memory->memory[rf.readReg(REG_HL, IS_16_BIT)]);
            } else if(y == 0x6) {
                memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = 
                    rf.readReg(z, IS_8_BIT);
            } else {
                rf.writeReg(rf.readReg(y, IS_8_BIT), rf.readReg(z, IS_8_BIT));
            }
            isDefined = true;
            break;     
        } case 0x76: { // HALT
            // ???? IDK
            state = HALTED;
            break;
        } case 0x77: { // LD (HL), A
        } case 0x78: { // LD A, B
        } case 0x79: { // LD A, C
        } case 0x7A: { // LD A, D 
        } case 0x7B: { // LD A, E
        } case 0x7C: { // LD A, H
        } case 0x7D: { // LD A, L
        } case 0x7E: { // LD A, (HL)
        } case 0x7F: { // LD A, A
            if(z == 0x6) {
                rf.writeReg(rf.readReg(y, IS_8_BIT), 
                    memory->memory[rf.readReg(REG_HL, IS_16_BIT)]);
            } else if(y == 0x6) {
                memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = 
                    rf.readReg(z, IS_8_BIT);
            } else {
                rf.writeReg(rf.readReg(y, IS_8_BIT), rf.readReg(z, IS_8_BIT));
            } 
            isDefined = true;
            break;
        }case 0x80:// ADD A, B
        case 0x81:// ADD A, C
        case 0x82:// ADD A, D
        case 0x83:// ADD A, E
        case 0x84:// ADD A, H
        case 0x85:// ADD A, L
        case 0x86:
        case 0x87: { // ADD A, A
            if(z == 0x6){
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) + memory->memory[rf.readReg(REG_HL, IS_16_BIT)]);
            }else{
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) + rf.readReg(z, IS_8_BIT));
            }
            if(rf.readReg(REG_A, IS_8_BIT) == 0) 
                setFlag(FLAG_Z);
            clearFlag(FLAG_N);
            isDefined = true;
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
            uint8_t val = memory->memory[rf.readReg(REG_HL, IS_16_BIT)];
            rf.writeReg(REG_A, val + getFlag(FLAG_C));
            if(rf.readReg(REG_A, IS_8_BIT) == 0) 
                setFlag(FLAG_Z);
            isDefined = true;
            break;
        } case 0x8F: { // ADC A, A
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) + (rf.readReg(REG_A, IS_8_BIT) + getFlag(FLAG_C)));
            if(rf.readReg(REG_A, IS_8_BIT) == 0) 
                setFlag(FLAG_Z);
            clearFlag(FLAG_N);
            isDefined = true;
            break;
        case 0x90: // SUB A, B
        case 0x91: // SUB A, C
        case 0x92: // SUB A, D
        case 0x93: // SUB A, E
        case 0x94: // SUB A, H
        case 0x95: // SUB A, L
        case 0x96: // SUB A, (HL)
        case 0x97:{ // SUB A, A
            if(z == 0x6)
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - memory->memory[rf.readReg(REG_HL, IS_8_BIT)]);
            else
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - rf.readReg(z, IS_8_BIT));

            if(rf.readReg(REG_A, IS_8_BIT) == 0) 
                setFlag(FLAG_Z);
            setFlag(FLAG_N);
            isDefined = true;
            break;
        } case 0x98: // SBC A, B
        case 0x99: // SBC A, C
        case 0x9A: // SBC A, D
        case 0x9B: // SBC A, E
        case 0x9C: // SBC A, H
        case 0x9D: // SBC A, L
        case 0x9E: // SBC A, (HL)
        case 0x9F: {// SBC A, A
            if(z == 0x6)
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - memory->memory[rf.readReg(REG_HL, IS_8_BIT)] -getFlag(FLAG_C));
            else
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - rf.readReg(z, IS_8_BIT) - getFlag(FLAG_C));
            if(rf.readReg(REG_A, IS_8_BIT) == 0) 
                setFlag(FLAG_Z);
            setFlag(FLAG_N);
            isDefined = true;
            break;
        }case 0xA0: // AND A, B
        case 0xA1: // AND A, C
        case 0xA2: // AND A, D
        case 0xA3: // AND A, E
        case 0xA4: // AND A, H
        case 0xA5: // AND A, L
        case 0xA6: // AND A, (HL)
        case 0xA7: // AND A, A
            if(z == 0x6){
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) & memory->memory[rf.readReg(REG_HL, IS_16_BIT)]);
            }else{
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) & rf.readReg(z, IS_8_BIT));
            }
            clearFlag(FLAG_C);
            clearFlag(FLAG_H);
            setFlag(FLAG_N);
            if(rf.readReg(REG_A, IS_8_BIT) == 0)
                setFlag(FLAG_Z);
            isDefined = true;
            break;
        } case 0xA8: // XOR A, B
        case 0xA9: // XOR A, C
        case 0xAA: // XOR A, D 
        case 0xAB: // XOR A, E
        case 0xAC: // XOR A, H
        case 0xAD: // XOR A, L
        case 0xAE: // XOR A, (HL)
        case 0xAF:{ // XOR A, A
            if(z == 0x6){
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) ^ memory->memory[rf.readReg(REG_L, IS_8_BIT)]);
            }else{
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) ^ rf.readReg(z, IS_8_BIT));
            }
            clearFlag(FLAG_C);
            clearFlag(FLAG_H);
            clearFlag(FLAG_N);
            if(rf.readReg(REG_A, IS_8_BIT) == 0)
                setFlag(FLAG_Z);
            isDefined = true;
            break;
        } case 0xB0: // OR A, B
        case 0xB1: // OR A, C
        case 0xB2: // OR A, D
        case 0xB3: // OR A, E
        case 0xB4: // OR A, H
        case 0xB5: // OR A, L
        case 0xB6: // OR A, (HL)
        case 0xB7: { // OR A, A
            if(z == 0x6){
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) | memory->memory[rf.readReg(REG_HL, IS_16_BIT)]);
            }else{
                rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) | rf.readReg(z, IS_8_BIT));
            }
            clearFlag(FLAG_C);
            clearFlag(FLAG_H);
            clearFlag(FLAG_N);
            if(rf.readReg(REG_A, IS_8_BIT) == 0)
                setFlag(FLAG_Z);
            isDefined = true;
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
            setFlag(FLAG_Z);
            setFlag(FLAG_N);
            isDefined = true;  
            break;
        } case 0xC0: { // RET NZ
            break;
        } case 0xC1: { // POP BC
            rf.writeReg(REG_B, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            rf.writeReg(REG_C, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
            break;
        } case 0xC2: { // JP NZ, u16
            break;
        } case 0xC3: { // JP u16
            uint16_t lower = memory->memory[rf.getPC() + 1];
            uint16_t higher = memory->memory[rf.getPC() + 2];
            rf.setPC(((higher << 8) | lower) - 1);
            isDefined = true;
            break;
        } case 0xC4: { // CALL NZ, u16
            break;
        } case 0xC5: { // PUSH BC
            memory->memory[rf.getSP()] = rf.readReg(REG_C, IS_8_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            memory->memory[rf.getSP()] = rf.readReg(REG_B, IS_8_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            isDefined = true;
            break;
        } case 0xC6: { // ADD A, u8
            break;
        } case 0xC7: { // RST 00h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0000 - 1);
            isDefined = true;
            break;
        } case 0xC8: { // RET Z
            // If the Carry flag isn't set, execute return
            if(getFlag(FLAG_Z)) {
                this->execute(0xC9);
            }
            isDefined = true;
            break;
        } case 0xC9: { // RET
            uint16_t pc_low = (uint16_t) memory->memory[rf.getSP()];
            uint16_t pc_high = (uint16_t) memory->memory[rf.getSP() + 1];
            std::cout << pc_low << " " << pc_high << std::endl;
            rf.setSP(rf.getSP() + 2);
            rf.setPC((pc_high << 8) | pc_low);
            isDefined = true;
            break;
        } case 0xCA: { // JP Z, u16
            if(getFlag(FLAG_Z)) {
                uint16_t lower = memory->memory[rf.getSP()];
                uint16_t upper = memory->memory[rf.getSP() + 1];
                uint16_t addr = (upper << 8) | lower;
                rf.setSP(addr);
            }
            rf.setSP(rf.getSP() + 2);
            isDefined = true;
            break;
        } case 0xCB: { // PREFIX CB
            cbPrefixExecute(memory->memory[rf.getPC() + 1]);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0xCC: { // CALL Z, u16
            break;
        } case 0xCD: { // CALL u16
            // push pc to stack
            uint16_t old_pc_upper = rf.getPC() >> 7;
            uint16_t old_pc_lower = rf.getPC() & 0xFF; 
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            // load new pc 
            uint16_t new_pc_lower = memory->memory[rf.getPC() + 1];
            uint16_t new_pc_upper = memory->memory[rf.getPC() + 2];
            std::cout << new_pc_upper << new_pc_lower << std::endl;
            // set new pc
            // subtract 1 to account for exec incrementing one
            rf.setPC(((new_pc_upper << 8) | new_pc_lower) - 1);
            rf.setSP(rf.getSP() - 2);
            isDefined = true;
            break;
        } case 0xCE: { // ADC A, u8
            break;
        } case 0xCF: { // RST 08h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0008 - 1);
            isDefined = true;
            break;
        } case 0xD0: { // RET NC
            // If the Carry flag isn't set, execute RET (0xC9) 
            if(!getFlag(FLAG_C)) {
                this->execute(0xC9);
            }
            isDefined = true;
            break;
        } case 0xD1: { // POP DE
            rf.writeReg(REG_D, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            rf.writeReg(REG_E, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
            break;
        } case 0xD2: { // JP NC, u16
            break;
        } case 0xD3: { // N/A
            break;
        } case 0xD4: { // CALL NC, u16
            break;
        } case 0xD5: { // PUSH DE
            memory->memory[rf.getSP()] = rf.readReg(REG_E, IS_8_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            memory->memory[rf.getSP()] = rf.readReg(REG_D, IS_8_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            isDefined = true;
            break;
        } case 0xD6: { // SUB A, u8
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) - memory->memory[rf.getPC() + 1]);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0xD7: { // RST 10h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0010 - 1);
            isDefined = true;
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
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0018 - 1);
            isDefined = true;
            break;
        } case 0xE0: { // LD (FF00 + u8), A
            int16_t addr_offset = memory->memory[rf.getPC() + 1];
            memory->memory[0xFF00 + addr_offset] = rf.readReg(REG_A, IS_8_BIT);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0xE1: { // POP HL
            rf.writeReg(REG_H, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            rf.writeReg(REG_L, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
            break;
        } case 0xE2: { // LD (FF00 + C), A
            break;
        } case 0xE3: { // N/A
            isDefined = true; //??
            break;
        } case 0xE4: { // N/A
            isDefined = true; //??
            break;
        } case 0xE5: { // PUSH HL
            memory->memory[rf.getSP()] = rf.readReg(REG_L, IS_16_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            memory->memory[rf.getSP()] = rf.readReg(REG_H, IS_16_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            isDefined = true;
            break;
        } case 0xE6: { // AND A, u8
            uint8_t next_value = memory->memory[rf.getPC() + 1];
            rf.writeReg(REG_A, rf.readReg(REG_A, IS_8_BIT) & next_value);
            isDefined = true;
            break;
        } case 0xE7: { // RST 20h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0020 - 1);
            isDefined = true;
            break;
        } case 0xE8: { // ADD SP, i8
            break;
        } case 0xE9: { // JP HL
            break;
        } case 0xEA: { // LD (u16), A
            uint8_t lower = memory->memory[rf.getSP()];
            uint8_t upper = memory->memory[rf.getSP() + 1];
            uint16_t addr = (upper << 8) | lower;
            memory->memory[addr] = rf.readReg(REG_A, IS_8_BIT);
            isDefined = true;
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
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0028 - 1);
            isDefined = true;
            break;
        } case 0xF0: { // LD A, (FF00, u8)
            uint16_t addr_offset = memory->memory[rf.getPC() + 1];
            rf.writeReg(REG_A, memory->memory[0xFF00 + addr_offset]);
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0xF1: { // POP AF
            rf.writeReg(REG_A, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            rf.writeReg(REG_F, memory->memory[rf.getSP()]);
            rf.setSP(rf.getSP() + 1);
            isDefined = true;
            break;
        } case 0xF2: { // LD A, (FF00 + C)
            break;
        } case 0xF3: { // DI
            // Disiable INTERRUPTS ?????
            // We don't have interrupts soooo im gonna say it works :)
            isDefined = true;
            break;
        } case 0xF4: { //  N/A
            break;
        } case 0xF5: { // PUSH AF
            memory->memory[rf.getSP()] = rf.readReg(REG_F, IS_16_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            memory->memory[rf.getSP()] = rf.readReg(REG_A, IS_16_BIT);
            rf.setSP(rf.getSP() - 1); // DOES GAME STACK GROWN DOWN??
            isDefined = true;
            break;
        } case 0xF6: { // OR A, u8
            break;
        } case 0xF7: { // RST 30h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0030 - 1); // Minus one because step() adds one
            isDefined = true;  
            break;
        } case 0xF8: { // LD HL, SP+i8
            break;
        } case 0xF9: { // LD SP, HL
            break;
        } case 0xFA: { // LD A, (u16)
            uint16_t lower = memory->memory[rf.getPC() + 1];
            uint16_t upper = memory->memory[rf.getPC() + 2];
            rf.setPC(rf.getPC() + 1);
            uint16_t addr = (upper << 16) | lower;
            printf("%d\n", addr);
            rf.writeReg(REG_A, memory->memory[addr]);
            isDefined = true;
            break;
        } case 0xFB: { // EI
            // We don't have interrupts so nothing to enable!
            isDefined = true;
            break;
        } case 0xFC: { // N/A
            break;
        } case 0xFD: { // N/A
            break;
        } case 0xFE: { // CP A, u8
            uint8_t val = memory->memory[rf.getPC() + 1];
            uint8_t A = rf.readReg(REG_A, IS_8_BIT);
            // WHAT IS HALF CARRY OH MY GOD
            setFlag(FLAG_N);
            if((val - A) == 0) {
                setFlag(FLAG_Z);
            }else{
                clearFlag(FLAG_Z);
            }

            if(val > A) {
                setFlag(FLAG_H);
            } else {
                clearFlag(FLAG_H);
            }
            rf.setPC(rf.getPC() + 1);
            isDefined = true;
            break;
        } case 0xFF: { // RST 38h
            uint16_t old_pc_upper = rf.getPC() >> 8;
            uint16_t old_pc_lower = rf.getPC() & 0xFF;
            memory->memory[rf.getSP() - 1] = old_pc_upper;
            memory->memory[rf.getSP() - 2] = old_pc_lower;
            rf.setSP(rf.getSP() - 2);
            rf.setPC(0x0038 - 1); // Minus one because step() adds one
            isDefined = true;
            break;
        }
    }

    if (isDefined == false) {
        unknownInstruction(ins);
        this->state = HALTED;
    }
}

void CPU::cbPrefixExecute(uint8_t ins){
    uint8_t x = INS_GET_X(ins);
    uint8_t y = INS_GET_Y(ins);
    uint8_t z = INS_GET_Z(ins);

    debug();

    if(x == 0){         // ROT -- rotate / shift
        if(y == 0) { // RLC
            if(z == 6) {
                // memory stuff here
            } else {
                // hahaha this is so fucked up :)
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                uint8_t c = tmp & (1 << 7);
                if(c == 1 << 7) setFlag(FLAG_C);
                tmp <<= 1;
                tmp |= c ? 1:0;
                rf.writeReg(z, tmp);
            }
        } else if(y == 2) { // RL
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                if(tmp & (1 << 7)) setFlag(FLAG_C);
                tmp <<= 1;
                rf.writeReg(z, tmp);
            }
        } else if(y == 1) { // RRC
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                if(tmp & 1) setFlag(FLAG_C);
                tmp >>= 1;
                rf.writeReg(z | (getFlag(FLAG_C) << 7), tmp);
            }
        } else if(y == 3) { // RR
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                if(tmp & 1) setFlag(FLAG_C);
                tmp >>= 1;
                rf.writeReg(z, tmp);
            }
        } else if(y == 4) { // SLA
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                if(tmp & (1 << 7)) setFlag(FLAG_C);
                tmp <<= 1;
                rf.writeReg(z, tmp);
            }
        } else if(y == 5) { // SRA
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                bool bit_7 = tmp & (1 << 7);
                if(tmp & 1) setFlag(FLAG_C);
                tmp >>= 1;
                tmp |= bit_7 << 7;
                rf.writeReg(z, tmp);
            }
        } else if(y == 7) { // SRL
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                if(tmp & 1) setFlag(FLAG_C);
                tmp >>= 1;
                rf.writeReg(z, tmp);
            }
        } else if(y == 6) { // SWAP
            if(z == 6) {
                // memory stuff here
            } else {
                uint8_t tmp = rf.readReg(z, IS_8_BIT);
                uint8_t upper = tmp >> 4;
                uint8_t lower = tmp & 0xFF;
                tmp = 0;
                tmp |= (lower << 4) | upper;
                rf.writeReg(z, tmp);
                clearFlag(FLAG_C);
            }
        }
    } else if(x == 1){  // BIT -- test bit
        setFlag(FLAG_H);
        clearFlag(FLAG_N);
        if(z == 6) {
            if(memory->memory[rf.readReg(REG_HL, IS_16_BIT)] & (1 << y)){
                setFlag(FLAG_Z);
            }
        } else if(rf.readReg(z, IS_8_BIT) & (1 << y)) {
            setFlag(FLAG_Z);
        }else{
            clearFlag(FLAG_Z);
        }
    } else if(x == 2){  // RES -- reset bit
        if(z == 6){
            uint8_t tmp = memory->memory[rf.readReg(REG_HL, IS_16_BIT)];
            tmp &= ~(1 << y);
            memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = tmp;
        } else {
            uint8_t tmp = rf.readReg(z, IS_8_BIT);
            tmp &= ~(1 << y);
            rf.writeReg(z, tmp);
        }
    } else if(x == 3){  // SET -- set bit
        if(z == 6){
            uint8_t tmp = memory->memory[rf.readReg(REG_HL, IS_16_BIT)];
            tmp |= 1 << y;
            memory->memory[rf.readReg(REG_HL, IS_16_BIT)] = tmp;
        } else {
            uint8_t tmp = rf.readReg(z, IS_8_BIT);
            tmp |= 1 << y;
            rf.writeReg(z, tmp);
        }
    }
  
}