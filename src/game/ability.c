
#include <stdlib.h>

#include "ability.h"

#define UNUSED(x) (void)(x)

void initAbility(ability_t* ability) {
	ability->champion = NULL;
}

void dropAbility(ability_t* ability) { UNUSED(ability); }
