# EmuBoy
WIP GameBoy emulator written in C++ with SDL2.
![Screenshot of 2048-gb running in EmuBoy](/images/screenshot.png)

## Building 
EmuBoy requires SDL2 and CMake (minimum 3.10) to build. To build EmuBoy run the following commands in the EmuBoy directory:
```bash
mkdir build && cd build
cmake .. 
make
```

## Using EmuBoy
If you are not in the build directory, navigate to it and then you can run the program using the following command:
```bash
./emuboy /path/to/legally/obtained/rom
```
