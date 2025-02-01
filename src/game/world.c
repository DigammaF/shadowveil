
#include <stdlib.h>

#include "world.h"

#define UNUSED(x) (void)(x)

void initWorld(world_t* world) {
	for (unsigned x = 0; x < WORLD_SIZE; x++) {
		for (unsigned y = 0; y < WORLD_SIZE; y++) {
			world->places[x][y] = NULL;
		}
	}
}

void dropWorld(world_t* world) {
	for (unsigned x = 0; x < WORLD_SIZE; x++) {
		for (unsigned y = 0; y < WORLD_SIZE; y++) {
			if (world->places[x][y] == NULL) { continue; }
			dropPlace(world->places[x][y]);
			free(world->places[x][y]);
			world->places[x][y] = NULL;
		}
	}
}

void generateWorld(world_t* world) {
	UNUSED(world);
    // TODO
}
