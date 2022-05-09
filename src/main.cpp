#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <unistd.h>

#include "cpu.hpp"
#include "memory.hpp"
#include "ppu.hpp"
#include "lcd.hpp"
#include "cartridge.hpp"
#include "clock.hpp"
#include "utilities.hpp"

#define DEBUG 0
#define RUN 1
#define SLOW 2

#define MODE SLOW 

// Slowtime is 500ms
#define SLOWTIME 20000

int main(int argc, char **argv) {

	// Check if ROM is passed as an argument
	if (argc != 2) {
		std::cout << "No ROM passed as an argument" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Initialize cartridge with ROM file from args
	Cartridge cartridge(argv[1]);
	std::vector<uint8_t> cartridgeContents = cartridge.getCartridgeContents(0x4000, 0x8000);

	// Print rom hex for debugging
	// debugBytes(cartridgeContents);

	// Create virtual memory
	Memory memory;
	memory.write(cartridgeContents, 0x0000);

	// Create virtual clock (crystal)
	Clock clock;
	
	// Create virtual cpu
    CPU cpu;
	cpu.setMemory(&memory);

	// run the cpu cycle in a seperate thread
	std::thread processor([](CPU *cpu) {
		Memory *memory = cpu->getMemory();
		uint8_t i = 0;
		while(true) {
			cpu->debug();
			cpu->step();

			// write to the first section of memory as indicator 
			// threads have some shared memory
			memory->write(i++, VRAM_OFFSET);

			//if(cpu->getInstruction() == 0) continue;
			if(MODE == SLOW) usleep(SLOWTIME);
			else if(MODE == RUN) continue;
			else if(MODE == DEBUG) getchar();
		}
	}, &cpu);
	
	// Create virtual ppu
	PPU ppu(&memory);

	// Create virtual lcd
    LCD lcd;

	while(true) {
		lcd.drawFrame(*ppu.debugTiles());
		lcd.handle_quit();
	}

	processor.join();
	printf("%x", OAM_2_VRAM(0x80));

	// Safely quit program
	return 0;
}
