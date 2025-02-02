
#ifndef CLIENT_H
#define CLIENT_H

#include "lantern.h"

typedef struct {
	unsigned running;
	socket_t socket;
	char* prompt;
} client_t;

void initClient(client_t* client);
void dropClient(client_t* client);

int mainClient(int argc, const char* argv[]);

#endif
