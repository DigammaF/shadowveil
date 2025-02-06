
#include <stdlib.h>

#include "hashmap.h"
#include "server.h"
#include "pawn.h"
#include "constants.h"
#include "champion.h"
#include "item.h"

#define UNUSED(x) (void)(x)

void initPawn(pawn_t* pawn) {
    pawn->exhaustion = 0;
	pawn->gold = 0;
    pawn->place = NULL;
    pawn->eventHandler = noHandler;
    pawn->account = NULL;
    pawn->user = NULL;
	initHashmap(&pawn->champions);
	initHashmap(&pawn->items);
}

void dropPawn(pawn_t* pawn) {
	for (unsigned n = 0; n < pawn->champions.capacity; n++) {
		champion_t* champion = pawn->champions.elements[n];
		if (champion == NULL) { continue; }
		dropChampion(champion);
		free(champion);
	}

	for (unsigned n = 0; n < pawn->items.capacity; n++) {
		item_t* item = pawn->items.elements[n];
		if (item == NULL) { continue; }
		dropItem(item);
		free(item);
	}

	dropHashmap(&pawn->champions);
	dropHashmap(&pawn->items);
}

void addItemToPawn(pawn_t* pawn, struct item_t* item) {
	item->pawnKey = hashmapLocateUnusedKey(&pawn->items);
	hashmapSet(&pawn->items, item->pawnKey, item);
	item->pawn = pawn;
}

void removeItemFromPawn(pawn_t* pawn, struct item_t* item) {
	hashmapSet(&pawn->items, item->pawnKey, NULL);
	item->pawn = NULL;
}

void addChampionToPawn(pawn_t* pawn, struct champion_t* champion) {
	champion->pawnKey = hashmapLocateUnusedKey(&pawn->champions);
	hashmapSet(&pawn->champions, champion->pawnKey, champion);
	champion->pawn = pawn;
}

void removeChampionToPawn(pawn_t* pawn, struct champion_t* champion) {
	hashmapSet(&pawn->champions, champion->pawnKey, NULL);
	champion->pawn = NULL;
}

void sendPawnEvent(server_t* server, pawn_t* pawn, event_t* event) {
    event->server = server;
    event->pawn = pawn;
    pawn->eventHandler((void*)event);
}
