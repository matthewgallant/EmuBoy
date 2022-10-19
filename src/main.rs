mod cpu;
mod memory;
mod cartridge;
mod lcd;
mod ppu;

use cpu::Cpu;
use memory::Memory;
use cartridge::Cartridge;
use lcd::Lcd;
use ppu::Ppu;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;

use std::env;
use std::process;

fn main() {

    // Get and check for arguments
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("No ROM passed, exiting...");
        process::exit(1);
    }

    // Create memory
    let mut memory = Memory::new();

    // Create cartridge from file and get first 32k of contents
    let cart = Cartridge::new(args[1].to_string());

    // Only ask for the second 16k as the first 16k isn't able to be banked and is returned automatically
    let cart_contents = cart.cartridge(0x4000, 0x8000);

    // Create LCD
    let mut lcd = Lcd::new();

    // TODO: Move event handler to new IO struct
    let mut event_pump = lcd.context.event_pump().unwrap();

    // Create PPU
    let mut ppu = Ppu::new(&mut lcd);

    // Lastly create the CPU
    let mut cpu = Cpu::new();

    'running: loop {
        cpu.step(&mut memory);
        ppu.step(&mut memory);
        //std::thread::sleep(std::time::Duration::from_millis(500));

        // TODO: Move event loop to new IO struct
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} |
                Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'running
                },
                _ => {}
            }
        }
    }
}
