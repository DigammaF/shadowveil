
#include "pawn.h"

void sendPawnEvent(server_t* server, pawn_t* pawn, pawn_event_t* event) {
    event->server = server;
    event->pawn = pawn;
    pawn->eventHandler((void*)event);
}
