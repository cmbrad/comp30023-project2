#include <time.h>
#include "log.h"

void get_time_str(char *time_str, int size);

void log_write(FILE *log_file, char *msg) {
        fprintf(log_file, "%s\n", msg);
        fflush(log_file);
}

void log_connect(FILE *log_file, char *ip, int soc_id) {
	char time_str[100];
	get_time_str(time_str, 100);
        fprintf(log_file, "[%s](%s)(soc_id %d) client connected\n", time_str, ip, soc_id);
        fflush(log_file);
}

void log_move(FILE *log_file, char *ip, int soc_id, move_t move) {
	char time_str[100];
	get_time_str(time_str, 100);
	if (soc_id == -1)
        	fprintf(log_file, "[%s](%s) server's move = %hd\n", time_str, ip, move);
	else
        	fprintf(log_file, "[%s](%s)(soc_id %d) client's move = %hd\n", time_str, ip, soc_id, move);

        fflush(log_file);
}

void get_time_str(char *time_str, int size) {
        time_t cur_time = time(NULL);
        struct tm *loc_time = localtime(&cur_time);

        strftime(time_str, size, "%d %m %y %H:%M:%S", loc_time);
}
