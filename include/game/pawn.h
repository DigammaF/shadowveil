
#ifndef PAWN_H
#define PAWN_H

#include "function_stack.h"
#include "server.h"
#include "account.h"
#include "user.h"
#include "place.h"

typedef struct {
    char* name;
    unsigned exhaustion;
    place_t* place;
    function_t eventHandler; // void* eventHandler(pawn_event_t*)
    account_t* account; // possiblement NULL si entité non joueur
    user_t* user; // possiblement NULL si le joueur n'est pas connecté
} pawn_t;

typedef enum {
    PAWN_ARRIVED,
    PAWN_LEFT,
} pawn_event_type_t;

typedef struct {
    pawn_event_type_t type;
    server_t* server;
    pawn_t* pawn;
    void* args;
} pawn_event_t;

void initPawn(pawn_t* pawn);
void dropPawn(pawn_t* pawn);

/**
 * 
 *  Envoie un évènement à l'entité
 *  il suffit de renseigner event.type et event.args
 * 
 */
void sendPawnEvent(server_t* server, pawn_t* pawn, pawn_event_t* event);

#endif
