
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

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
		struct timeval timeout = { 0, 500 };
		fd_set readDescriptors;
		FD_ZERO(&readDescriptors);
		FD_SET(server.socket.fileDescriptor, &readDescriptors);
		int maxFileDescriptor = server.socket.fileDescriptor;

		for (unsigned n = 0; n < MAX_USERS; n++) {
			if (server.users[n] == NULL) { continue; }
			user_t* user = server.users[n];
			FD_SET(user->socket->fileDescriptor, &readDescriptors);
			if (user->socket->fileDescriptor > maxFileDescriptor) {
				maxFileDescriptor = user->socket->fileDescriptor;
			}
		}

		int count = select(
			maxFileDescriptor + 1,
			&readDescriptors,
			NULL, NULL, &timeout
		);
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
