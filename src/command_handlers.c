
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

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }
#define UNUSED(x) (void)(x)

int checkArgCount(socket_t* socket, int count, int expected) {
	if (count == expected) { return 1; }
	char message[255];
	sprintf(
		message,
		"ERROR mauvais nombre d'arguments: %i (attendu: %i)\0", count, expected
	);
	sendData(socket, message);
	return 0;
}

void handleLogin(server_t* server, user_t* user, commandContext_t* context) {
	account_t* account;
	locateAccount(server, context->args[2], account);

	if (account == NULL) {
		printf("\t[introuvable]\n");
		sendData(user->socket, "OUTPUT ce compte n'existe pas\0");
		return;
	}

	if (account->flags | BANNED_FLAG) {
		printf("\t[banni]\n");
		sendData(user->socket, "OUTPUT ce compte est banni\0");
		return;
	}

	if (!checkPassword(account, context->args[3])) {
		printf("\t[mot de passe incorrect]\n");
		sendData(user->socket, "OUTPUT mauvais mot de passe\0");
		return;
	}

	if (account->flags | ADMIN_FLAG) {
		printf("\t[admin]\n");
		popFunction(&user->commandHandlers);
		pushFunction(&user->commandHandlers, adminHandler);
		char message[255];
		sprintf(message, "SET-CONTEXT %i\0", GAMEWORLD);
		sendData(user->socket, message);
		return NULL;
	}

	printf("\t[basique]\n");
	popFunction(&user->commandHandlers);
	pushFunction(&user->commandHandlers, gameworldHandler);
	user->account = account;
	char message[255];
	sprintf(message, "SET-CONTEXT %i\0", GAMEWORLD);
	sendData(user->socket, message);
}

void handleRegister(server_t* server, user_t* user, commandContext_t* context) {
	account_t* account;
	locateAccount(server, context->args[2], account);

	if (account != NULL) {
		printf("\t[existe déjà]\n");
		sendData(user->socket, "OUTPUT ce compte existe déjà\0");
		return;
	}

	printf("\t[allocation...]\n");
	account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	printf("\t[création...]\n");

	if (createAccount(server, account, context->args[2], context->args[3], 0)) {
		printf("\t[réussi]\n");
		user->account = account;
		popFunction(&user->commandHandlers);
		pushFunction(&user->commandHandlers, fillAccountHandler);
		char message[255];
		sprintf(message, "SET-CONTEXT %i\0", GAMEWORLD);
		sendData(user->socket, message);
		return NULL;
	} else {
		printf("\t[impossible]\n");
		sendData(user->socket, "ERROR impossible de créer le compte\0");
		return NULL;
	}
}

void* initialHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	server_t* server = context->server;

	if (!checkArgCount(user->socket, context->count, 4)) { return NULL; }

	if (strcmp(context->args[1], "LOGIN") == 0) {
		printf("(+) chargement de compte\n");
		handleLogin(server, user, context);
		return NULL;
	}

	if (strcmp(context->args[1], "REGISTER") == 0) {
		printf("(+) création d'un compte\n");
		handleRegister(server, user, context);
	}

	char message[255];
	sprintf(
		message,
		"ERROR argument invalide '%s' (attendu: LOGIN|REGISTER)\0", context->args[1]
	);
	sendData(user->socket, message);
	return NULL;
}

void* gameworldHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	char message[255];
	sprintf(message, "OUTPUT '%s'\0", context->args[1]);
	sendData(user->socket, message);
	return NULL;
}

void* adminHandler(void* arg) {
	UNUSED(arg);
	return NULL;
}

/**
 * Appelé durant la phase handleRegister()
 * Assigne un premier personnage au joueur
 */
void* fillAccountHandler(void* arg){
	character_t firstChampion = todo 
	//

	pushFunction(&user->commandHandlers, gameworldHandler);
}
