#pragma once

#define CPU_CLOCK_SPEED 4200000
#define PPU_CLOCK_SPEED 2100000

class Clock {
    public:
        Clock();

        bool canStepCPU();
        bool canStepPPU();
};
