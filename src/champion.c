
#include "champion.h"
#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

#define AVERAGE_STARTING_POINTS 5

/** Crée des personnages définis. */
int setupExamples() {

    character_t character1 = {
        .intelligenceStat = 5,
        .attackStat = 9,
        .defenseStat = 8,
        .magicAttackStat = 2,
        .magicDefenseStat = 3,
        .abilities = {NULL, NULL, NULL} // à compléter par des attaques spécifiques
    };


    character_t character2 = {
        .intelligenceStat = 9,
        .attackStat = 3,
        .defenseStat = 4,
        .magicAttackStat = 9,
        .magicDefenseStat = 8,
        .abilities = {NULL, NULL, NULL}
    };

    character_t character3 = {
        .intelligenceStat = 70,
        .attackStat = 75,
        .defenseStat = 50,
        .magicAttackStat = 40,
        .magicDefenseStat = 45,
        .abilities = {NULL, NULL, NULL}
    };

    printf("setupExamples : Trois personnages créés.\n");
    return 0;
}

/** Renvoie un pointeur vers un character_t généré procéduralement.
 * - Il possède une attaque choisie au hasard parmi la pool. 
 * - Il a au minimum 1 point par catégorie, et les persos ont en moyenne 5 pts par stat.
*/
champion_t* generateRandomChampion(){

    champion_t* champion = malloc( sizeof(champion_t) );

    champion->type = rand()% TYPE_MAX;

    champion->effects = 0; //pas d'effets => 0 partout

    for ( int i=0; i< STAT_COUNT ; i++ ){
        ( champion->stats )[randomStat] = 1; // init des stats à 1
    }
    for ( int pointsLeft = ( STAT_COUNT * AVERAGE_STARTING_POINTS ) ; pointsLeft>0 ; pointsLeft-- ){ //décrémenter le stock de points jusqu'à ce qu'il n'y en ait plus à distribuer
        STAT randomStat = (rand() % STAT_COUNT); //choisir l'une des stat au hasard
        ( ( champion->stats )[randomStat] ) ++ ; //et l'incrémenter
    }

    champion->abilities = {NULL, NULL, NULL}; //todo abilities;


    return champion;
}

void setStat(stat_t* stat, int value) { todo debuger
    if (value < stat->minValue) { stat->value = stat->minValue; }
    if (value > stat->maxValue) { stat->value = stat->maxValue; }
    stat->value = value;
}


1 point partout, 9*STAT_COUNT points à distribuer
1+(quantité de 0 à 9 inclus)