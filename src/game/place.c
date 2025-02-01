
#include <stdlib.h>
#include <stdio.h>

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

place_t* makePlain() {
    place_t* place = malloc(sizeof(place_t));
    CHECKM(place, "malloc place");
    place->name = "Plaine";
    return place;
}

place_t* makeDesert() {
    place_t* place = malloc(sizeof(place_t));
    CHECKM(place, "malloc place");
    place->name = "Désert";
    return place;
}
