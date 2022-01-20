#pragma once

#define CPU_CLOCK_SPEED 4200000
#define PPU_CLOCK_SPEED 2100000

class Clock {
    public:
        Clock();

        bool hasCyclePassedCPU();
        bool hasCyclePassedPPU();
    private:
        std::chrono::high_resolution_clock::time_point storedTimeCPU;
        std::chrono::high_resolution_clock::time_point storedTimePPU;
};
