
#ifndef ITEM_H
#define ITEM_H

#include "function_stack.h"

struct pawn_t;
struct server_t;
struct champion_t;

typedef struct item_t {
	char* name;
	struct pawn_t* pawn; // peut être NULL
	unsigned pawnKey;
	void* params; // possède la valeur
	function_t useHandler;
} item_t;

typedef enum {
	USE_ON_CHAMPION, // champion_use_args_t
} use_type_t;

typedef struct use_t {
    use_type_t type;
    struct server_t* server;
    item_t* item;
    void* args;
} use_t;

typedef struct no_item_params_t { } no_item_params_t;

void initItem(item_t* item);
void dropItem(item_t* item);

#define MAKE_USE(t, a) (use_t) { .type = t, .server = NULL, .item = NULL, .args = a }

void makeSqueaker(item_t* item);

void triggerItemUse(struct server_t* server, item_t* item, use_t* use);

typedef struct champion_use_args_t {
	struct champion_t* champion;
} champion_use_args_t;

void* squeakerUseHandler(void* _);

#endif
