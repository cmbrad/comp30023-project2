/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdint.h>

#define STATUS_MOVE_INVALID	1
#define STATUS_MOVE_VALID	2
#define STATUS_GAME_ACTIVE	3
#define STATUS_GAME_YELLOW_WIN	4
#define STATUS_GAME_RED_WIN	5
#define STATUS_GAME_DRAW	6

typedef int16_t status_t;
