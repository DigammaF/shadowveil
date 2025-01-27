
#ifndef CHAMPION_H
#define CHAMPION_H

#include "ability.h"

#define ABILITY_COUNT 3 /** nombre maximum d'attaques par personnage */

typedef enum {
    TYPE_FIRE = 0,
    TYPE_WATER,
    TYPE_GRASS,
    TYPE_ELECTRIC,
    TYPE_ICE,
    TYPE_FIGHTING,
    TYPE_POISON,
    TYPE_GROUND,
    TYPE_FLYING,
    TYPE_PSYCHIC,
    TYPE_BUG,
    TYPE_ROCK,
    TYPE_GHOST,
    TYPE_DRAGON,
    TYPE_DARK,
    TYPE_STEEL,
    TYPE_FAIRY,
    TYPE_NORMAL,
    TYPE_ABNORMAL,
    TYPE_LIGHT,
    TYPE_SHADOW,
    TYPE_WIND,
    TYPE_GRAVITY,
    TYPE_METAL,
    TYPE_VENOM,
    TYPE_MYSTIC,
    TYPE_ARCANE,
    TYPE_MIND,
    TYPE_LAVA,
    TYPE_COSMIC,
    TYPE_LUNAR,
    TYPE_SOLAR,
    TYPE_LIQUID,
    TYPE_VOLCANIC,
    TYPE_ANCIENT,
    TYPE_TURBO,
    TYPE_FLAME,
    TYPE_ECHO,
    TYPE_FROST,
    TYPE_ETHER,
    TYPE_MIRAGE,
    TYPE_FESTERING,
    TYPE_FUSION,
    TYPE_PRIMAL,
    TYPE_CRYSTAL,
    TYPE_RADIANT,
    TYPE_SPIRIT,
    TYPE_TERRIFYING,
    TYPE_LEGENDARY,
    TYPE_WILD,
    TYPE_HARMONY,
    TYPE_FURY,
    TYPE_ELITE,
    TYPE_EMBER,
    TYPE_MIRROR,
    TYPE_BLAST,
    TYPE_CHAOTIC,
    TYPE_BLOOD,
    TYPE_GLIMMER,
    TYPE_RELIC,
    TYPE_RUSH,
    TYPE_WARLOCK,
    TYPE_WITCHCRAFT
} champion_type_t;

#define MAX_TYPE 63

#define EFFECT_FLAME (1 << 0)
#define EFFECT_FREEZE (1 << 1)
#define EFFECT_POISONED (1 << 2)
#define EFFECT_PARALYZE (1 << 3)
#define EFFECT_BURN (1 << 4)
#define EFFECT_SLEEP (1 << 5)
#define EFFECT_CONFUSED (1 << 6)
#define EFFECT_FROSTBITE (1 << 7)
#define EFFECT_PARALYSIS (1 << 8)
#define EFFECT_BLEED (1 << 9)
#define EFFECT_WEAKENED (1 << 10)
#define EFFECT_MYSTIC (1 << 11)
#define EFFECT_ENERGIZED (1 << 12)
#define EFFECT_DEMONIC (1 << 13)
#define EFFECT_CHAOS (1 << 14)
#define EFFECT_STUN (1 << 15)
#define EFFECT_BLIND (1 << 16)
#define EFFECT_CURSE (1 << 17)
#define EFFECT_RAGE (1 << 18)
#define EFFECT_FEAR (1 << 19)
#define EFFECT_IMMUNITY (1 << 20)
#define EFFECT_REGEN (1 << 21)
#define EFFECT_VENOM (1 << 22)
#define EFFECT_RECOIL (1 << 23)
#define EFFECT_SHIELD (1 << 24)
#define EFFECT_RAIN (1 << 25)
#define EFFECT_SUN (1 << 26)
#define EFFECT_GRAVITY (1 << 27)
#define EFFECT_HEAL (1 << 28)
#define EFFECT_STEALTH (1 << 29)
#define EFFECT_DRAIN (1 << 30)
#define EFFECT_ECHO (1 << 31)
#define EFFECT_BARRIER (1 << 32)
#define EFFECT_MIRROR (1 << 33)
#define EFFECT_REFLECT (1 << 34)
#define EFFECT_FORCE (1 << 35)
#define EFFECT_WARP (1 << 36)
#define EFFECT_VACUUM (1 << 37)
#define EFFECT_SILENCE (1 << 38)
#define EFFECT_FREEZE_BARRIER (1 << 39)
#define EFFECT_SPEED_UP (1 << 40)
#define EFFECT_WIND (1 << 41)
#define EFFECT_FUSION (1 << 42)
#define EFFECT_ECLIPSE (1 << 43)
#define EFFECT_DARKNESS (1 << 44)
#define EFFECT_REVERSE (1 << 45)
#define EFFECT_BLOODLUST (1 << 46)
#define EFFECT_METAL (1 << 47)
#define EFFECT_LIGHT (1 << 48)
#define EFFECT_MIND_CONTROL (1 << 49)
#define EFFECT_UNHOLY (1 << 50)
#define EFFECT_DEBUFF (1 << 51)
#define EFFECT_MAGNETIC (1 << 52)
#define EFFECT_CORROSION (1 << 53)
#define EFFECT_HYPER_MODE (1 << 54)
#define EFFECT_GALE (1 << 55)
#define EFFECT_REVERSE_GRAVITY (1 << 56)
#define EFFECT_MOMENTUM (1 << 57)
#define EFFECT_CRITICAL (1 << 58)
#define EFFECT_TRANSFORM (1 << 59)
#define EFFECT_JUMP (1 << 60)
#define EFFECT_LIGHTNING (1 << 61)
#define EFFECT_SHOCK (1 << 62)
#define EFFECT_GROWTH (1 << 63)

#define MAX_EFFECT 63

typedef struct {
    int value;
    int maxValue;
} stat_t;

/** Structure représentant un personnage jouable. */
typedef struct {
    champion_type_t type;
    unsigned long long effects;

    stat_t intelligence; /** définit dans quel ordre les personnages agissent */
    stat_t attack;
    stat_t defense;
    stat_t magicAttack;
    stat_t magicDefense;
    stat_t health;

    ability_t* abilities[ABILITY_COUNT];
} champion_t;

#endif
