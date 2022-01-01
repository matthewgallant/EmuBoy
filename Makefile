
build:
	g++ src/*.cpp -std=c++11 -o bin/emuboy -I include

run: 
	bin/emuboy

debug: build run
