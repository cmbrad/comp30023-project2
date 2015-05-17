#include "player.h"
#include "connect4.h"

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

#define AI_IP "0.0.0.0"

move_t human_move(int soc_id, c4_t board);
move_t ai_move(int soc_id, c4_t board);

void ai_notify(int soc_id, char player, move_t move);
void human_notify(int soc_id, char player, move_t move);

#endif
