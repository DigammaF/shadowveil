
#ifndef LINK_H
#define LINK_H

#include "place.h"

typedef struct {
    char* name;
    place_t* target;
} link_t;

#endif
