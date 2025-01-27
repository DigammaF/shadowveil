
#include <stdlib.h>

#include "constants.h"
#include "ability.h"
#include "champion.h"

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
                break;

            case SET_HEALTH:
                target->health.value += arg;
                break;

            case ADD_EFFECT:
                target->effects |= EFFECTS[arg];
        }
    }
}
