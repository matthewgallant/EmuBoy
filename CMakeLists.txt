cmake_minimum_required(VERSION 3.10)

project(EmuBoy VERSION 0.1)

find_package(SDL2 REQUIRED)

include_directories(src)
include_directories(inc)

file(GLOB EmuBoySrcs src/*.cpp)
set(EmuBoyIncs include/)

add_executable(emuboy ${EmuBoySrcs})
target_include_directories(emuboy PRIVATE ${EmuBoyIncs})
target_link_libraries(emuboy ${SDL2_LIBRARIES})
set_target_properties(emuboy PROPERTIES LINKER_LANGUAGE CXX)
