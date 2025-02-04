
#include <stdlib.h>
#include <stdio.h>

#include "lantern.h"
#include "event_handlers.h"
#include "pawn.h"
#include "user.h"
#include "account.h"

#define UNUSED(x) (void)(x)

/**
 * 
 * 	Gestionnaire d'évènements associé à un pawn controlé par un joueur
 *  En particulier, ce pawn est lié à un account
 * 
 */
void* playerEventHandler(void* _) {
	event_t* event = (event_t*)_;
	pawn_t* pawn = event->pawn;
	user_t* user = pawn->user;

	if (user == NULL) { return NULL; }

	char message[1024];
	pawn_event_args* pawnEventArgs = NULL;
	message_event_args* message_event_args = NULL;

	switch (event->type) {
		case EVENT_PAWN_ARRIVED:
			pawnEventArgs = event->args;
			sprintf(message, "PAWN-ARRIVED %i %s", pawnEventArgs->pawn->placeKey, pawnEventArgs->pawn->name);
			sendData(&user->socket, message);
			break;

		case EVENT_PAWN_LEFT:
			pawnEventArgs = event->args;
			sprintf(message, "PAWN-LEFT %s", pawnEventArgs->pawn->name);
			sendData(&user->socket, message);
			break;

		case EVENT_PAWN_SPAWNED:
			pawnEventArgs = event->args;
			sprintf(message, "PAWN-SPAWN %i %s", pawnEventArgs->pawn->placeKey, pawnEventArgs->pawn->name);
			sendData(&user->socket, message);
			break;

		case EVENT_MESSAGE:
			message_event_args = event->args;
			sprintf(message, "MESSAGE %s", message_event_args->message);
			sendData(&user->socket, message);
			break;
	}

	return NULL;
}
