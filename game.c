#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "move.h"
#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "player.h"
#include "targs.h"
#include "log.h"

void *game_start(void *params) {
	targs_t *targs = (targs_t *)params;
	player_t *players[MAX_PLAYERS];

	players[0] = targs->player;
	players[0]->get_move = human_move;
	players[0]->colour = YELLOW;
	players[0]->notify_move = human_notify;
	players[1] = player_create(-1, AI_IP);
	players[1]->get_move = ai_move;
	players[1]->colour = RED;
	players[1]->notify_move = ai_notify;

	log_connect(targs->log_file, AI_IP, players[0]->soc_id);

	game_t *game = game_create(players, MAX_PLAYERS);
	game->log_file = targs->log_file;
	//printf("Created game!\n");
	game_process(game);
	//printf("Game over. %d won.\n", game->winner);
	return NULL;
}

void game_process(game_t *game) {
	c4_t board;

	srand(RSEED);
	init_empty(board);
	//print_config(board);

	move_t move = 0;
	player_t *cur_p = NULL;
	
	while (game->winner == -1 && move >= 0) {
		for (int i = 0; i < game->num_players && move >= 0; i++) {
			cur_p = game->players[i];
			if (cur_p->get_move == NULL) {
				//printf("MOVE FUNCTION NOT SET.\n");
				game->winner = -2;
				break;
			}

			if (!move_possible(board)) {
				game->winner = 0;	// DRAW
				break;
			} else {
				// Get move from current player
				move = cur_p->get_move(cur_p->soc_id, board);
				log_move(game->log_file, cur_p->ip, cur_p->soc_id, move);
				// Notify other players of move...
				for (int j = 0; j < game->num_players; j++) {
					if (game->players[j] == cur_p)
						continue;
					if (game->players[j]->notify_move == NULL) {
						//printf("Player has not given us a way to contact them. How rude.\n");
						game->winner = -2;
						break;
					}
					game->players[j]->notify_move(game->players[j]->soc_id, cur_p->colour, move);
				}

				//printf("move %hd\n", move);
				if (do_move(board, move, cur_p->colour) != 1) {
					//printf("Panic\n");
					game->winner = -2;
					break;
				}
				// print_config(board);

				/* and did we win??? */
				if (winner_found(board) == YELLOW || winner_found(board) == RED) {
					game->winner = winner_found(board);
				}
			}
		}
	}
	
	for (int i = 0; i < game->num_players; i++) {
		//printf("Closing %d.\n", game->players[i]->soc_id);
		close(game->players[i]->soc_id);
	}
}

void human_notify(int soc_id, char player, move_t move)
{
	send(soc_id, &move, sizeof(move), 0);
	//printf("OI HUMAN\n");
}

void ai_notify(int soc_id, char player, move_t move)
{
	//printf("OI AI\n");
}

move_t human_move(int soc_id, c4_t board) {
	move_t move = -1;
	if (recv(soc_id, &move, sizeof(move), 0) == 0) {
		//printf("Lost connection to client.");
		return -1;
	}
	return ntohs(move);
}

move_t ai_move(int soc_id, c4_t board) {
	//printf("Getting move for %d\n", soc_id);
	return suggest_move(board, RED);
}
	
game_t *game_create(player_t **players, int num_players) {
	game_t *game = malloc(sizeof(*game));
	if (game == NULL) {
		//printf("Failed to allocate memory for new game.");
		return NULL;
	}
	game->players = players;
	game->num_players = num_players;
	game->winner = -1;

	return game;
}
