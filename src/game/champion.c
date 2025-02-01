
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "ability.h"
#include "champion.h"
#include "random_utils.h"

#define UNUSED(x) (void)(x)
#define CHECK(status, message) { if ((status) == -1) { perror(message); exit(EXIT_FAILURE); } }
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initChampion(champion_t* champion) {
	champion->type = 0;
	for (unsigned n = 0; n < EFFECT_COUNT; n++) { champion->effects[n] = 0; }
	for (unsigned n = 0; n < STAT_COUNT; n++) { champion->stats[n] = MAKE_STAT(0); }
	for (unsigned n = 0; n < ABILITY_COUNT; n++) { champion->abilities[n] = NULL; }
}

void dropChampion(champion_t* champion) {
	for (unsigned n = 0; n < ABILITY_COUNT; n++) {
		if (champion->abilities[n] == NULL) { continue; }
		dropAbility(champion->abilities[n]);
		free(champion->abilities[n]);
		champion->abilities[n] = NULL;
	}
}

/** Crée des personnages définis. */
int setupExamples() {
    champion_t character1 = {
        .type = TYPE_COSMIC,
		.effects = { 0 },
		.stats = { MAKE_STATS(5, 6, 5, 3, 8, 10) },
		.abilities = { NULL }
    };


    champion_t character2 = {
        .type = TYPE_BLOOD,
		.effects = { 0 },
		.stats = { MAKE_STATS(7, 1, 9, 4, 5, 21) },
		.abilities = { NULL }
    };

    champion_t character3 = {
        .type = TYPE_DARK,
		.effects = { 0 },
		.stats = { MAKE_STATS(7, 4, 3, 8, 8, 11) },
		.abilities = { NULL }
    };

	UNUSED(character1); UNUSED(character2); UNUSED(character3);
    printf("setupExamples : Trois personnages créés.\n");
    return 0;
}

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

void applyAbility(champion_t* source, champion_t* destination, ability_t* ability) {
    unsigned reader = 0;
    champion_t* target = destination;

    while (reader < MAX_ABILITY_OP_CODES) {
        unsigned opCode = ability->opCodes[reader];
        unsigned arg = 0;
        if (reader + 1 < MAX_ABILITY_OP_CODES) { arg = ability->opCodes[reader + 1]; }

        switch (opCode) {
            case NOOP:
                reader++;
                break;

            case SET_TARGET:
                if (arg == 0) { target = source; }
                if (arg == 1) { target = destination; }
                reader += 2;
                break;

            case SET_HEALTH:
                setStat(&target->stats[HEALTH], target->stats[HEALTH].value + arg);
                reader += 2;
                break;

            case ADD_EFFECT:
                target->effects[arg] = 1;
                reader += 2;
                break;

            case REM_EFFECT:
                target->effects[arg] = 0;
                reader += 2;
                break;

            case IF_EFFECT:
                if (!(target->effects[arg])) { return; }
                reader++;
                break;

            case IF_TYPE:
                if (target->type != arg) { return; }
                reader++;
                break;
        }
    }
}
