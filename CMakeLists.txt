# CMakeLists.txt
# EmuBoy
# Initial Author: Jacob Mealey <jacobmealey.maine@gmail.com>

# arbitray minimum, just seemed like a good one
cmake_minimum_required(VERSION 3.2)

# setting project name and version 
project(EmuBoy VERSION 0.1)

# ------ LIBRARIES -------
# below is where libraries are detected, if the system
# being compiled on does not have a librarie we are using
# we want to fail early so we don't waste time compiling
# things if it doesn't work in the end
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

# catch2 version 3
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.0-preview5
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# get files from src and include directories
file(GLOB EmuBoySrcs src/*.cpp)
set(EmuBoyIncs include/)
set(PLATFORM_SPECIFIC_LIBS "-lpthread")

# pthreads are absolute bitch on githubactions
set(PLATFORM_SPECIFIC_LIBS "-lpthread")
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# set up exectuable for emuboy.
add_executable(emuboy ${EmuBoySrcs})

# add includes
target_include_directories(emuboy PRIVATE ${EmuBoyIncs})
target_include_directories(emuboy PRIVATE ${SDL2_INCLUDE_DIRS})

# link libraries 
target_link_libraries(emuboy ${SDL2_LIBRARIES})

## Tests
file(GLOB EmuBoyTests tests/*.cpp)
add_executable(emuboy_test 
    src/cpu.cpp 
    src/RegisterFile.cpp 
    src/utilities.cpp
    ${EmuBoyTests}
)
target_link_libraries(emuboy_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(emuboy_test PRIVATE ${EmuBoyIncs})
include(CTest)
include(Catch)
catch_discover_tests(emuboy_test)
