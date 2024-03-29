/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include "stdio.h"
#include "stdlib.h"
#include "player.h"

player_t *player_create(int soc_id, char *ip) {
	player_t *player = malloc(sizeof(*player));
	if (player == NULL) {
		printf("Failed to allocate for player.");
		return NULL;
	}
	player->soc_id = soc_id;
	player->ip = ip;

	return player;
}
