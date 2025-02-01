
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

#include "lantern.h"
#include "constants.h"

#define UNUSED(x) (void)(x)

void setupClientFileDescriptorSet(socket_t* clientSocket, fd_set* fileDescriptorSet, int* maxFileDescriptor) {
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
