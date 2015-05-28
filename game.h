/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdio.h>
#include "player.h"
#include "connect4.h"

#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#define MAX_PLAYERS 2

typedef struct game {
	player_t **players;
	int num_players;
	int winner;
	FILE *log_file;
} game_t;


void *game_start(void *);
game_t *game_create(player_t **, int);
void game_process(game_t *);

#define AI_IP "0.0.0.0"

move_t human_move(int soc_id, c4_t board);
move_t ai_move(int soc_id, c4_t board);

void human_notify_move(int, char, move_t);
void human_notify_status(int, status_t);

#endif
