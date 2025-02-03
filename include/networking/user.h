
#ifndef USER_H
#define USER_H

#include <time.h>

#include "lantern.h"
#include "function_stack.h"
#include "vector.h"

struct account_t;

typedef struct user_t {
	socket_t socket;
	struct account_t* account; // peut être NULL
	time_t lastActivity;
	unsigned id;
	function_stack_t commandHandlers;
	int running;
} user_t;

void initUser(user_t* user);
void dropUser(user_t* user);

#endif
