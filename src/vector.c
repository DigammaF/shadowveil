
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initVector(vector_t* vector) {
    vector->capacity = 1;
    vector->length = 0;
    vector->elements = malloc(sizeof(void*));
    CHECKM(vector->elements, "vector->elements");
}

void dropVector(vector_t* vector) {
    free(vector->elements);
}

void* vectorGet(vector_t* vector, unsigned key) {
    return vector->elements[key];
}

void vectorSet(vector_t* vector, unsigned key, void* value) {
    vector->elements[key] = value;
}

void vectorInsert(vector_t* vector, unsigned key, void* value) {
    if (vector->length + 1 >= vector->capacity) { increaseCapacity(vector); }

    for (unsigned n = vector->length - 1; n >= key; n--) {
        vector->elements[n + 1] = vector->elements[n];
    }

    vector->elements[key] = value;
    vector->length++;
}

void* vectorPop(vector_t* vector, unsigned key) {
    void* value = vector->elements[key];

    for (unsigned n = key; n < vector->length; n++) {
        vector->elements[n] = vector->elements[n + 1];
    }

    vector->length--;
    return value;
}

void increaseCapacity(vector_t* vector) {
    unsigned newCapacity = (unsigned)vector->capacity*GROWTH_RATE;
    if (newCapacity <= vector->capacity) { fprintf(stderr, "cannot increase vector capcity"); exit(EXIT_FAILURE); }
    vector->capacity = newCapacity;
    vector->elements = realloc(vector->elements, vector->capacity*sizeof(void*));
}
