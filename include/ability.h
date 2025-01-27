
#ifndef ABILITY_H
#define ABILITY_H

#include "champion.h"

#define MAX_ABILITY_OP_CODES 10

/**
 * 
 * Z: entiers relatifs
 * N: entiers positifs
 * 
 * SET-TARGET 0|1 choisit la cible entre source et destination
 * SET-HEALTH Z change les points de vie de la cible
 * ADD-EFFECT N ajoute un effet à la cible
 * IF-EFFECT N si la cible n'a pas cet effet, arrêter
 * IF-TYPE N si la cible n'est pas de ce type, arrêter
 * NOOP pas d'action
 * 
*/

#define SET_TARGET 0
#define SET_HEALTH 1
#define ADD_EFFECT 2
#define IF_EFFECT 3
#define IF_TYPE 4
#define NOOP 5

typedef struct {
    unsigned type;
    unsigned opCodes[MAX_ABILITY_OP_CODES];
    int monoTarget;
} ability_t;

void applyAbility(champion_t* source, champion_t* destination, ability_t* ability);

#endif
