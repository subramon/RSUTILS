# Variables
SRC := master.c
SRC += worker.c
OBJ = $(SRC:.c=.o)
TARGET = cancel_demo

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

# Default target: builds the binary
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to compile and immediately run the program
run: $(TARGET)
	@echo "Executing $(TARGET)..."
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean

