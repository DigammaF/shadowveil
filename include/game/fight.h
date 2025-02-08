
#ifndef FIGHT_H
#define FIGHT_H

#include <stdbool.h>

#include "hashmap.h"

struct champion_t;
struct pawn_t;

typedef struct fight_t {
	hashmap_t champions; // champion_t*
	hashmap_t pawns; // pawn_t*
	unsigned key;
} fight_t;

void initFight(fight_t* fight);
void dropFight(fight_t* fight);

void addChampionToFight(fight_t* fight, struct champion_t* champion);
void removeChampionFromFight(fight_t* fight, struct champion_t* champion);

void addPawnToFight(fight_t* fight, struct pawn_t* pawn);
void removePawnFromFight(fight_t* fight, struct pawn_t* pawn);

bool turnEnded(fight_t* fight);
void makeFight(fight_t* fight, struct pawn_t* pawnA, struct pawn_t* pawnB);
bool fightContainsChampion(fight_t* fight, struct champion_t* champion);
bool pawnCanOrderChampion(struct pawn_t* pawn, struct champion_t* champion);
void disband(fight_t* fight);

#endif
