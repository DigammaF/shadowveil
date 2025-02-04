
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

#include "client.h"
#include "lantern.h"
#include "constants.h"

#define UNUSED(x) (void)(x)

void initClient(client_t* client) { UNUSED(client); }

void dropClient(client_t* client) { UNUSED(client); }

void setupClientFileDescriptorSet(client_t* client, fd_set* fileDescriptorSet, int* maxFileDescriptor) {
	socket_t* clientSocket = &client->socket;
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

void handleClientSockets(client_t* client, fd_set* fileDescriptorSet){
	socket_t* clientSocket = &client->socket;

	if (FD_ISSET(clientSocket->fileDescriptor, fileDescriptorSet)) {
		char line[1024];
		int byteCount = recvData(clientSocket, line, 1024);
		if (byteCount == 0) {
			printf("(-) le serveur a mis fin à la connexion\n");
			client->running = 0;
			return;
		}
		printf("(received) '%s'\n", line);

		// for (unsigned n = 0; n < 1024; n++) {
		// 	printf("%c", line[n]);
		// }
		// printf("\n");
	}

	if (FD_ISSET(fileno(stdin), fileDescriptorSet)) {
		char line[512];
		fgets(line, sizeof(line), stdin);
		line[strlen(line) - 1] = '\0';
		printf("(sending) '%s'\n", line);
		char sent[1024];
		sprintf(sent, "COMMAND %s", line);
		sendData(clientSocket, sent);
	}
	
	return;
}

int mainClient(int argc, const char** argv) {
	UNUSED(argc); UNUSED(argv);
	socket_t clientSocket;
	connectServer(&clientSocket, "127.0.0.1", SERVER_PORT);
	client_t client;
	initClient(&client);
	client.socket = clientSocket;
	client.prompt = "( )";
	client.running = 1;
	
	while (client.running) {
		fd_set fileDescriptorSet;
		int maxFileDescriptor;
		struct timeval timeout = { 0, SERVER_TICK };
		setupClientFileDescriptorSet(&client, &fileDescriptorSet, &maxFileDescriptor);
		select(maxFileDescriptor + 1, &fileDescriptorSet, NULL, NULL, &timeout);
		handleClientSockets(&client, &fileDescriptorSet);
	}

	closeSocket(&clientSocket);
	dropClient(&client);
	return 0;
}
