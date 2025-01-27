
#ifndef CHAMPION_H
#define CHAMPION_H

#include "ability.h"

#define ATTACK_COUNT 3 /** nombre maximum d'attaques par personnage */

/** Structure représentant un personnage jouable. */
typedef struct {
    int intelligenceStat; /** définit dans quel ordre les personnages agissent */
    int attackStat;
    int defenseStat;
    int magicAttackStat;
    int magicDefenseStat;

    ability_t* abilities[ATTACK_COUNT];
} champion_t;

#endif
