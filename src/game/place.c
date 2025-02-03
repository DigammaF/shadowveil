
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "link.h"
#include "place.h"
#include "hashmap.h"
#include "vector.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }
#define UNUSED(x) (void)(x)

void initPlace(place_t* place) {
    initHashmap(&place->pawns);
    initHashmap(&place->features);
	initVector(&place->links);
}

void dropPlace(place_t* place) {
	for (unsigned n = 0; n < place->links.length; n++) {
		link_t* link = place->links.elements[n];
		dropLink(link);
		free(link);
	}

    dropHashmap(&place->pawns);
    dropHashmap(&place->features);
	dropVector(&place->links);
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
	vectorAppend(&source->links, link);
	return link;
}

void deleteLink(place_t* place, unsigned key) {
	link_t* link = vectorPop(&place->links, key);
	dropLink(link);
	free(link);
}

void makePlain(place_t* place) {
    place->name = "Plaine";
}

void makeDesert(place_t* place) {
    place->name = "Désert";
}
