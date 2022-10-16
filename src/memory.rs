pub struct Memory {
    pub memory: Vec<u8>
}

impl Memory {
    pub fn new() -> Self {
        let mut memory = Memory {
            memory: vec![0; 65536]
        };

        // Initialize I/O register memory addresses on start
        // Below excludes addresses that are set to 0x00 since that's already done up above.
        // See page 18 of http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf for more info.
        memory.set_byte(0x80, 0xFF10); // NR10
        memory.set_byte(0xBF, 0xFF11); // NR11
        memory.set_byte(0xF3, 0xFF12); // NR12
        memory.set_byte(0xBF, 0xFF14); // NR14
        memory.set_byte(0x3F, 0xFF16); // NR21
        memory.set_byte(0xBF, 0xFF19); // NR24
        memory.set_byte(0x7F, 0xFF1A); // NR30
        memory.set_byte(0xFF, 0xFF1B); // NR31
        memory.set_byte(0x9F, 0xFF1C); // NR32
        memory.set_byte(0xBF, 0xFF1E); // NR33
        memory.set_byte(0xFF, 0xFF20); // NR41
        memory.set_byte(0xBF, 0xFF23); // NR30
        memory.set_byte(0x77, 0xFF24); // NR50
        memory.set_byte(0xF3, 0xFF25); // NR51
        memory.set_byte(0xF1, 0xFF26); // NR52
        memory.set_byte(0x91, 0xFF40); // LCDC
        memory.set_byte(0xFC, 0xFF47); // BGP
        memory.set_byte(0xFF, 0xFF48); // OBP0
        memory.set_byte(0xFF, 0xFF49); // OBP1


        return memory;
    }

    pub fn byte(&self, address: u16) -> &u8 {
        return &self.memory[address as usize];
    }

    pub fn bytes(&self, start_address: u16, end_address: u16) -> &[u8] {
        return &self.memory[start_address as usize..end_address as usize];
    }

    pub fn set_byte(&mut self, data: u8, address: u16) {
        self.memory[address as usize] = data;
    }

    pub fn set_bytes(&mut self, data: Vec<u8>, address: u16) {
        self.memory.splice(address as usize..address as usize + data.len(), data);
    }

    pub fn read16(&self, address: u16) -> u16 {
        (self.memory[address as usize] as u16) | ((self.memory[(address + 1) as usize] as u16) << 8)
    }

    pub fn write16(&mut self, data: u16, address: u16) {
        self.set_byte((data & 0xFF) as u8, address);
        self.set_byte((data >> 8) as u8, address + 1);
    }
}
