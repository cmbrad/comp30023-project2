/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdio.h>
#include "move.h"
#include "status.h"

#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

int log_init(FILE **log_file);
void log_destroy(FILE *log_file);

void log_write(FILE *log_file, char *msg);
void log_connect(FILE *log_file, char *ip, int soc_id);
void log_game_over(FILE *log_file, char *ip, int soc_id, status_t code);
void log_move(FILE *log_file, char *ip, int soc_id, move_t move);

#endif
