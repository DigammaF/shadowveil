
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
		if (server->users[n] == NULL) { continue; } //todo mettre vecteur
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
	// TODO checker fileDescriptorSet pour savoir quoi faire
	
	//si message reçu du serveur
	if (0){
		char data[1024];
		size_t length = 1024;
		memset(data, 0, sizeof(data));
		
		recvData(clientSocket, data, length);
		printf("<<< '%s'\n", data);

	} else {
		//si input clavier
		// TODO lire input et le mettre dans stringToSend
		// sendData(clientSocket, stringToSend);
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
		setupClientFileDescriptorSet( &clientSocket, &fileDescriptorSet, &maxFileDescriptor );
		select(maxFileDescriptor + 1, &fileDescriptorSet, NULL, NULL, &timeout);
		handleClientSockets(&clientSocket, &fileDescriptorSet);
	}
}


// ====== BOTH SERVER AND CLIENT ====== //

int main(int argc, const char* argv[]) {
	if (strcmp(argv[1], "server") == 0) { mainServer(argc, argv); }
	if (strcmp(argv[1], "client") == 0) { mainClient(argc, argv); }

	vector_t vec;
	initVector(&vec);
	dumpVector(&vec);
	vectorAppend(&vec, (void*)8);
	vectorAppend(&vec, (void*)16);
	dumpVector(&vec);
	// printf("%i\n", (unsigned)vectorGet(&vec, 1));
	vectorSet(&vec, 1, (void*)42);
	dumpVector(&vec);
	vectorInsert(&vec, 0, (void*)23);
	dumpVector(&vec);
	vectorPop(&vec, 1);
	dumpVector(&vec);

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
