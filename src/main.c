
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "server.h"
#include "constants.h"

#define UNUSED(x) (void)(x)
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void setupServer(server_t* server) {
	account_t* account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	createAccount(server, account, "admin", "admin\n", ADMIN_FLAG);
}

int mainServer(int argc, const char* argv[]) {
	server_t server;
	initServer(&server);
	setupServer(&server);
	server.running = 1;
	serverSTREAM(&server.socket, "127.0.0.1", SERVER_PORT, 5);
	printf("(+) serveur prêt\n");

	while (server.running) {

		printf("(...) en attente d'utilisateur\n");
		socket_t* clientSocket = malloc(sizeof(socket_t));
		CHECKM(clientSocket, "clientSocket");
		acceptRemote(&server.socket, clientSocket);

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

	closeSocket(&server.socket);
	dropServer(&server);
	return 0;
}

void getInput(char* input) {
	printf(": ");
	scanf("%s", input);
	size_t len = strlen(input);
	if (len > 0 && input[len - 1] == '\n') {
		input[len - 1] = '\0';
	}
}

int mainClient(int argc, const char* argv[]) {
	socket_t socket;
	connectServer(&socket, "127.0.0.1", SERVER_PORT);

	while (1) {
		sendData(&socket, "COMMAND");
		char data[1024];
		size_t length = 1024;

		do {
			memset(data, 0, sizeof(data));
			recvData(&socket, data, length);
			printf("<<< '%s'\n", data);
		} while (1);
	}
}

int main(int argc, const char* argv[]) {
	if (strcmp(argv[1], "server") == 0) {
		mainServer(argc, argv);
	}

	if (strcmp(argv[1], "client") == 0) {
		mainClient(argc, argv);
	}

	return 0;
}
