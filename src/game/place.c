
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "link.h"
#include "place.h"
#include "hashmap.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initPlace(place_t* place) {
	for (unsigned n = 0; n < MAX_LINK_COUNT; n++) { place->links[n] = NULL; }
    initHashmap(place->pawns);
    initHashmap(place->features);
}

void dropPlace(place_t* place) {
	for (unsigned n = 0; n < MAX_LINK_COUNT; n++) {
		if (place->links[n] == NULL) { continue; }
		free(place->links[n]);
		place->links[n] = NULL;
	}
    dropHashmap(place->pawns);
    dropHashmap(place->features);
}

void initLink(link_t* link) {
	link->target = NULL;
}

void dropLink(link_t* link) {
	link->name = NULL;
}

link_t* createLink(place_t* source, place_t* destination, char* name) {
	for (unsigned n = 0; n < MAX_LINK_COUNT; n++) {
		if (source->links[n] != NULL) { continue; }
		link_t* link = malloc(sizeof(link_t));
		CHECKM(link, "malloc link");
		link->target = destination;
		link->name = name;
		source->links[n] = (struct link_t*)link;
		return link;
	}
	fprintf(stderr, "unable to find free key for new link\n");
	exit(EXIT_FAILURE);
}

void deleteLink(place_t* place, unsigned key) {
	dropLink((link_t*)place->links[key]);
	free(place->links[key]);
	place->links[key] = NULL;
}

void makePlain(place_t* place) {
    place->name = "Plaine";
}

void makeDesert(place_t* place) {
    place->name = "Désert";
}
