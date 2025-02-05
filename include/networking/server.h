
#ifndef SERVER_H
#define SERVER_H

#include "lantern.h"
#include "stack.h"
#include "hashmap.h"
#include "world.h"

struct user_t;
struct account_t;

typedef struct server_t {
	int running;
	socket_t socket;
	hashmap_t users; // user_t*, possède la valeur
	hashmap_t accounts; // account_t*, possède la valeur
	world_t world;
} server_t;

typedef struct command_context_t {
	server_t* server;
	struct user_t* user;
	char** args;
	unsigned count;
} command_context_t;

void handleNewConnection(server_t* server);
void handleUserRequest(server_t* server, struct user_t* user);

void initServer(server_t* server);
void dropServer(server_t* server);

void createUser(server_t* server, struct user_t* user, socket_t* socket);
void deleteUser(server_t* server, struct user_t* user);

void createAccount(server_t* server, struct account_t* account, char* name, char* password, unsigned flags);
void deleteAccount(server_t* server, struct account_t* account);
struct account_t* locateAccount(server_t* server, char* name);
int checkPassword(struct account_t* account, char* password);

void update(server_t* server);
void updateUser(server_t* server, struct user_t* user);

int mainServer(int argc, const char* argv[]);

#endif
