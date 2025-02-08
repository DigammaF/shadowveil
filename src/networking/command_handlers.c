
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
#include "deal.h"

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
	CHECKM(pawn, "malloc pawn");
	initPawn(pawn);
	pawn->account = account;
	account->pawn = pawn;
	pawn->name = strdup(account->name);
	pawn->user = user;
	pawn->gold = 1000;
	pawn->eventHandler = playerEventHandler;
	place_t* spawnPoint = server->world.places[1][1];
	spawnPawn(server, spawnPoint, pawn);
	return pawn;
}

void setupItems(pawn_t* pawn) {
	item_t* item = malloc(sizeof(item_t));
	CHECKM(item, "malloc item");
	initItem(item);
	makeSqueaker(item);
	addItemToPawn(pawn, item);
}

void setupChampions(pawn_t* pawn) {
	champion_t* spider = malloc(sizeof(champion_t));
	CHECKM(spider, "malloc spider");
	initChampion(spider);
	makeSpider(spider);
	addChampionToPawn(pawn, spider);
	pawn->team[0] = spider;
}

void handleRegister(server_t* server, user_t* user, command_context_t* context) {
	account_t* account = locateAccount(server, context->args[2]);

	if (account != NULL) {
		printf("\t[already exists]\n");
		sendData(&user->socket, "OUTPUT ce compte existe déjà");
		return;
	}

	account = malloc(sizeof(account_t));
	CHECKM(account, "malloc account");
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
		sprintf(message, "LIST-LINK %i %s", n, link->name);
		printf("(sending) %s\n", message);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < place->features.capacity; n++) {
		feature_t* feature = place->features.elements[n];
		if (feature == NULL) { continue; }
		sprintf(message, "LIST-FEATURE %i %s", n, feature->name);
		printf("(sending) %s\n", message);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < place->pawns.capacity; n++) {
		pawn_t* pawn = place->pawns.elements[n];
		if (pawn == NULL) { continue; }
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
	if (usedLink == NULL) { fprintf(stderr, "(!) %i could not be found", destinationKey); return; }
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
	feature_t* feature = hashmapGet(&place->features, featureKey);

	if (feature == NULL) { fprintf(stderr, "(!) %i could not be found in features\n", featureKey); return; }

	pawn_interaction_args_t args = { .pawn = pawn };
	interaction_t interaction = MAKE_INTERACTION((interaction_type_t)interactionType, &args);
	triggerFeatureInteraction(context->server, feature, &interaction);
}

void handleUseItem(command_context_t* context) {
	unsigned itemKey;

	if (!safeStrToUnsigned(context->args[2], &itemKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; };

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	item_t* item = hashmapGet(&pawn->items, itemKey);

	if (item == NULL) { fprintf(stderr, "(!) %i could not be found in items\n", itemKey); return; }

	solo_use_args_t args = { };
	use_t use = MAKE_USE(USE_SOLO, &args);
	triggerItemUse(context->server, item, &use);
}

void handleUseItemOnChampion(command_context_t* context) {
	unsigned itemKey;
	unsigned championKey;

	if (!safeStrToUnsigned(context->args[2], &itemKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; };
	if (!safeStrToUnsigned(context->args[3], &championKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[3]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	item_t* item = hashmapGet(&pawn->items, itemKey);
	champion_t* champion = hashmapGet(&pawn->champions, championKey);

	if (item == NULL) { fprintf(stderr, "(!) %i could not be found in items\n", itemKey); return; }
	if (champion == NULL) { fprintf(stderr, "(!) %i could not be found in champions\n", championKey); return; }

	champion_use_args_t args = { .champion = champion };
	use_t use = MAKE_USE(USE_ON_CHAMPION, &args);
	triggerItemUse(context->server, item, &use);
}

void handleMe(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	char message[1024];

	sprintf(message, "ABOUT-PAWN %s", pawn->name);
	sendData(&user->socket, message);

	sprintf(message, "ABOUT-YOUR-GOLD %i", pawn->gold);
	sendData(&user->socket, message);

	for (unsigned n = 0; n < pawn->items.capacity; n++) {
		item_t* item = pawn->items.elements[n];
		if (item == NULL) { continue; }
		sprintf(message, "LIST-ITEM %i %s", n, item->name);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < TEAM_SIZE; n++) {
		champion_t* champion = pawn->team[n];
		if (champion == NULL) { continue; }
		sprintf(message, "LIST-CHAMPION %i %s", champion->pawnKey, champion->name);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void handleSeeChampions(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	char message[1024];

	for (unsigned n = 0; n < pawn->champions.capacity; n++) {
		champion_t* champion = pawn->champions.elements[n];
		if (champion == NULL) { continue; }
		sprintf(message, "LIST-CHAMPION %i %s", n, champion->name);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void handleSelectChampion(command_context_t* context) {
	unsigned teamSpot;
	unsigned championKey;

	if (!safeStrToUnsigned(context->args[3], &teamSpot)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[3]); return; };
	if (!safeStrToUnsigned(context->args[2], &championKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	champion_t* champion = hashmapGet(&pawn->champions, championKey);

	if (champion == NULL) { fprintf(stderr, "(!) could not fetch champion %i\n", championKey); return; }
	if (teamSpot >= TEAM_SIZE) { fprintf(stderr, "(!) team spot too big (%i)\n", teamSpot); return; }

	removeChampionFromTeam(pawn, champion);
	pawn->team[teamSpot] = champion;
}

void handleUnselectChampion(command_context_t* context) {
	unsigned teamSpot;

	if (!safeStrToUnsigned(context->args[2], &teamSpot)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;

	if (teamSpot >= TEAM_SIZE) { fprintf(stderr, "(!) team spot too big (%i)\n", teamSpot); return; }

	pawn->team[teamSpot] = NULL;
}

void handleSellChampion(command_context_t* context) {
	unsigned championKey;
	unsigned price;

	if (!safeStrToUnsigned(context->args[2], &championKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }
	if (!safeStrToUnsigned(context->args[3], &price)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[3]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	champion_t* champion = hashmapGet(&pawn->champions, championKey);

	if (champion == NULL) { fprintf(stderr, "(!) could not fetch champion at %i\n", championKey); return; }

	server_t* server = context->server;
	world_t* world = &server->world;

	champion_deal_t* deal = malloc(sizeof(champion_deal_t));
	CHECKM(deal, "malloc champion deal");
	initChampionDeal(deal);
	deal->champion = champion;
	deal->price = price;
	deal->seller = pawn;
	deal->key = hashmapLocateUnusedKey(&world->championDeals);

	removeChampionFromPawn(pawn, champion);
	addChampionDealToWorld(world, deal);
	notifyChampionRemoved(server, pawn, champion, "mise en vente");
}

void handleBuyChampion(command_context_t* context) {
	unsigned dealKey;

	if (!safeStrToUnsigned(context->args[2], &dealKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	server_t* server = context->server;
	world_t* world = &server->world;
	champion_deal_t* deal = hashmapGet(&world->championDeals, dealKey);

	if (deal == NULL) { fprintf(stderr, "(!) could not fetch champion deal %i\n", dealKey); return; }

	if (deal->price > pawn->gold) {
		sendData(&user->socket, "ERROR pas assez d'or");
		return;
	}

	removeChampionDealFromWorld(world, deal);
	char reason[1024];
	sprintf(reason, "achat du champion %s", deal->champion->name);
	changePawnGold(server, pawn, -deal->price, reason);
	addChampionToPawn(pawn, deal->champion);
	pawn_t* seller = deal->seller;

	if (seller != NULL) {
		sprintf(reason, "vente du champion %s", deal->champion->name);
		changePawnGold(server, seller, deal->price, reason);
	}

	notifyChampionAdded(server, pawn, deal->champion, "achat");
	deal->champion = NULL;
	dropChampionDeal(deal);
	free(deal);
}

void handleCheckMarket(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	server_t* server = context->server;
	world_t* world = &server->world;
	char message[1024];

	for (unsigned n = 0; n < world->championDeals.capacity; n++) {
		champion_deal_t* deal = world->championDeals.elements[n];
		if (deal == NULL) { continue; }
		char* ownsDeal = (deal->seller == NULL) ? "N" : (deal->seller == pawn ? "Y":"N");
		sprintf(message, "LIST-CHAMPION-DEAL %i %i %s %s", n, deal->price, deal->champion->name, ownsDeal);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");

	for (unsigned n = 0; n < world->itemDeals.capacity; n++) {
		item_deal_t* deal = world->itemDeals.elements[n];
		if (deal == NULL) { continue; }
		char* ownsDeal = (deal->seller == NULL) ? "N" : (deal->seller == pawn ? "Y":"N");
		sprintf(message, "LIST-ITEM-DEAL %i %i %s %s", n, deal->price, deal->item->name, ownsDeal);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void handleCancelChampionDeal(command_context_t* context) {
	unsigned dealKey;

	if (!safeStrToUnsigned(context->args[2], &dealKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	server_t* server = context->server;
	world_t* world = &server->world;
	champion_deal_t* deal = hashmapGet(&world->championDeals, dealKey);

	if (deal == NULL) { fprintf(stderr, "(!) could not fetch champion deal %i\n", dealKey); return; }
	if (deal->seller == NULL) { fprintf(stderr, "(!) tried to cancel orphan trade\n"); return; }
	if (deal->seller != pawn) { fprintf(stderr, "(!) tried to cancel unowned deal\n"); return; }

	removeChampionDealFromWorld(world, deal);
	addChampionToPawn(pawn, deal->champion);
	notifyChampionAdded(server, pawn, deal->champion, "annulation de vente");
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

		if (strcmp(context->args[1], "ME") == 0) {
			handleMe(context);
			return NULL;
		}

		if (strcmp(context->args[1], "SEE-CHAMPIONS") == 0) {
			handleSeeChampions(context);
			return NULL;
		}

		if (strcmp(context->args[1], "CHECK-MARKET") == 0) {
			handleCheckMarket(context);
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

		if (strcmp(context->args[1], "UNSELECT-CHAMPION") == 0) {
			handleUnselectChampion(context);
			return NULL;
		}

		if (strcmp(context->args[1], "BUY-CHAMPION") == 0) {
			handleBuyChampion(context);
			return NULL;
		}

		if (strcmp(context->args[1], "CANCEL-CHAMPION-DEAL") == 0) {
			handleCancelChampionDeal(context);
			return NULL;
		}
	}

	if (context->count == 4) {
		if (strcmp(context->args[1], "INTERACT") == 0) {
			handleInteract(context);
			return NULL;
		}

		if (strcmp(context->args[1], "USE-ITEM-ON-CHAMPION") == 0) {
			handleUseItemOnChampion(context);
			return NULL;
		}

		if (strcmp(context->args[1], "SELECT-CHAMPION") == 0) {
			handleSelectChampion(context);
			return NULL;
		}

		if (strcmp(context->args[1], "SELL-CHAMPION") == 0) {
			handleSellChampion(context);
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
