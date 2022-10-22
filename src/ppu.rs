use crate::memory::Memory;
use crate::lcd::Lcd;

const HORIZONTAL_BLANK_MODE: i32 = 0;
const VERTICAL_BLANK_MODE: i32 = 1;
const SPRITE_SCAN_MODE: i32 = 2;
const RENDERING_MODE: i32 = 3;

const LCD_CONTROL_REGISTER: u16 = 0xFF40;
const LCD_STATUS_REGISTER: u16 = 0xFF41;
const LCD_SCROLL_Y_REGISTER: u16 = 0xFF42;
const LCD_SCROLL_X_REGISTER: u16 = 0xFF43;
const LCD_LINE_REGISTER: u16 = 0xFF44;

pub struct Ppu<'a> {
    lcd: &'a mut Lcd,
    ticks: i32,
    buffer: [[u8; 144]; 160],
    colors: [u8; 8],
    mode: i32,
    scanline: i32
}

impl<'a> Ppu<'a> {
    pub fn new<'b>(lcd: &'b mut Lcd) -> Ppu<'b> {
        Ppu {
            lcd: lcd,
            ticks: 0,
            buffer: [[0; 144]; 160],
            colors: [0; 8],
            mode: HORIZONTAL_BLANK_MODE,
            scanline: 0
            // TODO: Somehow assign Memory here so that it doesn't have to be passed to all functions dues to ownership conflicts with the CPU.
        }
    }

    pub fn step<'b>(&mut self, memory: &'b mut Memory) {

        // Check if PPU is powered off
        if !self.powered_on(memory) {
            return;
        }

        // Increment PPU cycle ticks
        // TODO: Increment by previously executed CPU instruction ticks instead
        self.ticks += 1;

        // Execute functions based on current PPU mode
        if self.mode == HORIZONTAL_BLANK_MODE && self.ticks >= 102 {
            self.horizontal_blank_mode(memory);
        } else if self.mode == VERTICAL_BLANK_MODE && self.ticks >= 228 {
            self.vertical_blank_mode(memory);
            let mut interupt = memory.byte(0xFF0F);
            interupt = interupt | 0x01;
            memory.set_byte(interupt, 0xFF0F);
        } else if self.mode == SPRITE_SCAN_MODE && self.ticks >= 40 {
            self.sprite_scan_mode(memory);
        } else if self.mode == RENDERING_MODE && self.ticks >= 86 {
            self.rendering_mode(memory);
        }
    }

    fn horizontal_blank_mode<'b>(&mut self, memory: &'b mut Memory) {
        self.ticks = 0;
        self.change_scanline(memory);

        // Check if it's time for a vblank or not
        if self.scanline == 144 {
            self.change_mode(memory, VERTICAL_BLANK_MODE);
            // TODO: Request interrupts from CPU
            self.lcd.draw_frame(self.buffer);
        } else {
            self.change_mode(memory, SPRITE_SCAN_MODE);
        }
    }

    fn vertical_blank_mode<'b>(&mut self, memory: &'b mut Memory) {
        self.ticks = 0;

        // Don't change the scanline on the first one
        if self.scanline != 0 {
            self.change_scanline(memory);
        }

        self.change_mode(memory, SPRITE_SCAN_MODE);
    }

    fn sprite_scan_mode<'b>(&mut self, memory: &'b mut Memory) {
        self.ticks = 0;
        self.change_mode(memory, RENDERING_MODE);
    }

    fn rendering_mode<'b>(&mut self, memory: &'b mut Memory) {
        self.ticks = 0;
        self.change_mode(memory, HORIZONTAL_BLANK_MODE);
        self.build_scanline(memory);
    }

    fn change_mode<'b>(&mut self, memory: &'b mut Memory, new_mode: i32) {
        
        // Set new mode
        self.mode = new_mode;

        // Get bits to check
        let first_bit: i32 = new_mode & 1;
        let second_bit: i32 = (new_mode >> 1) & 1;

        // Get LCD status register value to modify
        let mut lcd_stat = memory.byte(LCD_STATUS_REGISTER);

        // Modify register based on first bit
        if first_bit == 1 {
            lcd_stat |= 1;
        } else {
            lcd_stat &= !(1 as u8);
        }

        // Modify register based on second bit
        if second_bit == 1 {
            lcd_stat |= 1 << 1;
        } else {
            lcd_stat &= !((1 << 1) as u8);
        }

        // Write new value to LCD status register
        memory.set_byte(lcd_stat, LCD_STATUS_REGISTER);
    }

    fn change_scanline<'b>(&mut self, memory: &'b mut Memory) {
        self.scanline += 1;

        // Update line register with current scanline
        let current_line_reg_val = memory.byte(LCD_LINE_REGISTER);
        memory.set_byte(current_line_reg_val + 1, LCD_LINE_REGISTER);

        // Check to wrap scanline around to zero
        if self.scanline > 153 {
            self.scanline = 0;
            memory.set_byte(0, LCD_LINE_REGISTER);
        }
    }

    fn build_scanline<'b>(&mut self, memory: &'b mut Memory) {

        // Don't build more than 144 scanlines
        if self.scanline >= 144 {
            return;
        }

        // Read important register values
        let scroll_x: u8 = memory.byte(LCD_SCROLL_X_REGISTER);
        let scroll_y: u8 = memory.byte(LCD_SCROLL_Y_REGISTER);
        let control_reg_val: u8 = memory.byte(LCD_CONTROL_REGISTER);

        // Read bit 4 to determine which tile data block
        // to read from and whether or not it's signed
        let tile_data_start: u16;
        let is_tile_data_signed: bool;

        if ((control_reg_val >> 4) & 1) != 0 {
            tile_data_start = 0x8000;
            is_tile_data_signed = false;
        } else {
            tile_data_start = 0x8800;
            is_tile_data_signed = true;
        }

        // Read bit 3 to determine which tile map is being used
        let tile_map_start: u16;

        if ((control_reg_val >> 3) & 1) != 0 {
            tile_map_start = 0x9C00;
        } else {
            tile_map_start = 0x9800;
        }

        // Calculate starting y position and tile row
        let y_position: u8 = scroll_y + self.scanline as u8;
        let tile_row: u16 = y_position as u16 / 8;

        // Build en entire 160px scanline
        // 8px wide tile * 20 tiles = 160px
        for i in 0..20 {
            
            // Calculate starting x position, tile column, and tile memory address
            let x_position: u8 = scroll_x + i * 8;
            let tile_column: u16 = x_position as u16 / 8;
            let tile_map_address: u16 = tile_map_start + tile_row as u16 * 32 + tile_column;
            
            // Get current tile number
            let mut tile_num: u16;

            if is_tile_data_signed {
                tile_num = (memory.byte(tile_map_address) as u8) as u16;
            } else {
                tile_num = memory.byte(tile_map_address) as u16;
            }

            // Wrap around tiles if they exceed the 256x256 area
            if !is_tile_data_signed && tile_num <= 127 {
                tile_num += 256;
            }

            // Get current tile data address
            let tile_data_address: u16 = tile_data_start + tile_num * 16;

            // Calc current line in tile
            let line_y: u8 = y_position % 8;

            // Get the two bytes that make up 8 pixels
            let low_byte: u8 = memory.byte(tile_data_address + line_y as u16 * 2);
            let high_byte: u8 = memory.byte(tile_data_address + line_y as u16 * 2 + 1);

            // Get line colors from bytes
            self.get_tile_line_colors(low_byte, high_byte);
            
            // Loop through eight pixel row and load into pixel buffer
            for j in 0..8 {
                self.buffer[i as usize * 8 + j as usize][self.scanline as usize] = self.colors[j];
            }
        }
    }

    fn get_tile_line_colors(&mut self, low_byte: u8, high_byte: u8) {
        for i in 0..8 {
            let low_bit: u8 = (low_byte >> (7 - i)) & 0x01;
            let high_bit: u8 = (high_byte >> (7 - i)) & 0x01;
            self.colors[i] = (high_bit << 1) | low_bit;
        }
    }

    fn powered_on<'b>(&self, memory: &'b mut Memory) -> bool {
        let lcd_cont = memory.byte(LCD_CONTROL_REGISTER);
        
        if (lcd_cont >> 7) & 1 != 0 {
            return true;
        } else {
            return false;
        }
    }
}
