#include "player.h"

#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#define MAX_PLAYERS 2

typedef struct game {
	player_t **players;
	int num_players;
	int winner;
} game_t;


void *game_start(void *);
game_t *game_create(player_t **, int);
void game_process(game_t *);

#endif
