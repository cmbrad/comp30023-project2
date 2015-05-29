/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdio.h>
#include "player.h"

#ifndef INCLUDE_TARGS_H
#define INCLUDE_TARGS_H

typedef struct targs {
	FILE *log_file;
	player_t *player;
} targs_t;

#endif
