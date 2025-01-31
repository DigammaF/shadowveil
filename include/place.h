
#ifndef PLACE_H
#define PLACE_H

#include "link.h"

#define MAX_LINK_COUNT

typedef struct {
    char* name;
    link_t* links[MAX_LINK_COUNT];
} place_t;

#endif
