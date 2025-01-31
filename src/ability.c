
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
