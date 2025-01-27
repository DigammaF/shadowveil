
#ifndef PLACE_H
#define PLACE_H

#include "vector.h"

typedef struct {
    char* name;
    vector_t links; // link_t
} place_t;

#endif
