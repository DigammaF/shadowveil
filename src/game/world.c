
#include <stdlib.h>
#include <stdio.h>

#include "world.h"
#include "random_utils.h"

#define UNUSED(x) (void)(x)
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

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
			place_t* place = world->places[x][y];
			if (place == NULL) { continue; }
			dropPlace(place);
			free(place);
			world->places[x][y] = NULL;
		}
	}
}

void connectToNeighbors(world_t* world, unsigned x, unsigned y) {
	place_t* place = world->places[x][y];

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) { continue; }
			if (getRandomIntInclusive(1, 3) == 1) { continue; }
			unsigned wx = (x + dx) % WORLD_SIZE;
			unsigned wy = (y + dy) % WORLD_SIZE;
			char* name;
			if (dx == 0 && dy > 0) {
				name = "Nord";
			} else if (dx == 0 && dy < 0) {
				name = "Sud";
			} else if (dx > 0 && dy == 0) {
				name = "Est";
			} else if (dx < 0 && dy == 0) {
				name = "Ouest";
			} else if (dx > 0 && dy > 0) {
				name = "Nord Est";
			} else if (dx > 0 && dy < 0) {
				name = "Sud Est";
			} else if (dx < 0 && dy > 0) {
				name = "Nord Ouest";
			} else if (dx < 0 && dy < 0) {
				name = "Sud Ouest";
			}
			place_t* destination = world->places[wx][wy];
			createLink(place, destination, name);
		}
	}
}

void generateWorld(world_t* world) {
	UNUSED(world);
    
	for (unsigned x = 0; x < WORLD_SIZE; x++) {
		for (unsigned y = 0; y < WORLD_SIZE; y++) {
			place_t* place = malloc(sizeof(place_t));
			CHECKM(place, "malloc place");
			initPlace(place);

			if (getRandomIntInclusive(0, 1)) {
				makePlain(place);
			} else {
				makeDesert(place);
			}

			world->places[x][y] = place;
		}
	}

	for (unsigned x = 0; x < WORLD_SIZE; x++) {
		for (unsigned y = 0; y < WORLD_SIZE; y++) {
			connectToNeighbors(world, x, y);
		}
	}
}
