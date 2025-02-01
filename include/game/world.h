
#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#include "place.h"

#define WORLD_SIZE 65535

typedef struct {
    place_t* places[WORLD_SIZE][WORLD_SIZE]; // possède la valeur
} world_t;

void initWorld(world_t* world);
void dropWorld(world_t* world);

void generateWorld(world_t* world);

#endif
