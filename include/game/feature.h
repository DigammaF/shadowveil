
#ifndef FEATURE_H
#define FEATURE_H

#include "function_stack.h"

struct server_t;
struct pawn_t;
struct place_t;

typedef struct feature_t {
    char* name;
    function_t interactionHandler;
	unsigned placeKey;
	struct place_t* place; // can be NULL
	uint16_t interactionFlags; // (1 << interaction_type_t)
} feature_t;

void initFeature(feature_t* feature);
void dropFeature(feature_t* feature);

void makeRock(feature_t* feature);
void makeBush(feature_t* feature);

typedef enum {
	INTERACTION_HELP, INTERACTION_HIRE, INTERACTION_MINE, // pawn_interaction_args_t
	INTERACTION_COUNT
} interaction_type_t;

typedef struct interaction_t {
    interaction_type_t type;
    struct server_t* server;
    feature_t* feature;
    void* args;
} interaction_t;

#define MAKE_INTERACTION(t, a) (interaction_t) { .type = t, .server = NULL, .feature = NULL, .args = a }

void triggerFeatureInteraction(struct server_t* server, feature_t* feature, interaction_t* interaction);

typedef struct pawn_interaction_args_t {
	struct pawn_t* pawn;
} pawn_interaction_args_t;

void* debuginteractionHandler(void* _);

#endif
