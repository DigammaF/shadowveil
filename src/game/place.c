
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "link.h"
#include "place.h"
#include "hashmap.h"
#include "vector.h"
#include "pawn.h"

struct server_t;

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }
#define UNUSED(x) (void)(x)

void initPlace(place_t* place) {
    initHashmap(&place->pawns);
    initHashmap(&place->features);
	initHashmap(&place->links);
}

void dropPlace(place_t* place) {
	for (unsigned n = 0; n < place->links.capacity; n++) {
		link_t* link = place->links.elements[n];
		if (link == NULL) { continue; }
		dropLink(link);
		free(link);
	}

    dropHashmap(&place->pawns);
    dropHashmap(&place->features);
	dropHashmap(&place->links);
}

void initLink(link_t* link) {
	link->target = NULL;
}

void dropLink(link_t* link) { UNUSED(link); }

link_t* createLink(place_t* source, place_t* destination, char* name) {
	link_t* link = malloc(sizeof(link_t));
	CHECKM(link, "malloc link");
	link->target = destination;
	link->name = name;
	link->id = hashmapLocateUnusedKey(&source->links);
	hashmapSet(&source->links, link->id, link);
	return link;
}

void deleteLink(place_t* place, unsigned key) {
	hashmapSet(&place->links, key, NULL);
}

void makePlain(place_t* place) {
    place->name = "Plaine";
}

void makeDesert(place_t* place) {
    place->name = "Désert";
}

void addPawn(place_t* place, pawn_t* pawn) {
	pawn->pawnKey = hashmapLocateUnusedKey(&place->pawns);
	hashmapSet(&place->pawns, pawn->pawnKey, pawn);
	pawn->place = place;
}

void remPawn(place_t* place, pawn_t* pawn) {
	hashmapSet(&place->pawns, pawn->pawnKey, NULL);
	pawn->place = NULL;
}

void notifyPawnLeft(struct server_t* server, place_t* place, pawn_t* pawn) {
	pawn_move_args_t args = { .pawn = pawn };
	pawn_event_t event = MAKE_EVENT(PAWN_LEFT, &args);

	for (unsigned n = 0; n < place->pawns.capacity; n++) {
		pawn_t* localPawn = place->pawns.elements[n];
		if (localPawn == NULL) { continue; }
		sendPawnEvent(server, localPawn, &event);
	}
}

void notifyPawnArrived(struct server_t* server, place_t* place, pawn_t* pawn) {
	pawn_move_args_t args = { .pawn = pawn };
	pawn_event_t event = MAKE_EVENT(PAWN_ARRIVED, &args);

	for (unsigned n = 0; n < place->pawns.capacity; n++) {
		pawn_t* localPawn = place->pawns.elements[n];
		if (localPawn == NULL) { continue; }
		sendPawnEvent(server, localPawn, &event);
	}
}

void movePawn(struct server_t* server, pawn_t* pawn, link_t* link) {
	place_t* place = pawn->place;

	if (place != NULL) {
		remPawn(place, pawn);
		notifyPawnLeft(server, place, pawn);
	}

	place = link->target;
	addPawn(place, pawn);
	notifyPawnArrived(server, place, pawn);
}
