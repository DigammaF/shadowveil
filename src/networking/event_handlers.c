
#include <stdlib.h>
#include <stdio.h>

#include "lantern.h"
#include "event_handlers.h"
#include "pawn.h"
#include "user.h"
#include "account.h"
#include "champion.h"

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
	pawn_event_args_t* pawnEventArgs = NULL;
	message_event_args_t* messageEventArgs = NULL;
	gold_changed_event_args_t* goldChangedEventArgs = NULL;
	champion_event_args_t* championEventArgs = NULL;

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
			messageEventArgs = event->args;
			sprintf(message, "MESSAGE %s", messageEventArgs->message);
			sendData(&user->socket, message);
			break;

		case EVENT_CHAMPION_GAINED:
			championEventArgs = event->args;
			sprintf(message, "GAINED-CHAMPION %i %s %s", championEventArgs->champion->pawnKey, championEventArgs->champion->name, championEventArgs->reason);
			sendData(&user->socket, message);
			break;

		case EVENT_CHAMPION_LOST:
			championEventArgs = event->args;
			sprintf(message, "LOST-CHAMPION %s %s", championEventArgs->champion->name, championEventArgs->reason);
			sendData(&user->socket, message);
			break;

		case EVENT_GOLD_CHANGED:
			goldChangedEventArgs = event->args;
			sprintf(message, "YOUR-GOLD-CHANGED %s %s", goldChangedEventArgs->delta, goldChangedEventArgs->reason);
			sendData(&user->socket, message);
			break;

		default:
			return NULL;
	}

	return NULL;
}
