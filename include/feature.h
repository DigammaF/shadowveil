
#ifndef FEATURE_H
#define FEATURE_H

#include "function_stack.h"

typedef struct {
    function_t useHandler;
} feature_t;

void* noHandler(void* arg);

#endif
