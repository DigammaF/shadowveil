
#ifndef SESSION_H
#define SESSION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket_t.h"

int createSocket(const int mode, socket_t* socketObject);
int closeSocket(socket_t* socket);
int bindLocal(socket_t* socket, const char* address, const short port);
int listenSocket(socket_t* socket, const int maxClientCount);
int acceptRemote(socket_t* socket, socket_t* newSocket);
int connectToRemote(socket_t* socket, const char* address, const short port);
int setRemote(socket_t* socket, const char* address, const short port);

#endif
