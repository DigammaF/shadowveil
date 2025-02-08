
#include <stdbool.h>

#include "hashmap.h"
#include "fight.h"
#include "pawn.h"
#include "champion.h"

void initFight(fight_t* fight) {
	initHashmap(&fight->pawns);
	initHashmap(&fight->champions);
}

void dropFight(fight_t* fight) {
	dropHashmap(&fight->pawns);
	dropHashmap(&fight->champions);
}

void addChampionToFight(fight_t* fight, struct champion_t* champion) {
	champion->fightKey = hashmapLocateUnusedKey(&fight->champions);
	hashmapSet(&fight->champions, champion->fightKey, champion);
}

void removeChampionFromFight(fight_t* fight, struct champion_t* champion) {
	hashmapSet(&fight->champions, champion->fightKey, NULL);
}

void addPawnToFight(fight_t* fight, struct pawn_t* pawn) {
	pawn->fightKey = hashmapLocateUnusedKey(&fight->pawns);
	hashmapSet(&fight->pawns, pawn->fightKey, pawn);
}

void removePawnFromFight(fight_t* fight, struct pawn_t* pawn) {
	hashmapSet(&fight->pawns, pawn->fightKey, NULL);
}

bool turnEnded(fight_t* fight) {
	for (unsigned n = 0; n < fight->champions.capacity; n++) {
		champion_t* champion = fight->champions.elements[n];
		if (champion == NULL) { continue; }
		if (!champion->playedTurn) { return false; }
	}

	return true;
}

void makeFight(fight_t* fight, pawn_t* pawnA, pawn_t* pawnB) {
	pawnA->fighting = true;
	pawnB->fighting = true;
	addPawnToFight(fight, pawnA);
	addPawnToFight(fight, pawnB);

	for (unsigned n = 0; n < TEAM_SIZE; n++) {
		champion_t* championA = pawnA->team[n];
		champion_t* championB = pawnB->team[n];
		if (championA != NULL) { addChampionToFight(fight, championA); }
		if (championB != NULL) { addChampionToFight(fight, championB); }
	}
}
