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
int main() {
	// Get file from environment
	if (!std::getenv("ROM")) {
		std::cout << "No ROM found in path. Please set 'ROM' in your path to your ROM file." << std::endl;
		return -1;
	}

	//Initialize cartridge
	std::cout << "Loading Cartridge" << std::endl;
	Cartridge cartridge;
	std::vector<uint8_t> cartridgeContents = cartridge.getCartridgeContents(0x4000, 0x8000);

	// Print rom hex for debugging
	debugBytes(cartridgeContents);

	// Create virtual memory
	std::cout << "Create Memory" << std::endl;
	Memory memory;
	memory.write(cartridgeContents, 0x0000);

	// Create virtual clock (crystal)
	Clock clock;
	
	// Create virtual cpu
    CPU cpu;
	cpu.setMemory(&memory);

	// Create virtual ppu

	// PPU test setup (should run a but more than one)
	//for (int i = 0; i < 80000; i++) {
	//	ppu.step();
	//}


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
	
	PPU ppu(&memory);
    LCD lcd;
	// thread for the graphics, every cycle it redraws with the 
	// new vram memory... probably kind of slow
	std::thread graphics([](PPU *ppu, LCD *lcd) {
		while(true) {
			std::vector<int> *buff = ppu->debugTiles();
			if(buff == NULL) continue;
			lcd->drawFrame(*buff);
			lcd->handle_quit();
		}

	}, &ppu, &lcd);

	graphics.join();
	processor.join();
	printf("%x", OAM_2_VRAM(0x80));

	// Safely quit program
	return 0;
}
