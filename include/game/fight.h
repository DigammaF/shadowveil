
#ifndef FIGHT_H
#define FIGHT_H

#include <stdbool.h>

#include "hashmap.h"

struct champion_t;
struct pawn_t;

/** représente un combat en cours */
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

/** vérifie si le tour est terminé */
bool turnEnded(fight_t* fight);
/** ajoute les pions et champions au combat, et initialise les donnés relatives à la gestion de flux du tour */
void makeFight(fight_t* fight, struct pawn_t* pawnA, struct pawn_t* pawnB);
/** vérifie si le champion est concerné par le combat */
bool fightContainsChampion(fight_t* fight, struct champion_t* champion);
/** vérifie si le pion est autorisé à commander le champion */
bool pawnCanOrderChampion(struct pawn_t* pawn, struct champion_t* champion);
/** sort les pions et les champions du combat */
void disband(fight_t* fight);
/** TODO donne l'initiative au bon champion */
void giveInitiative(fight_t* fight);

#endif
