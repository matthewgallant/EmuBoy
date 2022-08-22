mod cpu;

extern crate sdl2;

use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::rect::Rect;
use std::time::Duration;
use std::fs;
use std::env;
use std::process;


fn main() {
    let args: Vec<String> = env::args().collect();
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    if args.len() < 2 {
        println!("No ROM passed, exiting");
        process::exit(1);
    }

    // right now this is directly execeuting on whatever is in the 2048gb binary 
    // THIS ISN'T CORRECT lol
    let mut processor = cpu::cpu_builder();
    let data = fs::read(&args[1]).unwrap();
    processor.execute(&data.into_boxed_slice());
    

    let window = video_subsystem.window("EmuBoy", 800, 600)
        .position_centered()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();
    
    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();


    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut i = 0;
    'running: loop {
        canvas.clear();
        i = (i + 1) % 255;
        canvas.set_draw_color(Color::RGB(i, 64, 255 -i));
        canvas.fill_rect(Rect::new(50, 50, 700, 500)).ok();
        canvas.set_draw_color(Color::RGB(255 - i, 64, i));
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} |
                Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'running
                },
                _ => {}
            }
        }

    canvas.present();
    ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }

    println!("Hello, world!");
}
