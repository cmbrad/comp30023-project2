#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "player.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include "move.h"

#define AI_IP "0.0.0.0"

move_t human_move(int soc_id);
move_t ai_move(int soc_id);

void *game_start(void *params) {
	player_t *players[MAX_PLAYERS];
	
	players[0] = (player_t *)params;
	players[0]->get_move = human_move;
	players[1] = player_create(-1, AI_IP);
	players[1]->get_move = ai_move;
	game_t *game = game_create(players, MAX_PLAYERS);
	printf("Created game!\n");
	game_process(game);
	printf("Game over.\n");
	return NULL;
}

void game_process(game_t *game) {
	move_t move = 0;
	player_t *cur_p = NULL;
	
	while (game->winner == -1 && move >= 0) {
		for (int i = 0; i < game->num_players && move >= 0; i++) {
			cur_p = game->players[i];
			if (cur_p->get_move == NULL) {
				printf("MOVE FUNCTION NOT SET.\n");
				exit(EXIT_FAILURE);
			}
			move = cur_p->get_move(cur_p->soc_id);
			printf("move %hhd\n", move);
		}
	}
	
	printf("Game Over.\n");
	for (int i = 0; i < game->num_players; i++) {
		close(game->players[i]->soc_id);
	}
}

move_t human_move(int soc_id) {
	move_t move = -1;
	printf("Getting move for %d\n", soc_id);
	if (recv(soc_id, &move, sizeof(move), 0) == 0) {
		printf("Lost connection to client.");
		return -1;
	}
	return ntohs(move);
}

move_t ai_move(int soc_id) {
	move_t move = -1;
	printf("Getting move for %d\n", soc_id);
	move=12;
	return move;
}
	
game_t *game_create(player_t **players, int num_players) {
	game_t *game = malloc(sizeof(*game));
	if (game == NULL) {
		printf("Failed to allocate memory for new game.");
		return NULL;
	}
	game->players = players;
	game->num_players = num_players;
	game->winner = -1;

	return game;
}
