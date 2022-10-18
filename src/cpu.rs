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
    pub l: u8
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
        }
    }

    pub fn step<'b>(&mut self, memory: &'b mut Memory) {
        println!("Stepping CPU...");
        self.execute(memory);
    }



    pub fn execute<'b>(&mut self, memory: &'b mut Memory) {
        println!("Op Code: 0x{:02X} and pc: {}", memory.memory[self.pc as usize], self.pc);
        let opcode = memory.memory[self.pc as usize];
        let z = opcode & 0x7;
        let y = (opcode >> 3) & 0x7;
        let x = (opcode >> 6) & 0x3;
        let p = (opcode >> 3) & 0x1;
        let q = (opcode >> 4) & 0x3; // double check p & q

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

                   } else if y == 3 { // JR d // THIS NEEDS TESTING
                       let d = memory.byte(self.pc + 1) as i16;
                       self.pc = ((self.pc as i16) + d) as u16;
                   } else if y >= 4 && y < 8 {
                       if self.get_flag(y - 4) {
                           let d = memory.byte(self.pc + 1) as i16;
                           self.pc = ((self.pc as i16) + d) as u16;
                       }
                   }

                } 
                1 => {} 
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
                4 => {
                    self.set_flag(FLAG_N, false);
                    self.set_flag(FLAG_H, (((self.get_r(y)) + 1) & 0x10) == 0x10);
                    self.set_flag(FLAG_C, (((self.get_r(y)) + 1) & 0x10) == 0x10);
                    self.set_r(y, self.get_r(y) + 1);
                    self.set_flag(FLAG_Z, self.get_rp(y) == 0);
                }
                5 => {
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
                6 => {  
                    self.set_r(memory.byte(self.pc + 1), y);
                    self.pc += 1;
                }
                7 => {}
                _ => {}
            }

        } else if x == 1 {
            if (z == 6) && (y == 6) {
                // HALT
                return;
            } 
            // LD 
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
                        }
                        6 => { // LD A, (0xFF00 + n)
                            self.set_r(memory.byte(0xFF00 + self.c as u16), self.a);
                        }
                        7 => { // LD HL, SP+ d
                            self.set_rp(self.sp + memory.byte(self.pc + 1) as u16, RP_HL);
                        }
                        _ => {}
                    }
                }
                1 => {}
                2 => {}
                3 => {}
                4 => {}
                5 => {}
                6 => {}
                7 => {}
                _ => {}
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
            return self.f >> flag == 0x01;
        }
        eprintln!("Invalid flag passed to cpu::get_flag");
        return self.f > 0;
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
           self.b = (val & 0xFF) as u8;
           self.c = (val >> 8) as u8;
        } else if rp == RP_DE {
           self.d = (val & 0xFF) as u8;
           self.e = (val >> 8) as u8;
        } else if rp == RP_HL {
           self.h = (val & 0xFF) as u8;
           self.l = (val >> 8) as u8;
        } else if rp == RP_SP {
           self.sp = val;
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
}
