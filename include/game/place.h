
#ifndef PLACE_H
#define PLACE_H

#include "link.h"
#include "hashmap.h"
#include "vector.h"

typedef struct {
    char* name;
    hashmap_t links; // link_t*, possède la valeur
    hashmap_t pawns; // pawn_t*
    hashmap_t features; // feature_t*
} place_t;

typedef struct {
    char* name;
    place_t* target;
	unsigned id;
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
