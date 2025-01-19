
#include <pthread.h>
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

#define UNUSED(x) (void)(x)

#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initServer(server_t* server) {
	initStack(&server->freeUsers);
	initStack(&server->freeAccounts);
	pthread_mutex_init(&server->mutex, NULL);

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

	pthread_mutex_destroy(&server->mutex);
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
	user->thread = malloc(sizeof(pthread_t));
	CHECKM(user->thread, "user->thread");
	pushFunction(&user->commandHandlers, initialHandler);
	userContext_t* context = malloc(sizeof(userContext_t));
	CHECKM(context, "context");
	context->user = user;
	context->server = server;
	pthread_create(user->thread, NULL, handleUser, (void*)context);
	return 1;
}

void deleteUser(server_t* server, user_t* user) {
	server->users[user->address] = NULL;
	push(&server->freeUsers, user->address);
	pthread_cancel(*user->thread);
	free(user->thread);
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

void* handleUser(void* arg) {
	printf("(+) démarrage du gestionnaire de client\n");
	userContext_t* context = (userContext_t*)arg;
	user_t* user = context->user;
	server_t* server = context->server;
	free(context);

	while (user->running) {
		char data[1024];
		recvData(user->socket, data, 1024);
		int argCount;
		char** args = splitString(data, &argCount);

		pthread_mutex_lock(&server->mutex);

		if (!functionStackEmpty(&user->commandHandlers)) {
			if (argCount > 0) {
				int processed = 0;

				if (strcmp(args[0], "COMMAND") == 0) {
					processed = 1;
					commandContext_t* commandContext = malloc(sizeof(commandContext_t));
					CHECKM(commandContext, "commandContext");
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

				if (strcmp(args[0], "END") == 0) {
					processed = 1;
					user->running = 0;
				}

				if (strcmp(args[0], "KEEP-ALIVE") == 0) {
					processed = 1;
				}

				if (!processed) {
					char message[255];
					sprintf(
						message,
						"ERROR argument invalide '%s' (attendu: COMMAND|END|KEEP-ALIVE)",
						args[0]
					);
					sendData(user->socket, message);
					sendData(user->socket, "AWAITING-COMMAND");
				}
			} else {
				sendData(user->socket, "ERROR pas de contenu");
				sendData(user->socket, "AWAITING-COMMAND");
			}
		} else {
			printf("(!) un utilisateur envoie une commande sans contexte\n");
		}

		freeSplit(args, argCount);
		pthread_mutex_unlock(&server->mutex);
	}

	closeSocket(user->socket);
	deleteUser(server, user);
	dropUser(user);
	return NULL;
}

void setupServer(server_t* server) {
	account_t* account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	createAccount(server, account, "admin", "admin", ADMIN);
}

int main(int argc, const char * argv[]) {
	UNUSED(argc); UNUSED(argv);
	server_t server;
	initServer(&server);
	setupServer(&server);
	server.running = 1;
	CHECK(createSocket(SOCK_STREAM, &server.socket), "createSocket");
	CHECK(bindLocal(&server.socket, "127.0.0.1", SERVER_PORT), "bindLocal");
	CHECK(listenSocket(&server.socket, 5), "listenSocket");
	printf("(+) serveur prêt\n");

	while (server.running) {

		printf("(...) en attente d'utilisateur\n");
		socket_t* clientSocket = malloc(sizeof(socket_t));
		CHECKM(clientSocket, "clientSocket");
		CHECK(acceptRemote(&server.socket, clientSocket), "acceptRemote");

		pthread_mutex_lock(&server.mutex);
		printf("(+) nouveau client connecté\n");
		user_t* user = malloc(sizeof(user_t));
		CHECKM(user, "user");
		initUser(user);
		printf("(+) création d'un utilisateur\n");

		if (createUser(&server, user, clientSocket)) {
			server.users[user->address] = user;
			printf("(+) nouveau utilisateur créé\n");
		} else {
			dropUser(user);
			closeSocket(clientSocket);
			free(user);
			free(clientSocket);
			printf("(-) abandon de service\n");
		}

		pthread_mutex_unlock(&server.mutex);
	}

	CHECK(closeSocket(&server.socket), "closeSocket");
	dropServer(&server);
}
