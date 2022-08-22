use std::fs;
use std::io::Read;

pub struct Cartridge {
    pub cartridge: Vec<u8>
}

impl Cartridge {
    pub fn new(file: String) -> Self {
        let mut open_file = fs::File::open(&file).expect("No file found.");
        let metadata = fs::metadata(&file).expect("Unable to read metadata.");
        let mut file_buffer = vec![0; metadata.len() as usize];
        open_file.read(&mut file_buffer).expect("Buffer overflow.");

        Self {
            cartridge: file_buffer
        }
    }

    pub fn cartridge(&self, bank_start: u16, bank_end: u16) -> Vec<u8> {
        return vec![&self.cartridge[0..0x4000], &self.cartridge[bank_start as usize..bank_end as usize]].concat();
    }
}
