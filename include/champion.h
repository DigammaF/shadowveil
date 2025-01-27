
#ifndef CHAMPION_H
#define CHAMPION_H

#define NB_OF_ATTACKS 3 /** nombre maximum d'attaques par personnage */


/** Structure représentant un personnage jouable. */
typedef struct {
    int intelligenceStat; /** définit dans quel ordre les personnages agissent */
    int attackStat;
    int defenseStat;
    int magicAttackStat;
    int magicDefenseStat;

    abilities_t abilities[NB_OF_ATTACKS]; /* pointeurs vers les attaques */

    //ajouts futurs potentiels : arme, armure, niveaux
} champion_t;


#endif
