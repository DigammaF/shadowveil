
#include "user.h"
#include "function_stack.h"

void initUser(user_t* user) {
	user->socket = NULL;
	user->account = NULL;
	initFunctionStack(&user->commandHandlers);
}

void dropUser(user_t* user) {
	dropFunctionStack(&user->commandHandlers);
}
