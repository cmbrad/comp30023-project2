/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdint.h>
#include "move.h"
#include "status.h"
#include "connect4.h"

#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

typedef struct player {
	int soc_id;    // Socket player is connectd to
	char *ip;      // IP player is connecting from
	char colour;
	move_t (*get_move)(int, c4_t);
	void (*notify_move)(int, char, move_t);
	void (*notify_status)(int, status_t);
} player_t;

player_t *player_create(int, char*);

#endif
