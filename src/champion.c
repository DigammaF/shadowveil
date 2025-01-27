
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
 * - Il a en moyenne 10 points par catégorie 
*/
character_t* generateRandomChampion(){
    todo
}