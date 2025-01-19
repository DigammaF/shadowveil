
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include "lantern.h"

#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }

int main() {
	socket_t socket;
	CHECK(createSocket(SOCK_STREAM, &socket), "createSocket");
	CHECK(bindLocal(&socket, "127.0.0.1", 20000), "bindLocal");
	CHECK(listenSocket(&socket, 3), "listenSocket");
	socket_t clientSocket;
	CHECK(acceptRemote(&socket, &clientSocket), "acceptRemote");
	char buffer[1024];
	CHECK(recvData(&clientSocket, (char*)&buffer, 1024), "recvData");
	printf("'%s'\n", buffer);
	CHECK(sendData(&clientSocket, "UwU\0"), "sendData");
	CHECK(closeSocket(&clientSocket), "closeSocket(&clientSocket)");
	CHECK(closeSocket(&socket), "closeSocket(&socket)");
}
