build:
	g++ src/*.cpp -std=c++11 -o bin/emuboy -I include -L lib -l SDL2-2.0.0

run: 
	bin/emuboy

debug: build run
