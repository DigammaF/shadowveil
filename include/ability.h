
#ifndef ABILITY_H
#define ABILITY_H

#define MAX_ABILITY_DATA 10

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
 * 
*/

#define SET_TARGET 0
#define SET_HEALTH 1
#define ADD_EFFECT 2
#define IF_EFFECT 3
#define IF_TYPE 4

typedef struct {
    unsigned type;
    unsigned data[MAX_ABILITY_DATA];
    int monoTarget;
} ability_t;

void applyAbility(champion_t* source, chamption_t* destination, ability_t* ability);

#endif
