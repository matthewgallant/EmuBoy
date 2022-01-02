#pragma once
#include <iomanip>

void debugRom(std::vector<uint8_t> romData);
void unknownInstruction(int instr);

// The following are utility macros 
// to allow us to properly parse opcode
// instructions. 
// 
// Each instruction is an 8-bit value 
// the layouts can be found here:
// http://www.z80.info/decoding.htm#

#define INS_GET_Z(ins) (ins & 0x7)
#define INS_GET_Y(ins) ((ins >> 3) & 0x7)
#define INS_GET_X(ins) ((ins >> 6) & 0x7)
#define INS_GET_P(ins) ((ins >> 3) & 0x1)
#define INS_GET_Q(ins) ((ins >> 3) & 0x3)