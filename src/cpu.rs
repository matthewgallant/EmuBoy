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

#[derive(PartialEq)]
pub enum CpuMode {
    RUN,
    HALT,
    STOP,
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
            mode: CpuMode::RUN,
            interrupts: true
        }
    }

    pub fn step<'b>(&mut self, memory: &'b mut Memory) {
        if self.mode == CpuMode::RUN {
            println!("Stepping CPU...");
            self.execute(memory);

        }
    }



    pub fn execute<'b>(&mut self, memory: &'b mut Memory) {
        println!("Op Code: 0x{:02X} and pc: {}", memory.memory[self.pc as usize], self.pc);
        self.print(memory);
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
                       self.mode = CpuMode::STOP; 
                   } else if y == 3 { // JR d // THIS NEEDS TESTING
                       let d = memory.byte(self.pc + 1) as i8;
                       self.pc = ((self.pc as i16) + d as i16) as u16;
                   } else if y >= 4 && y < 8 {  // JR cc[y-4, d
                       if self.get_cc(y-4) {
                           let d = memory.byte(self.pc + 1) as i8;
                           self.pc = ((self.pc as i16) + d as i16) as u16;
                       }
                       self.pc = self.pc + 1;
                   }

                } 
                1 => {
                    if q == 0 { // LD rp[p], nn
                        let nn = memory.read16(self.pc + 1);
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
                    println!("Opcode: {:4x} not yet implemented", opcode);
                }
                _ => {}
            }

        } else if x == 1 {
            if (z == 6) && (y == 6) {
                // HALT
                self.mode = CpuMode::HALT;
                return;
            } 
            // LD 
            self.set_r(self.get_r(z), y);
        } else if x == 2 {
            // ALU operations with y and z 
            self.alu(y, z, memory.byte(self.get_rp(RP_HL)));
        } else if x == 3 {
            match z {
                0 => {
                    match y {
                        0 => { // RET NZ
                            if !self.get_flag(FLAG_Z) {
                                self.pc = self.pop(memory);
                            }
                        }
                        1 => { // RET Z
                            if self.get_flag(FLAG_Z) {
                                self.pc = self.pop(memory);
                            }
                        }
                        2 => { // RET NC
                            if !self.get_flag(FLAG_C) {
                                self.pc = self.pop(memory);
                            }
                        }
                        3 => { // RET C
                            if self.get_flag(FLAG_C) {
                                self.pc = self.pop(memory);
                            }
                        }
                        4 => { // LD (0xFF00 + n), A
                            memory.set_byte(self.a, 0xFF00 + self.c as u16);
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
                        if p == 0 { // RET
                            self.pc = self.pop(memory);
                        } else if p == 1 { // RETI
                            self.pc = self.pop(memory);
                        } else if p == 2 { // JP HL
                            self.pc = self.get_rp(RP_HL);
                        } else if p == 3 { // LD SP, HL
                            self.sp = self.get_rp(RP_HL);
                        }
                    }
                }
                2 => {
                    if y < 4 { // JP cc[y], nn
                        if self.get_cc(y) {
                            self.pc = memory.read16(self.pc + 1);
                        }
                    } else if y == 4 { // LD (0xFF00 + C), A
                        memory.set_byte(self.a, 0xFF00 + self.c as u16);
                    } else if y == 5 { // LD (nn), A
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
                        self.pc = memory.read16(self.pc + 1);
                    } else if y == 1 { // CB prefix 
                        self.cb_prefix(memory.byte(self.pc + 1));
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
                        self.pc = memory.read16(self.pc + 1);
                    }
                }
                5 => {
                    if q == 0 { // PUSH rp2[2]
                        self.push(self.get_rp2(p), memory);
                    } else if p == 0 { // CALL nn
                        self.push(self.pc, memory);
                        self.pc = memory.read16(self.pc + 1);
                    }
                }
                6 => {println!("Opcode: 0x{:2X} not yet implemented", opcode);}
                7 => {println!("Opcode: 0x{:2X} not yet implemented", opcode);}
                _ => {println!("Opcode: 0x{:2X} not yet implemented", opcode);}
            }
        }

        self.pc += 1;
    }

    pub fn alu(&mut self, y: u8, z: u8, mem_hl: u8) {
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
                val += if self.get_flag(FLAG_C) {1} else {0};
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_H, (((self.a & 0xF) + (val & 0xF)) & 0x10) == 0x10);
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
                val += if self.get_flag(FLAG_C) {1} else {0};
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - 
                                        ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a -= val;
                self.set_flag(FLAG_Z, self.a == 0);
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
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            _ => {eprintln!("Unknown ALU instruction...")}
            
        }
    }

    fn cb_prefix(&mut self, prefix_op: u8) {
        let x = prefix_op >> 6;
        let y = (prefix_op >> 3) & 0x7;
        let z = prefix_op & 0x7;
        //println!("running cb prefix: {:2x}", prefix_op);
        if x == 0 { // rot[y] r[z]
            println!("Prefix Opcode: 0x{:2X} not yet implemented", prefix_op);
        } else if x == 1 { // BIT y, r[z]
            let res = (self.get_r(z) & (1 << y)) == 0;
            self.set_flag(FLAG_H, true);
            self.set_flag(FLAG_N, false);
            self.set_flag(FLAG_Z, res);
        } else if x == 2 {
            println!("Prefix Opcode: 0x{:2X} not yet implemented", prefix_op);
        } else if x == 3 {
            println!("Prefix Opcode: 0x{:2X} not yet implemented", prefix_op);
        }

    }

    fn push<'b>(&mut self, val: u16, memory: &'b mut Memory) {
        memory.set_byte((((val & 0xFF00) >> 8) & 0xFF) as u8, self.sp - 1);
        memory.set_byte((val & 0xFF) as u8, self.sp - 2);
        self.sp -= 2;
    }

    fn pop<'b>(&mut self, memory: &'b mut Memory) -> u16 {
        let val = ((memory.byte(self.sp + 1) as u16) << 8) | memory.byte(self.sp) as u16;
        self.sp += 2;
        return val;
    }

    pub fn get_flag(&mut self, flag: u8) -> bool{
        if flag == FLAG_C || flag == FLAG_H || flag == FLAG_N || flag == FLAG_Z {
            return (self.f & (1 << flag)) != 0;
        }
        eprintln!("Invalid flag passed to cpu::get_flag");
        return self.f > 0;
    } 
    
    pub fn set_flag(&mut self, flag: u8, val: bool) {
        if flag == FLAG_C || flag == FLAG_H || flag == FLAG_N || flag == FLAG_Z {
            if val  {
                self.f = self.f | (1 << flag); //set flag 
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
           self.a = (val & 0xFF) as u8;
           self.f = (val >> 8) as u8;
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
        eprintln!("Invalid value passed to cpu::get_cc, value={}", cc);
        return false;
    } 

    pub fn print(&mut self, memory: &Memory) {
        let stack = self.sp;
        println!("A: {:2x} \t F: {:2x}\t\t{:2x}", self.a, self.f, memory.byte(self.sp));
        println!("B: {:2x} \t C: {:2x}\t\t{:2x}", self.b, self.c, memory.byte(self.sp + 1));
        println!("D: {:2x} \t E: {:2x}\t\t{:2x}", self.d, self.e, memory.byte(self.sp + 2));
        println!("H: {:2x} \t L: {:2x}\t\t{:2x}", self.h, self.l, memory.byte(self.sp + 3));
        println!("AF: {:4x}, BC: {:4x}\t{:2x}", self.get_rp2(RP2_AF), self.get_rp(RP_BC), memory.byte(self.sp + 4));
        println!("DE: {:4x}, HL: {:4x}\t{:2x}", self.get_rp(RP_DE), self.get_rp(RP_HL), memory.byte(self.sp + 5));
        println!("sp: {:4x}, pc: {:4x}\t{:2x}", self.sp, self.pc, memory.byte(self.sp + 6));
    }
}
