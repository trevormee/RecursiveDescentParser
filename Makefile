# Makefile for Scanner Project

# Compiler settings
CC = gcc
CFLAGS = -Wall -g

# Output executable name
TARGET = scanner

# Source files
SRC = main.c scanner.c

# Object files (generated from source files)
OBJ = $(SRC:.c=.o)

# Include directories (if needed, adjust if you have extra directories)
INCLUDES = -I.

# Default target: build the executable
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Rule to compile .c files into .o object files
%.o: %.c scanner.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rule to run the scanner program
run: $(TARGET)
	./$(TARGET) $(SOURCE_FILE)

