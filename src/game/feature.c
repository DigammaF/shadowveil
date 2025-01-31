
#include <stdlib.h>

#include "feature.h"
#include "constants.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

#define UNUSED(x) (void)(x)

void initFeature(feature_t* feature) {
    feature->useHandler = noHandler;
}

void dropFeature(feature_t* feature) {
	UNUSED(feature);
}

feature_t* makeRock() {
    feature_t* feature = malloc(sizeof(feature_t));
    initFeature(feature);
    feature->name = "Rocher";
    return feature;
}

feature_t* makeBush() {
    feature_t* feature = malloc(sizeof(feature_t));
    initFeature(feature);
    feature->name = "Buisson";
    return feature;
}
