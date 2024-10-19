# Compiler settings
CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -O3 -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Target binary
TARGET = $(BIN_DIR)/Vanguard_3D

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Ensure obj and bin directories exist to prevent errors
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Build target
all: $(TARGET)

# Compile the target binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
