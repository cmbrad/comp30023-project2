/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

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

void notify_players(player_t **players, int player_cnt, status_t status);

void *game_start(void *params) {
	targs_t *targs = (targs_t *)params;
	player_t *players[MAX_PLAYERS];

	players[0] = targs->player;
	players[0]->get_move = human_move;
	players[0]->colour = YELLOW;
	players[0]->notify_move = human_notify_move;
	players[0]->notify_status = human_notify_status;
	players[1] = player_create(-1, AI_IP);
	players[1]->get_move = ai_move;
	players[1]->colour = RED;
	players[1]->notify_move = NULL;
	players[1]->notify_status = NULL;

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

	move_t move = 0;
	player_t *cur_p = NULL;
	
	while (game->winner == -1 && move >= 0) {
		for (int i = 0; i < game->num_players && move >= 0; i++) {
			cur_p = game->players[i];

			// Player somehow doesn't have a move function set.
			// This means we don't know how to get moves from them
			// Game over!
			if (cur_p->get_move == NULL) {
				game->winner = -2;
				break;
			}
			// There's no moves left and we have no winner? Draw.
			if (!move_possible(board)) {
				game->winner = 0;
				break;
			} else {
				// Get move from current player
				log_move(game->log_file, cur_p->ip, cur_p->soc_id, move);
			
				// Get an initial move for this player to make	
				move = cur_p->get_move(cur_p->soc_id, board);
				// Check if it's valid. If not ask for a new move.
				// We don't allow cheating here...
				while (do_move(board, move, cur_p->colour) != 1) {
					// AI players have no notify methods
					if (cur_p->notify_status != NULL)
						cur_p->notify_status(cur_p->soc_id, STATUS_MOVE_INVALID);
					move = cur_p->get_move(cur_p->soc_id, board);
				}
				// Notify the current player their move was valid (if we can anyway)
				// The AIs should always play a valid move.
				if (cur_p->notify_status != NULL)
					cur_p->notify_status(cur_p->soc_id, STATUS_MOVE_VALID);

				// Notify other players of move...
				for (int j = 0; j < game->num_players; j++) {
					if (game->players[j] == cur_p)
						continue;
					// AI players might not have notify functions implemented
					if (game->players[j]->notify_move != NULL) {
						game->players[j]->notify_move(game->players[j]->soc_id, cur_p->colour, move);
					}
				}

				// Have we finished this game? yeah/nah
				if (winner_found(board) == YELLOW || winner_found(board) == RED) {
					game->winner = winner_found(board);
					// Notify players of the winner, guess they'd want to know
					if (winner_found(board) == YELLOW)
						notify_players(game->players, game->num_players, STATUS_GAME_YELLOW_WIN);
					else if (winner_found(board) == RED)
						notify_players(game->players, game->num_players, STATUS_GAME_RED_WIN);
				} else if (!move_possible(board)){
					// Notify players nobody won! It's a drea
					notify_players(game->players, game->num_players, STATUS_GAME_DRAW);
				} else {
					// Game still yet to be decided
					notify_players(game->players, game->num_players, STATUS_GAME_ACTIVE);
				}
			}
		}
	}

	// Game is over. Close all the sockets!	
	for (int i = 0; i < game->num_players; i++) {
		close(game->players[i]->soc_id);
	}
}

void notify_players(player_t **players, int player_cnt, status_t status) {
	for (int i = 0; i < player_cnt; i++) {
		player_t *cur_p = players[i];
		if (cur_p->notify_status != NULL) {
			cur_p->notify_status(cur_p->soc_id, status);
		}
	}
}

void human_notify_status(int soc_id, status_t status) {
	// Send update to human player to tell them
	// of current game status
	status = htons(status);
	send(soc_id, &status, sizeof(status), 0);
}

void human_notify_move(int soc_id, char player, move_t move) {
	// Send update to human player to tell them the AI made a move
	move = htons(move);
	send(soc_id, &move, sizeof(move), 0);
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
