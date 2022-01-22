#include <iostream>
#include <chrono>

#include "clock.hpp"

Clock::Clock() {

    // Initialize stored time
    storedTimeCPU = std::chrono::high_resolution_clock::now();
    storedTimePPU = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Checks whether or not a CPU cycle @ 4.2 Mhz has passed
 * 
 * @return true if a cycle has completed
 * @return false if a cycle has not completed
 */
bool Clock::hasCyclePassedCPU() {

    // Store current time
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    // Calculate time duration between when last checked and now
    std::chrono::duration<double> durationDelta = currentTime - storedTimeCPU;

    // Cast duration to microseconds
    std::chrono::microseconds microsecondsDelta = std::chrono::duration_cast<std::chrono::microseconds>(durationDelta);

    // Get minimum passed time to perform a cycle
    // 1 second / CPU clock * 10000000 = 2.38us ~= 2us
    std::chrono::microseconds minClock(1 / CPU_CLOCK_SPEED * 10000000);

    // Check if enough time has passed to allow a clock cycle
    if (microsecondsDelta >= minClock) {
        
        // Update stored time to hold current time
        storedTimeCPU = currentTime;

        return true;
    } else {
        return false;
    }
}

/**
 * @brief Checks whether or not a PPU cycle @ 2.1 Mhz has passed
 * 
 * @return true if a cycle has completed
 * @return false if a cycle has not completed
 */
bool Clock::hasCyclePassedPPU() {
    
    // Store current time
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    // Calculate time duration between when last checked and now
    std::chrono::duration<double> durationDelta = currentTime - storedTimePPU;

    // Cast duration to microseconds
    std::chrono::microseconds microsecondsDelta = std::chrono::duration_cast<std::chrono::microseconds>(durationDelta);

    // Get minimum passed time to perform a cycle
    // 1 second / PPU clock * 10000000 = 4.76us ~= 5us
    std::chrono::microseconds minClock(1 / PPU_CLOCK_SPEED * 10000000);

    // Check if enough time has passed to allow a clock cycle
    if (microsecondsDelta >= minClock) {
        
        // Update stored time to hold current time
        storedTimePPU = currentTime;

        return true;
    } else {
        return false;
    }
}
