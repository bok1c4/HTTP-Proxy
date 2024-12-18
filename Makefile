# Compiler and flags
CC = clang++
CFLAGS = -Wall -Wextra -g -std=c++17 -I./headers

# Directories
SRC_DIR = .
PROXY_DIR = proxy
BIN_DIR = bin
CONTENT_DIR = content

# Source and object files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/http_server.cpp \
       $(SRC_DIR)/http_sockets.cpp \
       $(SRC_DIR)/utils.cpp \
       $(PROXY_DIR)/proxy.cpp

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

# Debug build with debug symbols
debug: CFLAGS += -DDEBUG
debug: clean build
	@echo "Debug build complete with debug flags."

# Clean all build artifacts
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all build clean debug

