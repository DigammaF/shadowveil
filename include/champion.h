
#ifndef CHAMPION_H
#define CHAMPION_H

#include "ability.h"
#include "constants.h"

#define ABILITY_COUNT 3 /** nombre maximum d'attaques par personnage */

typedef struct {
    int value;
    int maxValue;
} stats_t;

typedef enum STAT { 
    ATTACK, 
    DEFENSE, 
    MAGIC_ATTACK, 
    MAGIC_DEFENSE, 
    INTELLIGENCE, /** définit dans quel ordre les personnages agissent */
    HEALTH, 
    STAT_COUNT /** STAT_COUNT will be the number of elements stored in this enum, not counting itself */
};

/** Structure représentant un personnage jouable. */
typedef struct {
    champion_type_t type;
    unsigned long long effects;

    stats_t* stats[STAT_COUNT]; /** dictionnaire qui prends des STAT en clés et a des stats_t comme valeurs */

    ability_t* abilities[ABILITY_COUNT];
} champion_t;


#endif
