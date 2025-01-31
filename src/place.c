
#include <stdlib.h>

#include "link.h"
#include "place.h"
#include "hashmap.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initPlace(place_t* place) {
    memset(place->links, NULL, sizeof(link_t*)*MAX_LINK_COUNT);
    initHashmap(place->pawns);
    initHashmap(place->features);
}

void dropPlace(place_t* place) {
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
