
#ifndef CHAMPION_H
#define CHAMPION_H

#include <stdint.h>

#include "ability.h"
#include "constants.h"

#define ABILITY_COUNT 3 /** nombre maximum d'attaques par personnage */

typedef struct {
    int value;
    int maxValue;
    int minValue;
} stat_t;

/** Structure représentant un personnage jouable. */
typedef struct {
    champion_type_t type;
    uint64_t effects;

    stat_t intelligence; /** définit dans quel ordre les personnages agissent */
    stat_t attack;
    stat_t defense;
    stat_t magicAttack;
    stat_t magicDefense;
    stat_t health;

    ability_t* abilities[ABILITY_COUNT];
} champion_t;

void setStat(stat_t* stat, int value);

#endif
