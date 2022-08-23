extern crate sdl2;

use sdl2::pixels::Color;

const WINDOW_WIDTH: u32 = 160;
const WINDOW_HEIGHT: u32 = 144;
const WINDOW_SCALE: u32 = 3;

const COLOR_LIGHT_GREY: i32 = 1;
const COLOR_DARK_GREY: i32 = 2;
const COLOR_BLACK: i32 = 3;

pub struct Lcd {
    pub context: sdl2::Sdl,
    canvas: sdl2::render::Canvas<sdl2::video::Window>
}

impl Lcd {
    pub fn new() -> Self {
        let sdl_context = sdl2::init().unwrap();
        let video_subsystem = sdl_context.video().unwrap();

        let window = video_subsystem.window("EmuBoy", WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE)
            .position_centered()
            .build()
            .unwrap();

        let canvas = window.into_canvas().build().unwrap();

        Self {
            context: sdl_context,
            canvas: canvas
        }
    }

    pub fn draw_frame(&mut self, buffer: [i32; 160 * 144]) {

        // Clear render buffer
        self.canvas.clear();

        // Loop through each pixel and draw it
        for i in 1..WINDOW_HEIGHT {
            for j in 1..WINDOW_WIDTH {
                self.draw_pixel(j as i32, i as i32, buffer[j as usize * i as usize]);
            }
        }

        // Load buffered frame
        self.canvas.present();
    }

    fn draw_pixel(&mut self, pixel_pos_x: i32, pixel_pos_y: i32, pixel_color: i32) {

        // Set pixel color
        self.set_pixel_color(pixel_color);

        // Create the 'pixel' rect
        let pixel_rect = sdl2::rect::Rect::new(pixel_pos_x * WINDOW_SCALE as i32, pixel_pos_y * WINDOW_SCALE as i32, WINDOW_SCALE, WINDOW_SCALE);

        // Render the 'pixel'
        self.canvas.fill_rect(pixel_rect).expect("Could not draw pixel.");
    }

    fn set_pixel_color(&mut self, pixel_color: i32) {
        let (r, g, b): (u8, u8, u8);

        // Define color for pixel
        if pixel_color == COLOR_BLACK {
            (r, g, b) = (0, 0, 0);
        } else if pixel_color == COLOR_DARK_GREY {
            (r, g, b) = (172, 215, 148);
        } else if pixel_color == COLOR_LIGHT_GREY {
            (r, g, b) = (82, 146, 115);
        } else {
            (r, g, b) = (255, 255, 255);
        }

        // Set pixel color
        self.canvas.set_draw_color(Color::RGB(r, g, b));
    }
}
