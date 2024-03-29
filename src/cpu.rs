use crate::memory::Memory;


const FLAG_Z: u8 = 3;
const FLAG_N: u8 = 2;
const FLAG_H: u8 = 1;
const FLAG_C: u8 = 0;

const RP_BC: u8 = 0;
const RP_DE: u8 = 1;
const RP_HL: u8 = 2;
const RP_SP: u8 = 3;

const RP2_BC: u8 = 0;
const RP2_DE: u8 = 1;
const RP2_HL: u8 = 2;
const RP2_AF: u8 = 3;

const tick_table: [i32; 256] = [   //Number of machine cycles for each instruction:
/*   0,  1,  2,  3,  4,  5,  6,  7,      8,  9,  A, B,  C,  D, E,  F*/
     4, 12,  8,  8,  4,  4,  8,  4,     20,  8,  8, 8,  4,  4, 8,  4,  //0
     4, 12,  8,  8,  4,  4,  8,  4,     12,  8,  8, 8,  4,  4, 8,  4,  //1
     8, 12,  8,  8,  4,  4,  8,  4,      8,  8,  8, 8,  4,  4, 8,  4,  //2
     8, 12,  8,  8, 12, 12, 12,  4,      8,  8,  8, 8,  4,  4, 8,  4,  //3

     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //4
     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //5
     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //6
     8,  8,  8,  8,  8,  8,  4,  8,      4,  4,  4, 4,  4,  4, 8,  4,  //7

     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //8
     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //9
     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //A
     4,  4,  4,  4,  4,  4,  8,  4,      4,  4,  4, 4,  4,  4, 8,  4,  //B

     8, 12, 12, 16, 12, 16,  8, 16,      8, 16, 12, 0, 12, 24, 8, 16,  //C
     8, 12, 12,  4, 12, 16,  8, 16,      8, 16, 12, 4, 12,  4, 8, 16,  //D
    12, 12,  8,  4,  4, 16,  8, 16,     16,  4, 16, 4,  4,  4, 8, 16,  //E
    12, 12,  8,  4,  4, 16,  8, 16,     12,  8, 16, 4,  0,  4, 8, 16   //F
];

const prefix_tick_table: [i32; 256] = [  //Number of machine cycles for each 0xCBXX instruction:
/*  0, 1, 2, 3, 4, 5,  6, 7,        8, 9, A, B, C, D,  E, F*/
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //0
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //1
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //2
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //3

    8, 8, 8, 8, 8, 8, 12, 8,        8, 8, 8, 8, 8, 8, 12, 8,  //4
    8, 8, 8, 8, 8, 8, 12, 8,        8, 8, 8, 8, 8, 8, 12, 8,  //5
    8, 8, 8, 8, 8, 8, 12, 8,        8, 8, 8, 8, 8, 8, 12, 8,  //6
    8, 8, 8, 8, 8, 8, 12, 8,        8, 8, 8, 8, 8, 8, 12, 8,  //7

    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //8
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //9
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //A
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //B

    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //C
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //D
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8,  //E
    8, 8, 8, 8, 8, 8, 16, 8,        8, 8, 8, 8, 8, 8, 16, 8   //F
];

#[derive(PartialEq)]
pub enum CpuMode {
    Run,
    Halt,
    Stop,
}

pub struct Cpu {
    pub pc: u16,
    pub sp: u16,
    pub a: u8,
    pub f: u8,
    pub b: u8,
    pub c: u8,
    pub d: u8,
    pub e: u8,
    pub h: u8,
    pub l: u8,
    pub mode: CpuMode,
    pub interrupts: bool

}

impl Cpu {
    pub fn new() -> Self {
        Cpu {
            pc: 0x0000,
            sp: 0xFFFE,
            a: 0x01,
            f: 0xB0,
            b: 0x00,
            c: 0x13,
            d: 0x00,
            e: 0xD8,
            h: 0x01,
            l: 0x4D,
            mode: CpuMode::Run,
            interrupts: true
        }
    }

    pub fn step<'b>(&mut self, memory: &'b mut Memory) {
        if self.mode == CpuMode::Run {
            //println!("Stepping CPU...");
            self.execute(memory);

        }
    }
    
    pub fn ticks<'b>(&self, memory: &'b mut Memory) -> i32 {
        let prev_ins: u8 = memory.byte(self.pc - 2);
        let ins: u8 = memory.byte(self.pc - 1);
        
        let index: i32;
        if prev_ins == 0xCB {
            index = prefix_tick_table[ins as usize];
        } else {
            index = tick_table[ins as usize];
        }
        
        index
    }

    pub fn execute<'b>(&mut self, memory: &'b mut Memory) {
        // println!("Op Code: 0x{:02X} and pc: {}", memory.memory[self.pc as usize], self.pc);
        // self.print(memory);
        let opcode = memory.memory[self.pc as usize];
        let z = opcode & 0x7;
        let y = (opcode >> 3) & 0x7;
        let x = (opcode >> 6) & 0x3;
        let p = y >> 1;
        let q = y & 0x1;

        // UN prefixed opcdes
        if x == 0 {
            match z {
                0 => {
                   if y == 0 { // NOP 
                       
                   } else if y == 1 { // LD(nn), SP
                        let addr = memory.read16(self.pc + 1);
                        memory.write16(self.sp, addr);
                        self.pc += 2;
                   } else if y == 2 {  // STOP
                       // not sure what else needs to happen but  ¯\_(ツ)_/¯
                       self.mode = CpuMode::Stop; 
                   } else if y == 3 { // JR d // THIS NEEDS TESTING
                       let d = memory.byte(self.pc + 1) as i8;
                       self.pc = ((self.pc as i16) + d as i16) as u16 - 1;
                   } else if (4..8).contains(&y) {  // JR cc[y-4, d
                       if self.get_cc(y-4) {
                           let d = memory.byte(self.pc + 1) as i8;
                           self.pc = ((self.pc as i16) + d as i16) as u16 - 1;
                       }
                       self.pc += 1;
                   }

                } 
                1 => {
                    if q == 0 { // LD rp[p], nn
                        let nn = memory.read16(self.pc + 1);
                        println!("16 bit load on {}: {}", p, nn);
                        self.set_rp(nn, p);
                        self.pc += 2; //offset pc by 2 bytes
                    } else if q == 1 { // ADD HL, rp[p]
                        let hl = self.get_rp(RP_HL) + self.get_rp(p);
                        self.set_rp(hl, RP_HL);
                    }
                } 
                2 => {
                    if q == 0 {
                        if p == 0{ // LD (BC), A
                            memory.set_byte(self.a, self.get_rp(RP_BC));
                        } else if p == 1 { // LD (DE), A
                            memory.set_byte(self.a, self.get_rp(RP_DE));
                        } else if p == 2 { // LD (HL+), A
                            memory.set_byte(self.a, self.get_rp(RP_HL));
                            self.set_rp(self.get_rp(RP_HL) + 1, RP_HL);
                        } else if p == 3 { // LD (HL-), A
                            memory.set_byte(self.a, self.get_rp(RP_HL));
                            self.set_rp(self.get_rp(RP_HL) - 1, RP_HL);
                        }
                    
                    }
                    if q == 1 {
                        if p == 0 { // LD A, (BC)
                            self.a = memory.byte(self.get_rp(RP_BC));
                        } else if p == 1 { // LD A, (DE) 
                            self.a = memory.byte(self.get_rp(RP_DE));
                        } else if p == 2 { // LD A, (HL+)
                            self.a = memory.byte(self.get_rp(RP_HL));
                            self.set_rp(self.get_rp(RP_HL) + 1, RP_HL);
                        } else if p == 3 { // LD A, (HL-)
                            self.a = memory.byte(self.get_rp(RP_HL));
                            self.set_rp(self.get_rp(RP_HL) - 1, RP_HL);
                        }
                    }
                }
                // OKAY all of these flags need to be checked because this is so 
                // beyong fuking curseed.
                3 => {
                    if q == 0 { // INC rp[p]
                        if self.get_rp(p) == std::u16::MAX {
                            self.set_rp(0, p);
                            self.set_flag(FLAG_C, true);
                        } else {
                            self.set_flag(FLAG_C, (((self.get_rp(p)) + 1) & 0x10) == 0x10);
                            self.set_flag(FLAG_N, false);
                            self.set_flag(FLAG_H, (((self.get_rp(p)) + 1) & 0x10) == 0x10);
                            self.set_rp(self.get_rp(p) + 1, p);
                            self.set_flag(FLAG_Z, self.get_rp(p) == 0);       
                        }

                    } else if q == 1 { // DEC rp[p] 
                        if self.get_rp(p) == 1 {
                            self.set_flag(FLAG_Z, true);
                            self.set_flag(FLAG_C, true);
                            self.set_rp(0, p);
                        } else if self.get_rp(p) != 0 {
                            self.set_rp(self.get_rp(p) - 1, p);
                            self.set_flag(FLAG_N, true);
                            self.set_flag(FLAG_H, (((self.get_rp(p)) - 1) & 0x10) == 0x10);
                        }
                    }
                }
                4 => { // INC r[y]
                    self.set_flag(FLAG_N, false);
                    self.set_flag(FLAG_H, (((self.get_r(y)) + 1) & 0x10) == 0x10);
                    self.set_flag(FLAG_C, (((self.get_r(y)) + 1) & 0x10) == 0x10);
                    self.set_r(y, self.get_r(y) + 1);
                    self.set_flag(FLAG_Z, self.get_rp(y) == 0);
                }
                5 => { // DEC r[y]
                    if self.get_r(y) == 1 {
                        self.set_flag(FLAG_Z, true);
                        self.set_r(0, y);
                    } else if self.get_r(y) != 0 {
                        self.set_r(self.get_r(y) - 1, y);
                        self.set_flag(FLAG_N, true);
                        self.set_flag(FLAG_H, (((self.get_r(y)) - 1) & 0x10) == 0x10);
                        self.set_flag(FLAG_C, (((self.get_r(y)) - 1) & 0x10) == 0x10);
                    }
                }
                6 => { // LD r[y], n 
                    self.set_r(memory.byte(self.pc + 1), y);
                    self.pc += 1;
                }
                7 => {
                    println!("PC: 0x{:x} Opcode: 0x{:4x} not yet implemented", self.pc, opcode);
                }
                _ => {}
            }

        } else if x == 1 {
            if (z == 6) && (y == 6) {
                // HALT
                self.mode = CpuMode::Halt;
                return;
            } 
            // LD 
            self.set_r(self.get_r(z), y);
        } else if x == 2 {
            // ALU operations with y and z 
            self.alu(y, z, memory.byte(self.get_rp(RP_HL)), None);
        } else if x == 3 {
            match z {
                0 => {
                    match y {
                        0 => { // RET NZ
                            if !self.get_flag(FLAG_Z) {
                                self.pc = self.pop(memory) - 1;
                            }
                        }
                        1 => { // RET Z
                            if self.get_flag(FLAG_Z) {
                                self.pc = self.pop(memory) - 1;
                            }
                        }
                        2 => { // RET NC
                            if !self.get_flag(FLAG_C) {
                                self.pc = self.pop(memory) - 1;
                            }
                        }
                        3 => { // RET C
                            if self.get_flag(FLAG_C) {
                                self.pc = self.pop(memory) - 1;
                            }
                        }
                        4 => { // LD (0xFF00 + n), A
                            memory.set_byte(self.a, 0xFF00 + memory.byte(self.pc +1) as u16);
                            self.pc += 1;
                        }
                        5 => { // ADD SP, d
                            self.set_rp(memory.byte(self.pc + 1) as u16, RP_SP);
                            self.pc += 1;
                        }
                        6 => { // LD A, (0xFF00 + n)
                            self.set_r(memory.byte(0xFF00 + self.c as u16), self.a);
                        }
                        7 => { // LD HL, SP+ d
                            self.set_rp(self.sp + memory.byte(self.pc + 1) as u16, RP_HL);
                            self.pc += 1;
                        }
                        _ => {}
                    }
                }
                1 => {
                    if q == 0 { // POP
                        let pop = self.pop(memory);
                        self.set_rp2(pop, p);
                    } else if q == 1 {
                        if p == 0 || p == 1{ // RET & RETI
                            self.pc = self.pop(memory);
                        } else if p == 2 { // JP HL
                            self.pc = self.get_rp(RP_HL) - 1;
                        } else if p == 3 { // LD SP, HL
                            self.sp = self.get_rp(RP_HL);
                        }
                    }
                }
                2 => {
                    if y < 4 { // JP cc[y], nn
                        if self.get_cc(y) {
                            self.pc = memory.read16(self.pc + 1) - 1;
                        }
                    } else if y == 4 { // LD (0xFF00 + C), A
                        memory.set_byte(self.a, 0xFF00 + self.c as u16);
                    } else if y == 5 { // LD (nn), A
                        println!("LOADING FROM A to {}", self.pc + 1);
                        memory.set_byte(self.a, memory.read16(self.pc + 1));
                        self.pc += 2;
                    } else if y == 6 { // LD A, (0xFF00 + C)
                        self.a = memory.byte(0xFF00 + self.c as u16);
                    } else if y == 7 { //LD _A, (nn)
                        self.a = memory.byte(memory.read16(self.pc+1));
                        self.pc += 2;
                    }
                }
                3 => {
                    if y == 0 { // JP nn
                        self.pc = memory.read16(self.pc + 1) - 1;
                    } else if y == 1 { // CB prefix 
                        self.cb_prefix(memory.byte(self.pc + 1), memory);
                        self.pc += 1;
                    } else if y == 6 { // DI
                        self.interrupts = false
                    } else if y == 7 { // EI
                        self.interrupts = true
                    }
                }
                4 => { // Call cc[y], nn
                    if self.get_cc(y) {
                        self.push(self.pc, memory);
                        self.pc = memory.read16(self.pc + 1) - 1;
                    }
                }
                5 => {
                    if q == 0 { // PUSH rp2[2]
                        self.push(self.get_rp2(p), memory);
                    } else if p == 0 { // CALL nn
                        self.push(self.pc + 2, memory);
                        self.pc = memory.read16(self.pc + 1) - 1;
                    }
                }
                6 => { // Alu with immediate offset
                    let n = memory.byte(self.pc + 1);
                    self.alu(y, z, memory.byte(self.get_rp(RP_HL)), Some(n));
                    self.pc += 1;
                }
                7 => {println!("PC: 0x{:x} Opcode: 0x{:2X} not yet implemented", self.pc, opcode);}
                _ => {println!("PC: 0x{:x} Opcode: 0x{:2X} not yet implemented", self.pc, opcode);}
            }
        }

        self.pc += 1;
    }

    pub fn alu(&mut self, y: u8, z: u8, mem_hl: u8, n: Option<u8>) {
        let mut val = match z {
            0 => {self.b}
            1 => {self.c}
            2 => {self.d}
            3 => {self.e}
            4 => {self.h}
            5 => {self.l}
            6 => {mem_hl} // memory[self.hl]
            7 => {self.a}
            _ => {0}
        };

        val = match n {
            Some(a) =>  {
                println!("{}", a);
                a
            },
            None => val
        };

        match y {
            0 => { // ADD A
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_H, (((self.a & 0xF) + (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) + 
                                        ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a += val;
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            1 => { // ADC A
                val += u8::from(self.get_flag(FLAG_C)); 
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_H, (((self.a & 0xF) + (val)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) + 
                                        ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a += val;
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            2 => { // SUB
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - 
                                        ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a -= val;
                self.set_flag(FLAG_Z, self.a == 0);
            }  
            3 => { // SBC
                val += u8::from(self.get_flag(FLAG_C));
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - 
                                        ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a -= val;
                self.set_flag(FLAG_Z, self.a == 0);
                self.set_flag(FLAG_N, true);
            }
            4 => { // AND
                self.a &= val;
                self.set_flag(FLAG_C, false);
                self.set_flag(FLAG_H, false);
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            5 => { // XOR
                self.a ^= val;
                self.set_flag(FLAG_C, false);
                self.set_flag(FLAG_H, false);
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_Z, self.a == 0);
            }  
            6 => { // OR
                self.a |= val;
                self.set_flag(FLAG_C, false);
                self.set_flag(FLAG_H, false);
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            7 => { // CP
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.set_flag(FLAG_Z, self.a == 0);
                self.set_flag(FLAG_N, true);
            } 
            _ => {eprintln!("Unknown ALU instruction...")}
            
        }
    }

    fn cb_prefix(&mut self, prefix_op: u8, memory: &mut Memory) {
        let x = prefix_op >> 6;
        let y = (prefix_op >> 3) & 0x7;
        let z = prefix_op & 0x7;
        //println!("running cb prefix: {:2x}", prefix_op);
        if x == 0 { // rot[y] r[z]
            if y == 2 { // rl z
                let mut r = self.get_r(z);
                let oc = u8::from(self.get_flag(FLAG_C));
                self.set_flag(FLAG_C, (r & (0x80)) != 0);
                self.set_flag(FLAG_H, false);
                self.set_flag(FLAG_N, false);
                r <<= 1;
                r += oc;
                self.set_flag(FLAG_Z, r == 0);
                self.set_r(r, z);
            } else if y == 4 { // sla y
                let mut r = self.get_r(z);
                self.set_flag(FLAG_C, (r & (0x80)) != 0);
                self.set_flag(FLAG_H, false);
                self.set_flag(FLAG_N, false);
                r <<= 1;
                self.set_flag(FLAG_Z, r == 0);
                self.set_r(r, z);
            } else  { // set y
                //println!("PC: 0x{:x} Prefix Opcode: 0x{:2X} not yet implemented", self.pc, prefix_op);
                let new_r = self.get_r(z) | (0x01 << y);
                self.set_r(new_r, z);
            }


        } else if x == 1 { // BIT y, r[z]
            let val = if z == 6 {memory.byte(self.get_rp(RP_HL))} else {self.get_r(z)};
            let res = (val & (1 << y)) == 0;
            self.set_flag(FLAG_H, true);
            self.set_flag(FLAG_N, false);
            self.set_flag(FLAG_Z, res);
        } else if x == 2 {
            let mut val = if z == 6 {memory.byte(self.get_rp(RP_HL))} else {self.get_r(z)};
            val &= !(1 << y);
            if z == 6 {
                memory.set_byte(val, self.get_rp(RP_HL));
            } else {
                self.set_r(val, z);
            }
        } else if x == 3 {
            println!("Prefix Opcode: 0x{:2X} not yet implemented", prefix_op);
        }

    }

    fn push<'b>(&mut self, val: u16, memory: &'b mut Memory) {
        memory.set_byte(((val >> 8) & 0xFF) as u8, self.sp - 1);
        memory.set_byte((val & 0xFF) as u8, self.sp - 2);
        self.sp -= 2;
    }

    fn pop<'b>(&mut self, memory: &'b mut Memory) -> u16 {
        let val = ((memory.byte(self.sp + 1) as u16) << 8) | memory.byte(self.sp) as u16;
        println!("popping: {}", val);
        self.sp += 2;
        val
    }

    pub fn get_flag(&mut self, flag: u8) -> bool{
        if flag == FLAG_C || flag == FLAG_H || flag == FLAG_N || flag == FLAG_Z {
            return (self.f & (1 << flag)) != 0;
        }
        eprintln!("Invalid flag passed to cpu::get_flag");
        self.f > 0
    } 
    
    pub fn set_flag(&mut self, flag: u8, val: bool) {
        if flag == FLAG_C || flag == FLAG_H || flag == FLAG_N || flag == FLAG_Z {
            if val  {
                self.f |= 1 << flag; //set flag 
            } else {
                self.f &= 1 << flag; // clear flag
            }
        } else {
            eprintln!("Invalid flag passed to cpu::set_flag");
        }
    }

    pub fn get_rp(&self, rp: u8) -> u16 {
        if rp == RP_BC {
           return ((self.b as u16) << 8 ) | (self.c as u16);
        } else if rp == RP_DE {
           return ((self.d as u16) << 8 ) | (self.e as u16);
        } else if rp == RP_HL {
           return ((self.h as u16) << 8 ) | (self.l as u16);
        } else if rp == RP_SP {
           return self.sp;
        }
        0
    }

    pub fn set_rp(&mut self, val: u16, rp: u8) {
        if rp == RP_BC {
           self.c = (val & 0xFF) as u8;
           self.b = (val >> 8) as u8;
        } else if rp == RP_DE {
           self.e = (val & 0xFF) as u8;
           self.d = (val >> 8) as u8;
        } else if rp == RP_HL {
           self.l = (val & 0xFF) as u8;
           self.h = (val >> 8) as u8;
        } else if rp == RP_SP {
           self.sp = val;
        }
    }

    pub fn set_rp2(&mut self, val: u16, rp: u8) {
        if rp == RP_BC {
           self.c = (val & 0xFF) as u8;
           self.b = (val >> 8) as u8;
        } else if rp == RP_DE {
           self.e = (val & 0xFF) as u8;
           self.d = (val >> 8) as u8;
        } else if rp == RP_HL {
           self.l = (val & 0xFF) as u8;
           self.h = (val >> 8) as u8;
        } else if rp == RP2_AF{
           self.f = (val & 0xFF) as u8;
           self.a = (val >> 8) as u8;
        }
    }



    pub fn get_rp2(&self, rp: u8) -> u16 {
        if rp == RP2_BC || rp == RP2_DE || rp == RP2_HL {
           return self.get_rp(rp);
        } else if rp == RP2_AF{
           return ((self.a as u16) << 8 ) | (self.f as u16);
        }
        0
    }
    
    pub fn get_r(&self, r: u8) -> u8 {
        match r {
            0 => {self.b}
            1 => {self.c}
            2 => {self.d}
            3 => {self.e}
            4 => {self.h}
            5 => {self.l}
            6 => {0} // memory[self.hl]
            7 => {self.a}
            _ => {0}
        }
    }

    pub fn set_r(&mut self, d: u8, r: u8) {
        match r {
            0 => {self.b = d}
            1 => {self.c = d}
            2 => {self.d = d}
            3 => {self.e = d}
            4 => {self.h = d}
            5 => {self.l = d}
            6 => {} // memory[self.hl]
            7 => {self.a = d}
            _ => {}
        }
    }

    pub fn get_cc(&mut self, cc: u8) -> bool {
        if cc == 0 {
            return !self.get_flag(FLAG_Z);
        } else if cc == 1 {
            return self.get_flag(FLAG_Z);
        } else if cc == 2 {
            return !self.get_flag(FLAG_C);
        } else if cc == 3 {
            return self.get_flag(FLAG_C);
        } 
        eprintln!("PC: {} Invalid value passed to cpu::get_cc, value={}", self.pc, cc);
        false
    } 

    pub fn print(&mut self, memory: &Memory) {
        let _stack = self.sp;
        println!("A: {:2x} \t F: {:2x}\t\t{:2x}", self.a, self.f, memory.byte(self.sp));
        println!("B: {:2x} \t C: {:2x}\t\t{:2x}", self.b, self.c, memory.byte(self.sp + 1));
        println!("D: {:2x} \t E: {:2x}\t\t{:2x}", self.d, self.e, memory.byte(self.sp + 2));
        println!("H: {:2x} \t L: {:2x}\t\t{:2x}", self.h, self.l, memory.byte(self.sp + 3));
        println!("AF: {:4x}, BC: {:4x}\t{:2x}", self.get_rp2(RP2_AF), self.get_rp(RP_BC), memory.byte(self.sp + 4));
        println!("DE: {:4x}, HL: {:4x}\t{:2x}", self.get_rp(RP_DE), self.get_rp(RP_HL), memory.byte(self.sp + 5));
        println!("sp: {:4x}, pc: {:4x}\t{:2x}", self.sp, self.pc, memory.byte(self.sp + 6));
    }
}


#[cfg(test)]
mod cpu_tests {
    use crate::Cpu;
    use crate::cpu::*;

    #[test]
    fn arithmatic_decoding() {
        let mut c = Cpu::new();

        // ADD A, B
        c.a = 0x3A;
        c.b = 0xC6;
        c.alu(0, 0, 0, None);
        assert_eq!(c.a, 0);
        assert_eq!(c.get_flag(FLAG_Z), true);
        assert_eq!(c.get_flag(FLAG_H), true);
        assert_eq!(c.get_flag(FLAG_N), false);
        assert_eq!(c.get_flag(FLAG_C), true);

        // ADC A, B
        c.set_flag(FLAG_C, true);
        c.a = 0xE1;
        c.e = 0x0F;
        c.alu(1, 3, 0, None);
        assert_eq!(c.a, 0xF1);
        assert_eq!(c.get_flag(FLAG_Z), false);
        // assert_eq!(c.get_flag(FLAG_H), true);
        assert_eq!(c.get_flag(FLAG_C), false);

        // SUB B
        c.a = 0x3E;
        c.e = 0x3E;
        c.alu(2, 3, 0, None);
        assert_eq!(c.a, 0);
        assert_eq!(c.get_flag(FLAG_Z), true);
        assert_eq!(c.get_flag(FLAG_H), false);
        assert_eq!(c.get_flag(FLAG_N), false);
        assert_eq!(c.get_flag(FLAG_C), false);

        // SBC A, H
        c.set_flag(FLAG_C, true);
        c.a = 0x3B;
        c.h = 0x2A;
        c.alu(3, 4, 0, None);
        assert_eq!(c.a, 0x10);
        assert_eq!(c.get_flag(FLAG_Z), false);
        assert_eq!(c.get_flag(FLAG_H), false);
        assert_eq!(c.get_flag(FLAG_N), true);
        assert_eq!(c.get_flag(FLAG_C), false);

        // AND A, B
        c.a = 0x5A;
        c.l = 0x3F;
        c.alu(4, 5, 0, None);
        assert_eq!(c.a, 0x1A);
        assert_eq!(c.get_flag(FLAG_Z), false);
        // assert_eq!(c.get_flag(FLAG_H), true);
        assert_eq!(c.get_flag(FLAG_N), false);
        assert_eq!(c.get_flag(FLAG_C), false);

        // XOR A, B
        c.a  = 0x5A;
        c.alu(5, 7, 0, None);
        assert_eq!(c.a, 0);
        assert_eq!(c.get_flag(FLAG_Z), true);

        // OR A
        c.a  = 0x5A;
        c.alu(6, 7, 0, None);
        assert_eq!(c.a, 0x5A);
        assert_eq!(c.get_flag(FLAG_Z), false);
        
        // CP A, B
        c.a = 0x3C;
        c.b = 0x2F;
        c.alu(7, 0, 0, None);
        assert_eq!(c.get_flag(FLAG_Z), false);
        assert_eq!(c.get_flag(FLAG_N), true);
        assert_eq!(c.get_flag(FLAG_C), false);


    }
}
