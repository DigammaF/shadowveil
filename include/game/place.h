
#ifndef PLACE_H
#define PLACE_H

#include "link.h"
#include "hashmap.h"

#define MAX_LINK_COUNT 1024

typedef struct {
    char* name;
    struct link_t* links[MAX_LINK_COUNT]; // possède la valeur
    hashmap_t* pawns; // *pawn_t
    hashmap_t* features; // feature_t*
} place_t;

typedef struct {
    char* name;
    place_t* target;
} link_t;

void initPlace(place_t* place);
void dropPlace(place_t* place);

void initLink(link_t* link);
void dropLink(link_t* link);

link_t* createLink(place_t* source, place_t* destination, char* name);
void deleteLink(place_t* place, unsigned key);

void makePlain(place_t* place);
void makeDesert(place_t* place);

#endif
