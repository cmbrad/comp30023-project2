#include <stdint.h>
#include "move.h"
#include "connect4.h"

#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

typedef struct player {
	int soc_id;    // Socket player is connectd to
	char *ip;      // IP player is connecting from
	char colour;
	move_t (*get_move)(int soc_id, c4_t board);
	void (*notify_move)(int soc_id, char player_colour, move_t move);
} player_t;

player_t *player_create(int, char*);

#endif
