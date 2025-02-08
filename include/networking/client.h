
#ifndef CLIENT_H
#define CLIENT_H

#include "lantern.h"
#include "function_stack.h"
#include "vector.h"
#include "ncurses.h"

#define MAX_INPUT 300

typedef struct {
	unsigned running;
	socket_t socket;
	function_stack_t contextHandlers;
	fd_set ioState;
} client_t;

void initClient(client_t* client);
void dropClient(client_t* client);

int mainClient(int argc, const char* argv[]);
int mainGUIClient(int argc, const char* argv[]);

void* initialClientHandler(void* _);
void* loginClienthandler(void* _);
void* registerClientHandler(void* _);
void* gameworldClientHandler(void* _);
void* moveClientHandler(void* _);
void* interactClientHandler(void* _);
void* buyItemClientHandler(void* _);
void* buyChampionClienthandler(void* _);
void* sellItemClienthandler(void* _);
void* sellChampionClientHandler(void* _);

#endif
