
#include <stdlib.h>
#include <stdio.h>

#include "champion.h"

#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

#define AVERAGE_STARTING_POINTS 5

/** Crée des personnages définis. */
int setupExamples() {
    champion_t character1 = {
        .type = TYPE_COSMIC,
		.effects = { 0 },
		.stats = MAKE_STATS(5, 6, 5, 3, 8, 10),
		.abilities = { NULL }
    };


    champion_t character2 = {
        .type = TYPE_BLOOD,
		.effects = { 0 },
		.stats = MAKE_STATS(7, 1, 9, 4, 5, 21),
		.abilities = { NULL }
    };

    champion_t character3 = {
        .type = TYPE_DARK,
		.effects = { 0 },
		.stats = MAKE_STATS(7, 4, 3, 8, 8, 11),
		.abilities = { NULL }
    };

    printf("setupExamples : Trois personnages créés.\n");
    return 0;
}

int getRandomInt(int min, int max) { return min + rand() % (max - min); }

int compare(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

void generateChampion(unsigned seed, unsigned powerBudget, champion_t* champion) {
	if (powerBudget < STAT_COUNT) { fprintf(stderr, "generateChampion: powerBudget lower than stat count"); exit(EXIT_FAILURE); }
	srand(seed);
	unsigned budgets[STAT_COUNT - 1];

	for (unsigned n = 0; n < STAT_COUNT - 1; n++) {
		budgets[n] = getRandomInt(0, powerBudget);
	}

	qsort(budgets, STAT_COUNT - 1, sizeof(unsigned), compare);
	champion->stats[0] = MAKE_STAT(budgets[0]);

	for (unsigned n = 1; n < STAT_COUNT - 1; n++) {
		champion->stats[n] = MAKE_STAT(budgets[n] - budgets[n - 1]);
	}

	champion->stats[STAT_COUNT - 1] = MAKE_STAT(powerBudget - budgets[STAT_COUNT - 2]);
}

void setStat(stat_value_t* stat, int value) {
    if (value < stat->minValue) { stat->value = stat->minValue; return; }
    if (value > stat->maxValue) { stat->value = stat->maxValue; return; }
    stat->value = value;
}
