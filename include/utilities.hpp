#pragma once
#include <iomanip>

void debugBytes(std::vector<uint8_t> bytes);
void debugByte(uint8_t byte);
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


// Terminal colors oh my god 
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define WHTB "\e[47m"
