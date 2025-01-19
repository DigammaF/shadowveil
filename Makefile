CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude -Ilantern/include -Llantern
LDFLAGS = -llantern -pthread -g

SRCS = src/server.c src/string_utils.c src/command_handlers.c src/function_stack.c src/user.c src/stack.c
OBJS = $(SRCS:src/%.c=obj/%.o)
DEPS = server.h string_utils.h constants.h account.h command_handlers.h function_stack.h stack.h user.h

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean