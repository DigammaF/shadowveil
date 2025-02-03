
#ifndef PAWN_H
#define PAWN_H

#include "function_stack.h"

struct account_t;
struct user_t;
struct server_t;
struct place_t;

typedef struct pawn_t {
    char* name;
    unsigned exhaustion;
    struct place_t* place;
    function_t eventHandler; // void* eventHandler(pawn_event_t*)
    struct account_t* account; // possiblement NULL si entité non joueur
    struct user_t* user; // possiblement NULL si le joueur n'est pas connecté
	unsigned pawnKey; // la clé de l'entité dans le dictionnaire .pawns de la place_t dans laquelle elle se situe
} pawn_t;

typedef enum {
    PAWN_ARRIVED,
    PAWN_LEFT,
} pawn_event_type_t;

typedef struct {
    pawn_event_type_t type;
    struct server_t* server;
    pawn_t* pawn;
    void* args;
} pawn_event_t;

#define MAKE_EVENT(t, a) (pawn_event_t) { .type = t, .server = NULL, .pawn = NULL, .args = a }

void initPawn(pawn_t* pawn);
void dropPawn(pawn_t* pawn);

/**
 * 
 *  Envoie un évènement à l'entité
 *  il suffit de renseigner event.type et event.args
 * 
 */
void sendPawnEvent(struct server_t* server, pawn_t* pawn, pawn_event_t* event);

typedef struct {
	pawn_t* pawn;
} pawn_move_args_t;

#endif
