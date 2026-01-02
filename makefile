# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -g

# Target executable name
TARGET  = invsch.exe

# Source files
SRCS    = main.c \
          hash.c \
          operations.c \
          helpers.c

# Object files (auto-generated)
OBJS    = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c to .o
%.o: %.c invsch.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Full rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild
