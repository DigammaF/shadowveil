
#ifndef SERVER_H
#define SERVER_H

#include "lantern.h"
#include "account.h"
#include "user.h"
#include "stack.h"

#define MAX_USERS 1024
#define MAX_ACCOUNTS 1024

typedef struct {
	pthread_mutex_t mutex;
	int running;
	socket_t socket;
	user_t* users[MAX_USERS];
	stack_t freeUsers;
	account_t* accounts[MAX_ACCOUNTS];
	stack_t freeAccounts;
} server_t;

typedef struct {
	server_t* server;
	user_t* user;
	char** args;
	unsigned count;
} commandContext_t;

typedef struct {
	server_t* server;
	user_t* user;
} userContext_t;

void* handleUser(void* arg);

void initServer(server_t* server);
void dropServer(server_t* server);

int createUser(server_t* server, user_t* user, socket_t* socket);
void deleteUser(server_t* server, user_t* user);

int createAccount(server_t* server, account_t* account, char* name, char* password, unsigned flags);
void deleteAccount(server_t* server, account_t* account);

#endif
