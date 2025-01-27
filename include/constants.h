
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#define SERVER_PORT 58420

typedef enum {
	INITIAL, ADMIN, GAMEWORLD, COMBAT
} context_t;

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
	TYPE_WITCHCRAFT,
	TYPE_MAX
} champion_type_t;

#define EFFECT_FLAME ((uint64_t)1 << 0)
#define EFFECT_FREEZE ((uint64_t)1 << 1)
#define EFFECT_POISONED ((uint64_t)1 << 2)
#define EFFECT_PARALYZE ((uint64_t)1 << 3)
#define EFFECT_BURN ((uint64_t)1 << 4)
#define EFFECT_SLEEP ((uint64_t)1 << 5)
#define EFFECT_CONFUSED ((uint64_t)1 << 6)
#define EFFECT_FROSTBITE ((uint64_t)1 << 7)
#define EFFECT_PARALYSIS ((uint64_t)1 << 8)
#define EFFECT_BLEED ((uint64_t)1 << 9)
#define EFFECT_WEAKENED ((uint64_t)1 << 10)
#define EFFECT_MYSTIC ((uint64_t)1 << 11)
#define EFFECT_ENERGIZED ((uint64_t)1 << 12)
#define EFFECT_DEMONIC ((uint64_t)1 << 13)
#define EFFECT_CHAOS ((uint64_t)1 << 14)
#define EFFECT_STUN ((uint64_t)1 << 15)
#define EFFECT_BLIND ((uint64_t)1 << 16)
#define EFFECT_CURSE ((uint64_t)1 << 17)
#define EFFECT_RAGE ((uint64_t)1 << 18)
#define EFFECT_FEAR ((uint64_t)1 << 19)
#define EFFECT_IMMUNITY ((uint64_t)1 << 20)
#define EFFECT_REGEN ((uint64_t)1 << 21)
#define EFFECT_VENOM ((uint64_t)1 << 22)
#define EFFECT_RECOIL ((uint64_t)1 << 23)
#define EFFECT_SHIELD ((uint64_t)1 << 24)
#define EFFECT_RAIN ((uint64_t)1 << 25)
#define EFFECT_SUN ((uint64_t)1 << 26)
#define EFFECT_GRAVITY ((uint64_t)1 << 27)
#define EFFECT_HEAL ((uint64_t)1 << 28)
#define EFFECT_STEALTH ((uint64_t)1 << 29)
#define EFFECT_DRAIN ((uint64_t)1 << 30)
#define EFFECT_ECHO ((uint64_t)1 << 31)
#define EFFECT_BARRIER ((uint64_t)1 << 32)
#define EFFECT_MIRROR ((uint64_t)1 << 33)
#define EFFECT_REFLECT ((uint64_t)1 << 34)
#define EFFECT_FORCE ((uint64_t)1 << 35)
#define EFFECT_WARP ((uint64_t)1 << 36)
#define EFFECT_VACUUM ((uint64_t)1 << 37)
#define EFFECT_SILENCE ((uint64_t)1 << 38)
#define EFFECT_FREEZE_BARRIER ((uint64_t)1 << 39)
#define EFFECT_SPEED_UP ((uint64_t)1 << 40)
#define EFFECT_WIND ((uint64_t)1 << 41)
#define EFFECT_FUSION ((uint64_t)1 << 42)
#define EFFECT_ECLIPSE ((uint64_t)1 << 43)
#define EFFECT_DARKNESS ((uint64_t)1 << 44)
#define EFFECT_REVERSE ((uint64_t)1 << 45)
#define EFFECT_BLOODLUST ((uint64_t)1 << 46)
#define EFFECT_METAL ((uint64_t)1 << 47)
#define EFFECT_LIGHT ((uint64_t)1 << 48)
#define EFFECT_MIND_CONTROL ((uint64_t)1 << 49)
#define EFFECT_UNHOLY ((uint64_t)1 << 50)
#define EFFECT_DEBUFF ((uint64_t)1 << 51)
#define EFFECT_MAGNETIC ((uint64_t)1 << 52)
#define EFFECT_CORROSION ((uint64_t)1 << 53)
#define EFFECT_HYPER_MODE ((uint64_t)1 << 54)
#define EFFECT_GALE ((uint64_t)1 << 55)
#define EFFECT_REVERSE_GRAVITY ((uint64_t)1 << 56)
#define EFFECT_MOMENTUM ((uint64_t)1 << 57)
#define EFFECT_CRITICAL ((uint64_t)1 << 58)
#define EFFECT_TRANSFORM ((uint64_t)1 << 59)
#define EFFECT_JUMP ((uint64_t)1 << 60)
#define EFFECT_LIGHTNING ((uint64_t)1 << 61)
#define EFFECT_SHOCK ((uint64_t)1 << 62)
#define EFFECT_GROWTH ((uint64_t)1 << 63)

extern uint64_t EFFECTS[];

#endif
