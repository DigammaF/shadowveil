
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "lantern.h"
#include "constants.h"
#include "server.h"
#include "stack.h"
#include "function_stack.h"
#include "command_handlers.h"
#include "string_utils.h"

#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initServer(server_t* server) {
	initStack(&server->freeUsers);
	initStack(&server->freeAccounts);

	for (unsigned n = 0; n < MAX_USERS; n++) {
		server->users[n] = NULL;
		push(&server->freeUsers, n);
	}

	for (unsigned n = 0; n < MAX_ACCOUNTS; n++) {
		server->accounts[n] = NULL;
		push(&server->freeAccounts, n);
	}
}

void dropServer(server_t* server) {
	for (unsigned n = 0; n < MAX_USERS; n++) {
		if (server->users[n] != NULL) {
			dropUser(server->users[n]);
			free(server->users[n]);
		}
	}

	for (unsigned n = 0; n < MAX_ACCOUNTS; n++) {
		if (server->accounts[n] != NULL) {
			free(server->accounts[n]);
		}
	}

	dropStack(&server->freeUsers);
	dropStack(&server->freeAccounts);
}

int createUser(server_t* server, user_t* user, socket_t* socket) {
	if (stackEmpty(&server->freeUsers)) {
		printf("(!) impossible de trouver un emplacement utilisateur libre\n");
		return 0;
	}

	user->running = 1;
	user->socket = socket;
	user->lastActivity = time(NULL);
	user->address = pop(&server->freeUsers);
	pushFunction(&user->commandHandlers, initialHandler);
	return 1;
}

void deleteUser(server_t* server, user_t* user) {
	server->users[user->address] = NULL;
	push(&server->freeUsers, user->address);
}

int createAccount(
	server_t* server, account_t* account, char* name, char* password, unsigned flags
) {
	if (stackEmpty(&server->freeAccounts)) {
		printf("(!) impossible de trouver un emplacement de compte libre\n");
		return 0;
	}

	account->name = name;
	account->password = password;
	account->flags = flags;
	account->address = pop(&server->freeAccounts);
	server->accounts[account->address] = account;
	return 1;
}

void deleteAccount(server_t* server, account_t* account) {
	push(&server->freeAccounts, account->address);
	server->accounts[account->address] = NULL;
}

void handleNewConnection(server_t* server) {
	printf("(+) accepting client\n");
	socket_t* socket = malloc(sizeof(socket_t));
	CHECKM(socket, "malloc socket");
	acceptRemote(&server->socket, socket);
	user_t* user = malloc(sizeof(user_t));
	CHECKM(user, "malloc user");
	initUser(user);
	user->socket = socket;
	if (createUser(server, user, socket)) {
		printf("(+) created new user\n");
	} else {
		printf("(!) unable to create new user\n");
		closeSocket(socket);
	}
}

void handleUserCommand(server_t* server, user_t* user, char**args, int argCount) {
	if (functionStackEmpty(&user->commandHandlers)) {
		printf("(!) user has no command handler\n");
		return;
	}

	commandContext_t* commandContext = malloc(sizeof(commandContext_t));
	CHECKM(commandContext, "malloc commandContext");
	commandContext->args = args;
	commandContext->count = argCount;
	commandContext->user = user;
	commandContext->server = server;
	function_t commandHandler = peekFunction(&user->commandHandlers);
	commandHandler((void*)commandContext);
	free(commandContext);
	user->lastActivity = time(NULL);
	if (functionStackEmpty(&user->commandHandlers)) { user->running = 0; }
}

void handleUserEnd(user_t* user) {
	closeSocket(user->socket);
	user->running = 0;
}

void handleUserKeepAlive(user_t* user) {
	user->lastActivity = time(NULL);
}

void handleUserRequest(server_t* server, user_t* user) {
	char data[1024];
	recvData(user->socket, data, 1024);
	int argCount;
	char** args = splitString(data, &argCount);

	if (argCount <= 0) {
		printf("(!) user sent no data\n");
		return;
	}

	int processed = 0;

	if (strcmp(args[0], "COMMAND") == 0) {
		processed = 1;
		handleUserCommand(server, user, args, argCount);
	}

	if (strcmp(args[0], "END") == 0) {
		processed = 1;
		handleUserEnd(user);
	}

	if (strcmp(args[0], "KEEP-ALIVE") == 0) {
		processed = 1;
		handleUserKeepAlive(user);
	}

	if (!processed) {
		char message[255];
		sprintf(
			message,
			"ERROR argument invalide '%s' (attendu: COMMAND|END|KEEP-ALIVE)",
			args[0]
		);
		sendData(user->socket, message);
	}

	freeSplit(args, argCount);
}

void update(server_t* server) {
	for (unsigned n = 0; n < MAX_USERS; n++) {
		if (server->users[n] == NULL) { continue; }
		updateUser(server, server->users[n]);
	}
}

void updateUser(server_t* server, user_t* user) {
	if (!user->running) {
		deleteUser(server, user);
		dropUser(user);
	}
}
