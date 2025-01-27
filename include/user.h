
#ifndef USER_H
#define USER_H

#include <time.h>

#include "lantern.h"
#include "account.h"
#include "function_stack.h"

typedef struct {
	socket_t* socket;
	account_t* account;
	time_t lastActivity;
	unsigned address;
	function_stack_t commandHandlers;
	int running;
	vector_t*  champions; /* Contient les champion_t de l'utilisateur. */
} user_t;

void initUser(user_t* user);
void dropUser(user_t* user);

#endif
