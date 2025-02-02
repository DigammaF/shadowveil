
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lantern.h"
#include "command_handlers.h"
#include "user.h"
#include "server.h"
#include "account.h"
#include "constants.h"
#include "function_stack.h"
#include "string_utils.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }
#define UNUSED(x) (void)(x)

int checkArgCount(user_t* user, int count, int expected) {
	if (count == expected) { return 1; }
	printf("(!) [%i] user sent %i args (expected: %i)", user->address, count, expected);
	return 0;
}

void handleLogin(server_t* server, user_t* user, commandContext_t* context) {
	account_t* account = locateAccount(server, context->args[2]);

	if (account == NULL) {
		printf("\t[cannot find]\n");
		sendData(user->socket, "OUTPUT ce compte n'existe pas");
		return;
	}

	if (account->flags & BANNED_FLAG) {
		printf("\t[banned]\n");
		sendData(user->socket, "OUTPUT ce compte est banni");
		return;
	}

	if (!checkPassword(account, context->args[3])) {
		printf("\t[wrong password]\n");
		sendData(user->socket, "OUTPUT mauvais mot de passe");
		return;
	}

	if (account->flags & ADMIN_FLAG) {
		printf("\t[admin]\n");
		popFunction(&user->commandHandlers);
		pushFunction(&user->commandHandlers, adminHandler);
		char message[255];
		sprintf(message, "SET-CONTEXT %i", ADMIN);
		sendData(user->socket, message);
		return;
	}

	printf("\t[standard]\n");
	popFunction(&user->commandHandlers);
	pushFunction(&user->commandHandlers, gameWorldHandler);
	user->account = account;
	char message[255];
	sprintf(message, "SET-CONTEXT %i", GAMEWORLD);
	sendData(user->socket, message);
}

void handleRegister(server_t* server, user_t* user, commandContext_t* context) {
	account_t* account = locateAccount(server, context->args[2]);

	if (account != NULL) {
		printf("\t[already exists]\n");
		sendData(user->socket, "OUTPUT ce compte existe déjà");
		return;
	}

	account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	printf("\t[creation of '%s' with password '%s']\n", context->args[2], context->args[3]);

	if (createAccount(server, account, context->args[2], context->args[3], 0)) {
		printf("\t[success]\n");
		user->account = account;
		popFunction(&user->commandHandlers);
		pushFunction(&user->commandHandlers, gameWorldHandler);
		char message[255];
		sprintf(message, "SET-CONTEXT %i", GAMEWORLD);
		sendData(user->socket, message);
		return;
	} else {
		printf("\t[impossible]\n");
		sendData(user->socket, "ERROR impossible de créer le compte");
		return;
	}
}

void* initialHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	server_t* server = context->server;

	if (!checkArgCount(user, context->count, 4)) { return NULL; }

	if (strcmp(context->args[1], "LOGIN") == 0) {
		printf("(+) fetching account\n");
		handleLogin(server, user, context);
		return NULL;
	}

	if (strcmp(context->args[1], "REGISTER") == 0) {
		printf("(+) creating account\n");
		handleRegister(server, user, context);
		return NULL;
	}

	printf("(!) invalid argument '%s' (expected: LOGIN|REGISTER)\n", context->args[1]);
	return NULL;
}

void* gameWorldHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	char message[255];
	sprintf(message, "OUTPUT '%s'", context->args[1]);
	sendData(user->socket, message);
	return NULL;
}

void* adminHandler(void* arg) {
	UNUSED(arg);
	return NULL;
}

void* debugEchoHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	char message[1024];
	char* line = joinString(context->args, " ");
	printf("(received from %i) '%s'\n", user->address, line);
	sprintf(message, "OUTPUT '%s'", line);
	sendData(user->socket, message);
	return NULL;
}

// /**
//  * Appelé durant la phase handleRegister()
//  * Assigne un premier personnage au joueur
//  */
// void* fillAccountHandler(void* arg){
// 	character_t firstChampion = todo 
// 	//

// // 	pushFunction(&user->commandHandlers, gameworldHandler);
// // }
