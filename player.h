#include <stdint.h>
#include "move.h"

#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

typedef struct player {
	int soc_id;    // Socket player is connectd to
	char *ip;      // IP player is connecting from
	move_t (*get_move)(int soc_id);
} player_t;

player_t *player_create(int, char*);

#endif
