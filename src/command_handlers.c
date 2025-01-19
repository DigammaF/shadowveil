
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

void* initialHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	server_t* server = context->server;

	if (context->count == 4) {
		if (strcmp(context->args[1], "LOGIN") == 0) {
			printf("(+) chargement de compte\n");
			for (unsigned n = 0; n < MAX_ACCOUNTS; n++) {
				if (server->accounts[n] == NULL) { continue; }
				account_t* account = server->accounts[n];
				if (strcmp(account->name, context->args[2]) == 0) {
					if (strcmp(account->password, context->args[3]) == 0) {
						if (account->flags | BANNED) {
							printf("\t[banni]\n");
							sendData(user->socket, "OUTPUT ce compte est banni");
							sendData(user->socket, "AWAITING-COMMAND");
							return NULL;
						}

						if (account->flags | ADMIN) {
							printf("\t[admin]\n");
							popFunction(&user->commandHandlers);
							pushFunction(&user->commandHandlers, adminHandler);
							char message[255];
							sprintf(message, "SET-CONTEXT %i", ADMIN_CONTEXT);
							sendData(user->socket, message);
							sendData(user->socket, "AWAITING-COMMAND");
							return NULL;
						}

						printf("\t[basique]\n");
						popFunction(&user->commandHandlers);
						pushFunction(&user->commandHandlers, gameworldHandler);
						user->account = account;
						char message[255];
						sprintf(message, "SET-CONTEXT %i", GAMEWORLD_CONTEXT);
						sendData(user->socket, message);
						sendData(user->socket, "AWAITING-COMMAND");
						return NULL;
					} else {
						printf("\t[mot de passe incorrect]\n");
						sendData(user->socket, "OUTPUT mauvais mot de passe");
						sendData(user->socket, "AWAITING-COMMAND");
						return NULL;
					}
				}
			}

			printf("\t[introuvable]\n");
			sendData(user->socket, "OUTPUT ce compte n'existe pas");
			sendData(user->socket, "AWAITING-COMMAND");
			return NULL;
		}

		if (strcmp(context->args[1], "REGISTER") == 0) {
			printf("(+) création d'un compte\n");
			for (unsigned n = 0; n < MAX_ACCOUNTS; n++) {
				if (server->accounts[n] == NULL) { continue; }
				account_t* account = server->accounts[n];

				if (strcmp(account->name, context->args[2]) == 0) {
					printf("\t[existe déjà]\n");
					sendData(user->socket, "OUTPUT ce compte existe déjà");
					sendData(user->socket, "AWAITING-COMMAND");
					return NULL;
				}
			}

			printf("\t[allocation...]\n");
			account_t* account = malloc(sizeof(account_t));
			CHECKM(account, "account");
			printf("\t[création...]\n");
			if (createAccount(server, account, context->args[2], context->args[3], 0)) {
				printf("\t[réussi]\n");
				user->account = account;
				popFunction(&user->commandHandlers);
				pushFunction(&user->commandHandlers, gameworldHandler);
				char message[255];
				sprintf(message, "SET-CONTEXT %i", GAMEWORLD_CONTEXT);
				sendData(user->socket, message);
				sendData(user->socket, "AWAITING-COMMAND");
				return NULL;
			} else {
				printf("\t[impossible]\n");
				sendData(user->socket, "ERROR impossible de créer le compte");
				sendData(user->socket, "AWAITING-COMMAND");
				return NULL;
			}
		}

		char message[255];
		sprintf(
			message,
			"ERROR argument invalide '%s' (attendu: LOGIN|REGISTER)", context->args[1]
		);
		sendData(user->socket, message);
		sendData(user->socket, "AWAITING-COMMAND");
		return NULL;
	} else {
		char message[255];
		sprintf(
			message,
			"ERROR mauvais nombre d'arguments: %i (attendu: 4)", context->count
		);
		sendData(user->socket, message);
		sendData(user->socket, "AWAITING-COMMAND");
		return NULL;
	}
}

void* gameworldHandler(void* arg) {
	commandContext_t* context = (commandContext_t*)arg;
	user_t* user = context->user;
	char message[255];
	sprintf(message, "OUTPUT '%s'", context->args[1]);
	sendData(user->socket, message);
	sendData(user->socket, "AWAITING-COMMAND");
	return NULL;
}

void* adminHandler(void* arg) {
	UNUSED(arg);
	return NULL;
}
