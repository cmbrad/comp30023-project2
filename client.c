/*
 * Author : Christopher Bradley <635847>
 * Contact: bradleyc1@student.unimelb.edu.au
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "move.h"
#include "status.h"
#include "connect4.h"

void send_our_move(int s, move_t move);
move_t get_opponent_move(int s);
status_t get_status_update(int s);
int check_win_state(int s);

int main(int argc, char *argv[])
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	int s, server_port;
	move_t move;
	status_t status;
	c4_t board;
	
	if (argc == 3) {
		host = argv[1];
		server_port = atoi(argv[2]);
	}
	else {
		fprintf(stderr, "Usage :client host server_port\n");
		exit(1);
	}

	/* translate host name into peer's IP address ; This is name translation service by the operating system */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "Unknown host %s  \n",host);
		exit(1);
	}
	/* Building data structures for socket */

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	memcpy((char *)&sin.sin_addr, hp->h_addr_list[0], hp->h_length);
	sin.sin_port = htons(server_port);

	/* Active open */
	/* Preliminary steps: Setup: creation of active open socket*/

	if ( (s = socket(AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		perror("Error in creating a socket to the server");
		exit(1);
	}

	if(connect(s, (struct sockaddr *)&sin, sizeof(sin))  < 0  )
	{
		perror("Error in connecting to the host");
		close(s);
		exit(1);
	}
	
	// We have a connection! Actually make the board.	
	init_empty(board);
	// Print initial board so user has some idea what to do.
	print_config(board);
	int game_over = 0;
	while (!game_over) {
		do {
			printf("Enter a move to play: ");
			scanf("%hd", &move);
			// Send our move
			send_our_move(s, move);
			// Check if we played a valid move
			status = get_status_update(s);
		} while (status == STATUS_MOVE_INVALID);
		
		// Add the move to our own board
		do_move(board, move, YELLOW);
		// Only print board after a valid move
		print_config(board);

		// We might have won! Check.
		if (check_win_state(s))
			break;
		
		// Receive a move from our opponent
		move = get_opponent_move(s);
		// Add their move to our board
		do_move(board, move, RED);
		// Print board after AI move
		print_config(board);

		// The AI might have won... Check
		if (check_win_state(s))
			break;
	}
	// Game over! Close the socket
	close(s);
}

void send_our_move(int s, move_t move) {
	// Convert to network byte order for sending
	move = htons(move);
	send(s, &move, sizeof(move), 0);
}

/*
 * Gets a move from the server for the opponent
 * We assume the server has validated it.
 */
move_t get_opponent_move(int s) {
	move_t move;
	recv(s, &move, sizeof(move), 0);
	return ntohs(move);
}

/*
 * Gets a status update from the server, ie, move invalid
 * or game over
 */
status_t get_status_update(int s) {
	status_t status;
	recv(s, &status, sizeof(status), 0);
	return ntohs(status);
}

/*
 * Consume a status update from the server to see if
 * the game is over.
 */
int check_win_state(int s) {
	status_t status = get_status_update(s);
	if (status == STATUS_GAME_YELLOW_WIN) {
		printf("Yay we won!\n");
		return 1;
	} else if (status == STATUS_GAME_RED_WIN) {
		printf("Aww we lose :(\n");
		return 1;
	} else if (status == STATUS_GAME_DRAW) {
		printf("It's a draw. o.o\n");
		return 1;
	}

	return 0;
}
