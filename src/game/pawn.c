
#include <stdlib.h>

#include "server.h"
#include "pawn.h"
#include "constants.h"

#define UNUSED(x) (void)(x)

void initPawn(pawn_t* pawn) {
    pawn->exhaustion = 0;
    pawn->place = NULL;
    pawn->eventHandler = noHandler;
    pawn->account = NULL;
    pawn->user = NULL;
}

void dropPawn(pawn_t* pawn) {
	UNUSED(pawn);
}

void sendPawnEvent(server_t* server, pawn_t* pawn, pawn_event_t* event) {
    event->server = server;
    event->pawn = pawn;
    pawn->eventHandler((void*)event);
}
