/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <time.h>
#include <pthread.h>
#include "log.h"

pthread_mutex_t log_lock;
void get_time_str(char *time_str, int size);

int log_init(FILE **log_file) {
	*log_file = fopen("log.txt", "ab+");

	if (pthread_mutex_init(&log_lock, NULL) != 0)
		return 1;
	return 0;
}

void log_destroy(FILE *log_file) {
        fflush(log_file);
	pthread_mutex_destroy(&log_lock);
	fclose(log_file);
}

void log_write(FILE *log_file, char *msg) {
	pthread_mutex_lock(&log_lock);
        fprintf(stdout, "%s\n", msg);
        fprintf(log_file, "%s\n", msg);
	pthread_mutex_unlock(&log_lock);
}

void log_connect(FILE *log_file, char *ip, int soc_id) {
	char time_str[100];
	get_time_str(time_str, 100);
	pthread_mutex_lock(&log_lock);
        fprintf(stdout, "[%s](%s)(soc_id %d) client connected\n", time_str, ip, soc_id);
        fprintf(log_file, "[%s](%s)(soc_id %d) client connected\n", time_str, ip, soc_id);
	pthread_mutex_unlock(&log_lock);
}

void log_move(FILE *log_file, char *ip, int soc_id, move_t move) {
	char time_str[100];
	get_time_str(time_str, 100);
	pthread_mutex_lock(&log_lock);
	if (soc_id == -1) {
        	fprintf(stdout, "[%s](%s) server's move = %hd\n", time_str, ip, move);
        	fprintf(log_file, "[%s](%s) server's move = %hd\n", time_str, ip, move);
	} else {
        	fprintf(stdout, "[%s](%s)(soc_id %d) client's move = %hd\n", time_str, ip, soc_id, move);
        	fprintf(log_file, "[%s](%s)(soc_id %d) client's move = %hd\n", time_str, ip, soc_id, move);
	}
	pthread_mutex_unlock(&log_lock);
}

void get_time_str(char *time_str, int size) {
        time_t cur_time = time(NULL);
        struct tm *loc_time = localtime(&cur_time);

        strftime(time_str, size, "%d %m %y %H:%M:%S", loc_time);
}
