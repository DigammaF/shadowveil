
#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

#define CHECKKEY(hashmap, key) { if (key > hashmap->capacity) { fprintf(stderr, "%i is not in hashmap bounds [0-%i[\n", key, hashmap->capacity); exit(EXIT_FAILURE); } }

void initHashmap(hashmap_t* hashmap) {
    hashmap->capacity = 1;
    hashmap->elements = malloc(sizeof(void*));
    CHECKM(hashmap->elements, "hashmap->elements");
}

void dropHashmap(hashmap_t* hashmap) {
    free(hashmap->elements);
}

void* hashmapGet(hashmap_t* hashmap, unsigned key) {
    CHECKKEY(hashmap, key);
    return hashmap->elements[key];
}

void hashmapSet(hashmap_t* hashmap, unsigned key, void* value) {
    while (key >= hashmap->capacity) { increaseHashmapCapacity(&hashmap); }
    hashmap->elements[key] = value;
}

void increaseHashmapCapacity(hashmap_t* hashmap) {
    unsigned newCapacity = hashmap->capacity*HASHMAP_GROWTH_RATE + 3;
    if (newCapacity <= hashmap->capacity) { fprintf(stderr, "cannot increase hashmap capacity\n"); exit(EXIT_FAILURE); }
    hashmap->capacity = newCapacity;
    hashmap->elements = realloc(hashmap->elements, hashmap->capacity*sizeof(void*));
    CHECKM(hashmap->elements, "realloc(hashmap->elements)");
}

void dumpHashmap(hashmap_t* hashmap) {
    for (unsigned n = 0; n < hashmap->capacity; n++) {
        if (hashmap->elements[n] == NULL) { continue; }
        printf("%i -> %i ", n, (unsigned)hashmap->elements[n]);
    }
}
