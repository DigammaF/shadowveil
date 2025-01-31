
#ifndef FEATURE_H
#define FEATURE_H

#include "function_stack.h"

typedef struct {
    char* name;
    function_t useHandler;
} feature_t;

void initFeature(feature_t* feature);
void dropFeature(feature_t* feature);

feature_t* makeRock();
feature_t* makeBush();

#endif
