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

#define MODE DEBUG 

// Slowtime is 500ms
#define SLOWTIME 500000
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
	PPU ppu(memory);

	// PPU test setup (should run a but more than one)
	for (int i = 0; i < 80000; i++) {
		ppu.step();
	}

	// run the cpu cycle in a seperate thread
	std::thread processor([](CPU *cpu) {
		cpu->debug();
		while(true) {
			cpu->step();

			if(cpu->getInstruction() == 0) continue;
			if(MODE == SLOW) usleep(SLOWTIME);
			else if(MODE == RUN) continue;
			else if(MODE == DEBUG) getchar();
		}
	}, &cpu);

	processor.join();

	// Safely quit program
	return 0;
}
