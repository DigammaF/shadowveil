#define NB_OF_ATTACKS 3 /** NB_OF_ATTACKS for a character */

/** Structure représentant un personnage jouable. */
typedef struct {
    int intelligenceStat;
    int attackStat;
    int 
    abilities_t abilities[NB_OF_ATTACKS]; /* attacks */

} character_t;