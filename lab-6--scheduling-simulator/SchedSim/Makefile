CC = gcc
CFLAGS = -Wall -g -std=c99

# Target executable
TARGET = schedsim

# Source files
SRCS = schedsim.c util.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
