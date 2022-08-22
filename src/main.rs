mod cpu;
mod memory;
mod cartridge;
mod lcd;

use memory::Memory;
use cartridge::Cartridge;
use lcd::Lcd;

use std::env;
use std::process;
use std::fs;

fn main() {

    // Get and check for arguments
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("No ROM passed, exiting");
        process::exit(1);
    }

    // Create memory
    let mut mem = Memory::new();

    // Test setting and reading a single byte
    mem.set_byte(0x01, 0x00);
    let byte = mem.byte(0x00);
    println!("Byte: {:?}", byte);

    // Test setting and reading multiple bytes
    mem.set_bytes(vec![0x00, 0x01, 0x02, 0x03], 0x00);
    let bytes = mem.bytes(0x00, 0x04);
    println!("Bytes: {:?}", bytes);

    // Load cartridge from file and get first 32k of contents
    let cart = Cartridge::new(args[1].to_string());
    let cart_contents = cart.cartridge(0x4000, 0x8000);
    println!("Cart contents: {:?}", cart_contents);

    // Create LCD
    let lcd = Lcd::new();

    // right now this is directly execeuting on whatever is in the 2048gb binary 
    // THIS ISN'T CORRECT lol
    let mut processor = cpu::cpu_builder();
    let data = fs::read(&args[1]).unwrap();
    processor.execute(&data.into_boxed_slice());

    println!("Hello, world!");
}
