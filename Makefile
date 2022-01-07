IDIR=include
SRC=src
CXX=g++
CXXFLAGS=-I $(IDIR)
ODIR=obj


DEPS=$(IDIR)/*.hpp
OBJ := $(addprefix $(ODIR)/,\
	cpu.o cartridge.o main.o memory.o \
	RegisterFile.o utilities.o)

emuboy: $(OBJ)  
	$(CXX) -o bin/$@ $^ $(CXXFLAGS) 


build_files: 
	mkdir -p $(ODIR) 
	mkdir -p bin 

$(ODIR)/%.o: $(SRC)/%.cpp  
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: build_files emuboy

clean:
	rm -rf $(ODIR)
	rm -rf bin

run: 
	bin/emuboy

debug: all run

