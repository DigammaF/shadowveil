CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude -Ilantern/include -Llantern
LDFLAGS = -llantern -pthread -g

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/function_stack.c $(SRC_DIR)/string_utils.c $(SRC_DIR)/stack.c $(SRC_DIR)/server.c $(SRC_DIR)/user.c $(SRC_DIR)/command_handlers.c $(SRC_DIR)/vector.c

# Object files
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Targets
BIN = main

# Default target
all: $(BIN)

# Build binary
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object code
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean