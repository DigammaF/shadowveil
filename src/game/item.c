
#include <stdlib.h>

#include "lantern.h"
#include "constants.h"
#include "item.h"
#include "pawn.h"
#include "place.h"
#include "user.h"

void initItem(item_t* item) {
	item->params = NULL;
	item->useHandler = noHandler;
}

void dropItem(item_t* item) {
	if (item->params != NULL) { free(item->params); }
}

void makeSqueaker(item_t* item) {
	item->name = "Boite-a-couinement";
	item->params = malloc(sizeof(no_item_params_t));
	item->useHandler = squeakerUseHandler;
}

void triggerItemUse(struct server_t* server, item_t* item, use_t* use) {
	use->server = server;
	use->item = item;
	item->useHandler((void*)use);
	pawn_t* pawn = item->pawn;
	if (pawn == NULL) { return; }
	place_t* place = pawn->place;
	if (place == NULL) { return; }
	item_use_event_args_t args = { .use = use };
	event_t event = MAKE_EVENT(EVENT_ITEM_USE, &args);
	notifyPlace(server, place, &event);
}

void* squeakerUseHandler(void* _) {
	use_t* use = (use_t*)_;
	item_t* item = use->item;
	pawn_t* pawn = item->pawn;
	if (pawn == NULL) { return NULL; }
	user_t* user = pawn->user;
	if (user == NULL) { return NULL; }
	sendData(&user->socket, "OUTPUT *couinement*");
	return NULL;
}
