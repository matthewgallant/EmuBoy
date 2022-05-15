# CMakeLists.txt
# EmuBoy
# Initial Author: Jacob Mealey <jacobmealey.maine@gmail.com>

# arbitray minimum, just seemed like a good one
cmake_minimum_required(VERSION 3.10)

# setting project name and version 
project(EmuBoy VERSION 0.1)

# ------ LIBRARIES -------
# below is where libraries are detected, if the system
# being compiled on does not have a librarie we are using
# we want to fail early so they don't waste time compiling
# things if it doesn't work in the end
find_package(SDL2 REQUIRED)

# get files from src and include directories
file(GLOB EmuBoySrcs src/*.cpp)
set(EmuBoyIncs include/)

# set up exectuable for emuboy.
add_executable(emuboy ${EmuBoySrcs})
target_include_directories(emuboy PRIVATE ${EmuBoyIncs})
# link libraries 
target_link_libraries(emuboy ${SDL2_LIBRARIES})
