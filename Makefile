# Compiler and flags
CC = clang++
CFLAGS = -Wall -Wextra -g -std=c++17 -I./header

# Directories
SRC_DIR = .
BIN_DIR = bin
CONTENT_DIR = content

# Source and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/http_server.cpp $(SRC_DIR)/http_sockets.cpp
OBJS = $(SRCS:.cpp=.o)

# Target binary
TARGET = $(BIN_DIR)/main

# Default rule (build everything)
all: build

# Ensure binary directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build the project
build: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .cpp file into a .o file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Debug with clang++ debug flags
debug: $(TARGET)
	$(CC) --debug -o $(TARGET) $(OBJS)
	echo "Server and client built with debug symbols."

# Clean all build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(BIN_DIR)

