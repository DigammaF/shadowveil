CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude -Ilantern/include -Llantern
LDFLAGS = -llantern -pthread -g

# Directories
SRC_DIR = src
BUILD_DIR = obj

# Source files
SHARED_SRC = $(SRC_DIR)/function_stack.c $(SRC_DIR)/string_utils.c $(SRC_DIR)/user.c $(SRC_DIR)/command_handlers.c $(SRC_DIR)/stack.c
CLIENT_SRC = $(SRC_DIR)/client.c
SERVER_SRC = $(SRC_DIR)/server.c

# Object files
SHARED_OBJ = $(SHARED_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CLIENT_OBJ = $(CLIENT_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
SERVER_OBJ = $(SERVER_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Targets
CLIENT_BIN = client
SERVER_BIN = server

# Default target
all: $(CLIENT_BIN) $(SERVER_BIN)

# Build client binary
$(CLIENT_BIN): $(SHARED_OBJ) $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build server binary
$(SERVER_BIN): $(SHARED_OBJ) $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile shared code
$(SHARED_OBJ): $(SHARED_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Compile client code
$(CLIENT_OBJ): $(CLIENT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Compile server code
$(SERVER_OBJ): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(CLIENT_BIN) $(SERVER_BIN)

.PHONY: all clean