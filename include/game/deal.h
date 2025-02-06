
#ifndef DEAL_H
#define DEAL_H

struct champion_t;
struct item_t;
struct pawn_t;

typedef struct champion_deal_t {
	struct champion_t* champion;
	unsigned price;
	struct pawn_t* seller;
} champion_deal_t;

typedef struct item_deal_t {
	struct item_t* item;
	unsigned price;
	struct pawn_t* seller;
} item_deal_t;

#endif
