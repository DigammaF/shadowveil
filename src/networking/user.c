
#include <stdio.h>

#include "user.h"
#include "function_stack.h"
#include "stack.h"
#include "constants.h"

void initUser(user_t* user) {
	user->account = NULL;
	initFunctionStack(&user->commandHandlers);
	initStack(&user->contexts);
}

void dropUser(user_t* user) {
	dropFunctionStack(&user->commandHandlers);
	dropStack(&user->contexts);
}

void setUserContext(user_t* user, context_t context) {
	char message[COMMUNICATION_SIZE];
	sprintf(message, "SET-CONTEXT %i", (unsigned)context);
	sendData(&user->socket, message);
	push(&user->contexts, (unsigned)context);
}

void popUserContex(user_t* user) {
	pop(&user->contexts);
	context_t context = (context_t)peek(&user->contexts);
	char message[COMMUNICATION_SIZE];
	sprintf(message, "SET-CONTEXT %i", (unsigned)context);
	sendData(&user->socket, message);
}
