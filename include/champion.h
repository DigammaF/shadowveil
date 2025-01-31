
#ifndef CHAMPION_H
#define CHAMPION_H

#include <stdint.h>

#include "ability.h"
#include "constants.h"

#define MAKE_STAT(n) (stat_value_t){ .value = 0, .maxValue = (n), .minValue = 0 }
#define MAKE_STATS(atk, dfe, matk, mdfe, intel, health) \
    ((stat_value_t[]) { \
        MAKE_STAT(atk), \
        MAKE_STAT(dfe), \
        MAKE_STAT(matk), \
        MAKE_STAT(mdfe), \
        MAKE_STAT(intel), \
        MAKE_STAT(health) \
    })

#define ABILITY_COUNT 3 /** nombre maximum d'attaques par personnage */

typedef struct {
    int value;
    int maxValue;
    int minValue;
} stat_value_t;

typedef enum { 
    ATTACK,
    DEFENSE,
    MAGIC_ATTACK,
    MAGIC_DEFENSE, 
    INTELLIGENCE, /** définit dans quel ordre les personnages agissent */
    HEALTH, 
    STAT_COUNT /** STAT_COUNT will be the number of elements stored in this enum, not counting itself */
} stat_t;

/** Structure représentant un personnage jouable. */
typedef struct {
    champion_type_t type;
    int effects[EFFECT_COUNT];
    stat_value_t stats[STAT_COUNT]; /** dictionnaire qui prends des STAT en clés et a des stats_t comme valeurs */
    ability_t* abilities[ABILITY_COUNT];
} champion_t;

void setStat(stat_value_t* stat, int value);

void generateChampion(unsigned seed, unsigned powerBudget, champion_t* champion);

#endif
