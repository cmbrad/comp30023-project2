#include <stdio.h>
#include "move.h"

void log_write(FILE *log_file, char *msg);
void log_connect(FILE *log_file, char *ip, int soc_id);
void log_move(FILE *log_file, char *ip, int soc_id, move_t move);
