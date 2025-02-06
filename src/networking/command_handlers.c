
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lantern.h"
#include "command_handlers.h"
#include "user.h"
#include "server.h"
#include "account.h"
#include "constants.h"
#include "function_stack.h"
#include "string_utils.h"
#include "pawn.h"
#include "place.h"
#include "event_handlers.h"
#include "feature.h"
#include "item.h"
#include "champion.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }
#define UNUSED(x) (void)(x)

int checkArgCount(user_t* user, int count, int expected) {
	if (count == expected) { return 1; }
	printf("(!) [%i] user sent %i args (expected: %i)", user->id, count, expected);
	return 0;
}

void handleLogin(server_t* server, user_t* user, command_context_t* context) {
	account_t* account = locateAccount(server, context->args[2]);

	if (account == NULL) {
		printf("\t[cannot find]\n");
		sendData(&user->socket, "OUTPUT ce compte n'existe pas");
		return;
	}

	if (account->flags & BANNED_FLAG) {
		printf("\t[banned]\n");
		sendData(&user->socket, "OUTPUT ce compte est banni");
		return;
	}

	if (!checkPassword(account, context->args[3])) {
		printf("\t[wrong password]\n");
		sendData(&user->socket, "OUTPUT mauvais mot de passe");
		return;
	}

	if (account->flags & ADMIN_FLAG) {
		printf("\t[admin]\n");
		popFunction(&user->commandHandlers);
		pushFunction(&user->commandHandlers, adminHandler);
		char message[255];
		sprintf(message, "SET-CONTEXT %i", ADMIN);
		sendData(&user->socket, message);
		return;
	}

	printf("\t[standard]\n");
	popFunction(&user->commandHandlers);
	pushFunction(&user->commandHandlers, gameWorldHandler);
	user->account = account;
	char message[255];
	sprintf(message, "SET-CONTEXT %i", GAMEWORLD);
	sendData(&user->socket, message);
}

pawn_t* setupPawn(server_t* server, account_t* account, user_t* user) {
	pawn_t* pawn = malloc(sizeof(pawn_t));
	initPawn(pawn);
	pawn->account = account;
	account->pawn = pawn;
	pawn->name = strdup(account->name);
	pawn->user = user;
	pawn->eventHandler = playerEventHandler;
	place_t* spawnPoint = server->world.places[1][1];
	spawnPawn(server, spawnPoint, pawn);
	return pawn;
}

void setupItems(pawn_t* pawn) {
	item_t* item = malloc(sizeof(item_t));
	initItem(item);
	makeSqueaker(item);
	addItemToPawn(pawn, item);
}

void setupChampions(pawn_t* pawn) {

}

void handleRegister(server_t* server, user_t* user, command_context_t* context) {
	account_t* account = locateAccount(server, context->args[2]);

	if (account != NULL) {
		printf("\t[already exists]\n");
		sendData(&user->socket, "OUTPUT ce compte existe déjà");
		return;
	}

	account = malloc(sizeof(account_t));
	CHECKM(account, "account");
	initAccount(account);
	printf("\t[creation of '%s' with password '%s']\n", context->args[2], context->args[3]);
	createAccount(server, account, context->args[2], context->args[3], 0);
	user->account = account;
	printf("\t[spawning pawn]\n");
	pawn_t* pawn = setupPawn(server, account, user);
	printf("\t[giving items]\n");
	setupItems(pawn);
	printf("\t[giving champions]\n");
	setupChampions(pawn);
	printf("\t[success]\n");
	popFunction(&user->commandHandlers);
	pushFunction(&user->commandHandlers, gameWorldHandler);
	char message[255];
	sprintf(message, "SET-CONTEXT %i", GAMEWORLD);
	sendData(&user->socket, message);
	return;
}

void* initialHandler(void* arg) {
	command_context_t* context = (command_context_t*)arg;
	user_t* user = context->user;
	server_t* server = context->server;

	if (!checkArgCount(user, context->count, 4)) { return NULL; }

	if (strcmp(context->args[1], "LOGIN") == 0) {
		printf("(+) fetching account\n");
		handleLogin(server, user, context);
		return NULL;
	}

	if (strcmp(context->args[1], "REGISTER") == 0) {
		printf("(+) creating account\n");
		handleRegister(server, user, context);
		return NULL;
	}

	printf("(!) invalid argument '%s' (expected: LOGIN|REGISTER)\n", context->args[1]);
	return NULL;
}

void handleSee(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	place_t* place = pawn->place;

	char message[1024];
	sprintf(message, "ABOUT-PLACE %s", place->name);
	sendData(&user->socket, message);

	for (unsigned n = 0; n < place->links.capacity; n++) {
		link_t* link = place->links.elements[n];
		if (link == NULL) { continue; }
		char message[1024];
		sprintf(message, "LIST-LINK %i %s", n, link->name);
		printf("(sending) %s\n", message);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < place->features.capacity; n++) {
		feature_t* feature = place->features.elements[n];
		if (feature == NULL) { continue; }
		char message[1024];
		sprintf(message, "LIST-FEATURE %i %s", n, feature->name);
		printf("(sending) %s\n", message);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < place->pawns.capacity; n++) {
		pawn_t* pawn = place->pawns.elements[n];
		if (pawn == NULL) { continue; }
		char message[1024];
		sprintf(message, "LIST-PAWN %i %s", n, pawn->name);
		printf("(sending) %s\n", message);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void handleMessage(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	place_t* place = pawn->place;

	char* text = joinString(&context->args[2], " ");
	char message[1024];
	sprintf(message, "%s: %s", account->name, text);
	message_event_args_t args = { .message = message };
	event_t event = MAKE_EVENT(EVENT_MESSAGE, &args);

	for (unsigned n = 0; n < place->pawns.capacity; n++) {
		pawn_t* localPawn = place->pawns.elements[n];
		if (localPawn == NULL) { continue; }
		sendPawnEvent(context->server, localPawn, &event);
	}

	freeJoin(text);
}

void handleGlobalMessage(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	server_t* server = context->server;
	world_t* world = &server->world;

	char* text = joinString(&context->args[2], " ");
	char message[1024];
	sprintf(message, "%s: %s", account->name, text);
	message_event_args_t args = { .message = message };
	event_t event = MAKE_EVENT(EVENT_MESSAGE, &args);

	for (unsigned n = 0; n < world->pawns.capacity; n++) {
		pawn_t* localPawn = world->pawns.elements[n];
		if (localPawn == NULL) { continue; }
		sendPawnEvent(context->server, localPawn, &event);
	}
	
	freeJoin(text);
}

void handleMove(command_context_t* context){
	user_t* user = context->user;
	server_t* server = context->server;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	place_t* place = pawn->place;
	char message[1024];

	unsigned destinationKey = 0;

	if (!safeStrToUnsigned(context->args[2], &destinationKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }

	link_t* usedLink = hashmapGet(&place->links, destinationKey);
	if (usedLink == NULL) { fprintf(stderr, "(!) Tried to fetch an unexisting link %i", destinationKey); return; }
	movePawn(server, pawn, usedLink);
	sprintf(message, "YOU-MOVED %s", usedLink->target->name);
	sendData(&user->socket, message);
	return;
}

void handleListOnline(command_context_t* context) {
	user_t* user = context->user;
	server_t* server = context->server;
	world_t* world = &server->world;

	for (unsigned n = 0; n < world->pawns.capacity; n++) {
		pawn_t* localPawn = world->pawns.elements[n];
		if (localPawn == NULL) { continue; }
		account_t* localAccount = localPawn->account;
		if (localAccount == NULL) { continue; }
		char message[1024];
		sprintf(message, "LIST-ACCOUNT %s", localAccount->name);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void handleInteract(command_context_t* context) {
	unsigned featureKey;
	unsigned interactionType;

	if (!safeStrToUnsigned(context->args[2], &featureKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }
	if (!safeStrToUnsigned(context->args[3], &interactionType)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[3]); return; }
	if (interactionType >= INTERACTION_COUNT) { fprintf(stderr, "(!) %i is too big\n", interactionType); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	place_t* place = pawn->place;
	feature_t* feature = place->features.elements[featureKey];
	
	if (feature == NULL) { fprintf(stderr, "(!) %i could not be found in features\n", featureKey); return; }

	pawn_interaction_args_t args = { .pawn = pawn };
	interaction_t interaction = MAKE_INTERACTION((interaction_type_t)interactionType, &args);
	triggerFeatureInteraction(context->server, feature, &interaction);
}

void handleUseItem(command_context_t* context) {
	unsigned itemKey;

	if (!safeStrToUnsigned(context->args[2], &itemKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; };

}

void* gameWorldHandler(void* arg) {
	command_context_t* context = (command_context_t*)arg;

	if (context->count == 2) {
		if (strcmp(context->args[1], "SEE") == 0) {
			handleSee(context);
			return NULL;
		}

		if (strcmp(context->args[1], "LIST-ONLINE") == 0) {
			handleListOnline(context);
			return NULL;
		}
	}

	if (context->count == 3) {
		if (strcmp(context->args[1], "MOVE") == 0) {
			handleMove(context);
			return NULL;
		}

		if (strcmp(context->args[1], "USE-ITEM") == 0) {
			handleUseItem(context);
			return NULL;
		}
	}

	if (context->count == 4) {
		if (strcmp(context->args[1], "INTERACT") == 0) {
			handleInteract(context);
			return NULL;
		}
	}

	if (context->count > 2) {
		if (strcmp(context->args[1], "MESSAGE") == 0) {
			handleMessage(context);
			return NULL;
		}

		if (strcmp(context->args[1], "GLOBAL-MESSAGE") == 0) {
			handleGlobalMessage(context);
			return NULL;
		}
	}
	
	printf("(!) unable to parse command\n");
	return NULL;
}

void* adminHandler(void* arg) {
	UNUSED(arg);
	return NULL;
}

void* debugEchoHandler(void* arg) {
	command_context_t* context = (command_context_t*)arg;
	user_t* user = context->user;
	char message[1024];
	char* line = joinString(context->args, " ");
	printf("(received from %i) '%s'\n", user->id, line);
	sprintf(message, "OUTPUT '%s'", line);
	sendData(&user->socket, message);
	return NULL;
}
