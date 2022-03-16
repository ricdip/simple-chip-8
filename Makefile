# compiler options
CC = g++
CFLAGS = -pedantic -Wall -Wextra -Werror
SDLFLAGS = $(shell pkg-config --cflags --libs sdl2)
CFLAGSDEBUG = -pedantic -Wall -Wextra -Werror -fsanitize=address -g

# sources and objects
SRC = graphics/display.cpp emulation/chip8.cpp main.cpp
OBJ = main.exe
OBJDEBUG = main_debug.exe


.DEFAULT_GOAL: help

# rules
.SILENT: help
help:
	printf "%s \\t\\t %s\n" "help" "show this help message"
	printf "%s \\t\\t %s\n" "all" "build, run and clean application"
	printf "%s \\t\\t %s\n" "clean" "clean application"
	printf "%s \\t\\t %s\n" "build" "build application"
	printf "%s \\t %s\n" "build_debug" "build debug application (fsanitize included)"
	printf "%s \\t\\t %s\n" "run" "run application"
	printf "%s \\t %s\n" "run_debug" "run debug application"

build: $(SRC)
	@echo "Building application files..."
	$(CC) $(CFLAGS) $(SDLFLAGS) $(SRC) -o $(OBJ)

build_debug: $(SRC)
	@echo "Building application files for debug..."
	$(CC) $(CFLAGSDEBUG) $(SDLFLAGS) $(SRC) -o $(OBJDEBUG)

clean:
	@echo "Cleaning application files..."
	rm -f $(OBJ)
	rm -f $(OBJDEBUG)

run: build
	@echo "Running application..."
	./$(OBJ)

run_debug: build_debug
	@echo "Running debug application..."
	./$(OBJDEBUG)

all: clean build run clean


.PHONY: help build build_debug clean run run_debug all
