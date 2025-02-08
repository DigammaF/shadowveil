
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SERVER_PORT (short)20100
#define SERVER_TICK 500 // ms

#define WORLD_SEED 555

void* noHandler(void* arg);

typedef enum {
	INITIAL, ADMIN, GAMEWORLD, COMBAT
} context_t;

typedef enum {
	TYPE_WATER = 0,
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
	TYPE_COUNT
} champion_type_t;

typedef enum {
    EFFECT_FLAME,
    EFFECT_FREEZE,
    EFFECT_POISONED,
    EFFECT_PARALYZE,
    EFFECT_BURN,
    EFFECT_SLEEP,
    EFFECT_CONFUSED,
    EFFECT_FROSTBITE,
    EFFECT_PARALYSIS,
    EFFECT_BLEED,
    EFFECT_WEAKENED,
    EFFECT_MYSTIC,
    EFFECT_ENERGIZED,
    EFFECT_DEMONIC,
    EFFECT_CHAOS,
    EFFECT_STUN,
    EFFECT_BLIND,
    EFFECT_CURSE,
    EFFECT_RAGE,
    EFFECT_FEAR,
    EFFECT_IMMUNITY,
    EFFECT_REGEN,
    EFFECT_VENOM,
    EFFECT_RECOIL,
    EFFECT_SHIELD,
    EFFECT_RAIN,
    EFFECT_SUN,
    EFFECT_GRAVITY,
    EFFECT_HEAL,
    EFFECT_STEALTH,
    EFFECT_DRAIN,
    EFFECT_ECHO,
    EFFECT_BARRIER,
    EFFECT_MIRROR,
    EFFECT_REFLECT,
    EFFECT_FORCE,
    EFFECT_WARP,
    EFFECT_VACUUM,
    EFFECT_SILENCE,
    EFFECT_FREEZE_BARRIER,
    EFFECT_SPEED_UP,
    EFFECT_WIND,
    EFFECT_FUSION,
    EFFECT_ECLIPSE,
    EFFECT_DARKNESS,
    EFFECT_REVERSE,
    EFFECT_BLOODLUST,
    EFFECT_METAL,
    EFFECT_LIGHT,
    EFFECT_MIND_CONTROL,
    EFFECT_UNHOLY,
    EFFECT_DEBUFF,
    EFFECT_MAGNETIC,
    EFFECT_CORROSION,
    EFFECT_HYPER_MODE,
    EFFECT_GALE,
    EFFECT_REVERSE_GRAVITY,
    EFFECT_MOMENTUM,
    EFFECT_CRITICAL,
    EFFECT_TRANSFORM,
    EFFECT_JUMP,
    EFFECT_LIGHTNING,
    EFFECT_SHOCK,
    EFFECT_GROWTH,
	EFFECT_COUNT,
} effect_type_t;

#endif
