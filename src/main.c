
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

#include "vector.h"
#include "server.h"
#include "constants.h"
#include "string_utils.h"

#define SERVER_TICK 500 // ms

#define UNUSED(x) (void)(x)
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }


// ====== SERVER ====== //


void setupServer(server_t* server) {
	account_t* account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	createAccount(server, account, "admin", "admin\n", ADMIN_FLAG);
}

void setupServerFileDescriptorSet(
	server_t* server, fd_set* fileDescriptorSet, int* maxFileDescriptor
) {
	FD_ZERO(fileDescriptorSet);
	FD_SET(server->socket.fileDescriptor, fileDescriptorSet);
	*maxFileDescriptor = server->socket.fileDescriptor;

	for (unsigned n = 0; n < MAX_USERS; n++) {
		if (server->users[n] == NULL) { continue; }
		user_t* user = server->users[n];
		FD_SET(user->socket->fileDescriptor, fileDescriptorSet);
		if (user->socket->fileDescriptor > *maxFileDescriptor) {
			*maxFileDescriptor = user->socket->fileDescriptor;
		}
	}
}

void handleServerSockets(server_t* server, fd_set* fileDescriptorSet) {
	if (FD_ISSET(server->socket.fileDescriptor, fileDescriptorSet)) {
		handleNewConnection(server);
	}

	for (unsigned n = 0; n < MAX_USERS; n++) {
		if (server->users[n] == NULL) { continue; }
		user_t* user = server->users[n];
		if (FD_ISSET(user->socket->fileDescriptor, fileDescriptorSet)) {
			handleUserRequest(server, user);
		}
	}
}

int mainServer(int argc, const char* argv[]) {
	UNUSED(argc); UNUSED(argv);
	server_t server;
	initServer(&server);
	setupServer(&server);
	server.running = 1;
	serverSTREAM(&server.socket, "127.0.0.1", SERVER_PORT, 5);
	printf("(+) server ready\n");

	while (server.running) {
		update(&server);
		struct timeval timeout = { 0, SERVER_TICK };
		fd_set fileDescriptorSet;
		int maxFileDescriptor;
		setupServerFileDescriptorSet(&server, &fileDescriptorSet, &maxFileDescriptor);
		select(maxFileDescriptor + 1, &fileDescriptorSet, NULL, NULL, &timeout);
		handleServerSockets(&server, &fileDescriptorSet);
	}

	closeSocket(&server.socket);
	dropServer(&server);
	return 0;
}


// ====== CLIENT ====== //


void setupClientFileDescriptorSet(
	socket_t* clientSocket, fd_set* fileDescriptorSet, int* maxFileDescriptor
) {	
	FD_ZERO(fileDescriptorSet);
	
	//écoute du serveur via le socket client
	FD_SET(clientSocket->fileDescriptor, fileDescriptorSet);
	*maxFileDescriptor = clientSocket->fileDescriptor;
	
	//écoute du clavier local
	int stdinFileDescriptor = fileno(stdin);
	FD_SET(stdinFileDescriptor, fileDescriptorSet);
	if (stdinFileDescriptor > *maxFileDescriptor) { *maxFileDescriptor = stdinFileDescriptor; }
	
	return;
}

void handleClientSockets(socket_t* clientSocket, fd_set* fileDescriptorSet){
	UNUSED(fileDescriptorSet);
	char line[1024];

	if (FD_ISSET(clientSocket->fileDescriptor, fileDescriptorSet)) {
		int byteCount = recvData(clientSocket, line, 1024);
		if (!byteCount) { printf("server closed connection!\n"); exit(EXIT_SUCCESS); }
		printf("(received) '%s'\n", line);
	}

	if (FD_ISSET(fileno(stdin), fileDescriptorSet)) {
		fgets(line, sizeof(line), stdin);
		line[strlen(line) - 1] = '\0';
		printf("(sending) '%s'\n", line);
		sendData(clientSocket, line);
	}
	
	return;
}

int mainClient(int argc, const char* argv[]) {
	UNUSED(argc); UNUSED(argv);
	socket_t clientSocket;
	connectServer(&clientSocket, "127.0.0.1", SERVER_PORT);
	
	while (1) {
		fd_set fileDescriptorSet;
		int maxFileDescriptor;
		struct timeval timeout = { 0, SERVER_TICK };
		setupClientFileDescriptorSet(&clientSocket, &fileDescriptorSet, &maxFileDescriptor);
		select(maxFileDescriptor + 1, &fileDescriptorSet, NULL, NULL, &timeout);
		handleClientSockets(&clientSocket, &fileDescriptorSet);
	}
}


// ====== BOTH SERVER AND CLIENT ====== //


int main(int argc, const char* argv[]) {

	if (argc > 1) {
		if (strcmp(argv[1], "server") == 0) { mainServer(argc, argv); }
		if (strcmp(argv[1], "client") == 0) { mainClient(argc, argv); }
	}

	unsigned count;
	char** parts = splitString("ONE TWO THREE", &count);

	for (unsigned n = 0; n < count; n++) {
		printf("'%s'\n", parts[n]);
	}

	char* text = joinString(parts, "-");
	printf("'%s'\n", text);

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
