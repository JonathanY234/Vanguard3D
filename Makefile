# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude `sdl2-config --cflags`

# Linker flags
LDFLAGS = `sdl2-config --libs`

# Source and output files
SRC = src/main.cpp
OUT = Vanguard_3D

# Default target
all: $(OUT)

# Build the output executable
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(OUT)
