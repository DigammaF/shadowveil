
#ifndef PAWN_H
#define PAWN_H

#include "hashmap.h"
#include "feature.h"
#include "function_stack.h"
#include "item.h"

struct account_t;
struct user_t;
struct server_t;
struct place_t;
struct interaction_t;
struct item_t;
struct champion_t;
struct use_t;

typedef struct pawn_t {
    char* name;
    unsigned exhaustion;
    struct place_t* place;
    function_t eventHandler; // void* eventHandler(event_t*)
    struct account_t* account; // possiblement NULL si entité non joueur
    struct user_t* user; // possiblement NULL si le joueur n'est pas connecté
	unsigned placeKey; // la clé de l'entité dans le dictionnaire .pawns de la place_t dans laquelle elle se situe
	unsigned worldKey; // la clé de l'entité dans le dictionnaire .pawns de world_t
	unsigned gold;
	hashmap_t champions; // champion_t*, possède la valeur
	hashmap_t items; // item_t*, possède la valeur
} pawn_t;

typedef enum {
    EVENT_PAWN_ARRIVED, // pawn_event_args_t
    EVENT_PAWN_LEFT, // pawn_event_args_t
	EVENT_PAWN_SPAWNED, // pawn_event_args_t
	EVENT_MESSAGE, // message_event_args_t
	EVENT_INTERACTION, // interaction_event_args_t
	EVENT_ITEM_USE, // item_use_event_args_t
} event_type_t;

typedef struct event_t {
    event_type_t type;
    struct server_t* server;
    pawn_t* pawn;
    void* args;
} event_t;

#define MAKE_EVENT(t, a) (event_t) { .type = t, .server = NULL, .pawn = NULL, .args = a }

void initPawn(pawn_t* pawn);
void dropPawn(pawn_t* pawn);

void addItemToPawn(pawn_t* pawn, struct item_t* item);
void removeItemFromPawn(pawn_t* pawn, struct item_t* item);

void addChampionToPawn(pawn_t* pawn, struct champion_t* champion);
void removeChampionToPawn(pawn_t* pawn, struct champion_t* champion);

/**
 * 
 *  Envoie un évènement à l'entité
 *  il suffit de renseigner event.type et event.args
 * 
 */
void sendPawnEvent(struct server_t* server, pawn_t* pawn, event_t* event);

typedef struct pawn_event_args_t {
	pawn_t* pawn;
} pawn_event_args_t;

typedef struct message_event_args_t {
	char* message;
} message_event_args_t;

typedef struct interaction_event_args_t {
	struct interaction_t* interaction;
} interaction_event_args_t;

typedef struct item_use_event_args_t {
	struct use_t* use;
} item_use_event_args_t;

#endif
