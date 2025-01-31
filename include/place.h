
#ifndef PLACE_H
#define PLACE_H

#include "link.h"
#include "hashmap.h"

#define MAX_LINK_COUNT 1024

typedef struct {
    char* name;
    link_t* links[MAX_LINK_COUNT];
    hashmap_t* pawns; // *pawn_t
    hashmap_t* features; // feature_t*
} place_t;

void initPlace(place_t* place);
void dropPlace(place_t* place);

place_t* makePlain();
place_t* makeDesert();

#endif
