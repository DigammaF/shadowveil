
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "combat_command_handler.h"
#include "server.h"
#include "user.h"
#include "fight.h"
#include "pawn.h"
#include "account.h"
#include "ability.h"
#include "champion.h"
#include "string_utils.h"

void handleCombatAttack(command_context_t* context) {
	unsigned sourceChampionKey;
	unsigned destinationChampionKey;
	unsigned abilityKey;

	if (!safeStrToUnsigned(context->args[2], &sourceChampionKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[2]); return; }
	if (!safeStrToUnsigned(context->args[3], &destinationChampionKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[3]); return; }
	if (!safeStrToUnsigned(context->args[4], &abilityKey)) { fprintf(stderr, "(!) Failed to convert '%s' to an unsigned\n", context->args[4]); return; }

	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	fight_t* fight = pawn->fight;

	champion_t* sourceChampion = hashmapGet(&fight->champions, sourceChampionKey);
	champion_t* destinationChampion = hashmapGet(&fight->champions, destinationChampionKey);

	if (sourceChampion == NULL) { fprintf(stderr, "(!) could not fetch champion %i\n", sourceChampionKey); return; }
	if (destinationChampion == NULL) { fprintf(stderr, "(!) could not fetch champion %i\n", destinationChampionKey); return; }

	if (!pawnCanOrderChampion(pawn, sourceChampion)) {
		fprintf(stderr, "(!) can't order champion\n"); return;
	}

	if (isDead(sourceChampion)) {
		fprintf(stderr, "(!) champion is dead\n"); return;
	}

	ability_t* ability = hashmapGet(&sourceChampion->abilities, abilityKey);
	applyAbility(sourceChampion, destinationChampion, ability);
}

void handleRunaway(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	world_t* world = &context->server->world;
	fight_t* fight = pawn->fight;

	for (unsigned n = 0; n < fight->pawns.capacity; n++) {
		pawn_t* pawn = fight->pawns.elements[n];
		if (pawn == NULL) { continue; }
		user_t* localUser = pawn->user;

		if (localUser != NULL) {
			popFunction(&localUser->commandHandlers);
			popUserContex(localUser);
		}
	}

	disband(fight);
	removeFightFromWorld(world, fight);
	dropFight(fight);
	free(fight);
}

void handleCombatSee(command_context_t* context) {
	user_t* user = context->user;
	account_t* account = user->account;
	pawn_t* pawn = account->pawn;
	fight_t* fight = pawn->fight;
	char message[1024];

	for (unsigned n = 0; n < fight->champions.capacity; n++) {
		champion_t* champion = fight->champions.elements[n];
		if (champion == NULL) { continue; }
		sprintf(message, "LIST-CHAMPION-DETAIL %i %s %i %i", n, champion->name, champion->stats[HEALTH].value, champion->stats[HEALTH].maxValue);
		sendData(&user->socket, message);
	}

	sendData(&user->socket, "END-LIST");
}

void* combatCommandHandler(void* arg) {
	command_context_t* context = (command_context_t*)arg;

	if (context->count == 2) {
		if (strcmp(context->args[1], "RUNAWAY") == 0) {
			handleRunaway(context);
			return NULL;
		}

		if (strcmp(context->args[1], "SEE") == 0) {
			handleCombatSee(context);
			return NULL;
		}
	}

	if (context->count == 5) {
		if (strcmp(context->args[1], "ATTACK") == 0) {
			handleCombatAttack(context);
			return NULL;
		}
	}

	printf("(!) unable to parse command\n");
	return NULL;
}
