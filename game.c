#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "player.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>

void *game_start(void *params) {
	char *ai_ip = "0.0.0.0";
	player_t *players[MAX_PLAYERS];
	
	players[0] = (player_t *)params;
	players[1] = player_create(-1, ai_ip);

	game_t *game = game_create(players, MAX_PLAYERS);
	printf("Created game!\n");
	game_process(game);
	printf("Game over.\n");
	return NULL;
}

void game_process(game_t *game) {
	int8_t move = -1;
	player_t *hum = game->players[0];
	while(recv(hum->soc_id, &move, sizeof(move), 0)) {
		printf("[DATE] (%s)(soc_id %d) msg = %hhd\n", hum->ip, hum->soc_id, move);
	}
	printf("lost connection to client.\n");
	close(hum->soc_id);
}
	
game_t *game_create(player_t **players, int num_players) {
	game_t *game = malloc(sizeof(*game));
	if (game == NULL) {
		printf("Failed to allocate memory for new game.");
		return NULL;
	}
	game->players = players;
	game->num_players = num_players;

	return game;
}
