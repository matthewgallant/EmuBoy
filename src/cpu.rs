use crate::memory::Memory;


const FLAG_Z: u8 = 7;
const FLAG_N: u8 = 6;
const FLAG_H: u8 = 5;
const FLAG_C: u8 = 4;

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

    pub fn step<'b>(&self, memory: &'b mut Memory) {
        println!("Stepping CPU...");
    }

    pub fn execute(&mut self, memory: &[u8]) {
        println!("Op Code: {} and pc: {}", memory[self.pc as usize], self.pc);
        let opcode = memory[self.pc as usize];
        let z = opcode & 0x7;
        let y = (opcode >> 3) & 0x7;
        let x = (opcode >> 6) & 0x7;
        let p = (opcode >> 3) & 0x1;
        let q = (opcode >> 4) & 0x3; // double check p & q

        // UN prefixed opcdes
        if x == 0 {
            match z {
                0 => {
                    if q == 0 { // LD rp[p] nn

                    } else if q == 1 { // ADD HL rp[p]

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

        } else if x == 1 {
            if (z == 6) && (y == 6) {
                // HALT
                return;
            } 
            // LD 
        } else if x == 2 {
            // ALU operations with y and z 
            self.alu(y, z);
        } else if x == 3 {
            match z {
                0 => {}
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
    }

    pub fn alu(&mut self, y: u8, z: u8) {
        let mut val = match z {
            0 => {self.b}
            1 => {self.c}
            2 => {self.d}
            3 => {self.e}
            4 => {self.h}
            5 => {self.l}
            6 => {0} // memory[self.hl]
            7 => {self.a}
            _ => {0}
        };

        match y {
            0 => { // ADD A
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_H, (((self.a & 0xF) + (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) + ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a += val;
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            1 => { // ADC A
                val += if self.get_flag(FLAG_C) {1} else {0};
                self.set_flag(FLAG_N, false);
                self.set_flag(FLAG_H, (((self.a & 0xF) + (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) + ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a += val;
                self.set_flag(FLAG_Z, self.a == 0);
            } 
            2 => { // SUB
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - ((val as u16) & 0xFF)) & 0x100) == 0x100);
                self.a -= val;
                self.set_flag(FLAG_Z, self.a == 0);
            }  
            3 => { // SBC
                val += if self.get_flag(FLAG_C) {1} else {0};
                self.set_flag(FLAG_N, true);
                self.set_flag(FLAG_H, (((self.a & 0xF) - (val & 0xF)) & 0x10) == 0x10);
                self.set_flag(FLAG_C, ((((self.a as u16) & 0xFF) - ((val as u16) & 0xFF)) & 0x100) == 0x100);
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
            _ => {}
            
        }
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
}
