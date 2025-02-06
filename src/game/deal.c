
#include <stdlib.h>

#include "deal.h"

#define UNUSED(x) (void)(x)

void initChampionDeal(champion_deal_t* championDeal) {
	championDeal->champion = NULL;
	championDeal->seller = NULL;
}

void dropChampionDeal(champion_deal_t* championDeal) { UNUSED(championDeal); }

void initItemDeal(item_deal_t* itemDeal) {
	itemDeal->item = NULL;
	itemDeal->seller = NULL;
}

void dropItemDeal(item_deal_t* itemDeal) { UNUSED(itemDeal); }
