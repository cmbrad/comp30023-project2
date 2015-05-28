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
#include "connect4.h"

int main(int argc, char *argv[])
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	int s, server_port;
	move_t move;

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
	print_config(board);
	while(scanf("%hd", &move))
	{
		//printf("%lu %lu %lu\n", sizeof(int8_t), sizeof(int16_t), sizeof(int32_t));
		//printf("Send move: %hd, sizeof(move)=%lu\n", move, sizeof(move));
		move = htons(move);
		send(s, &move, sizeof(move), 0);

		do_move(board, ntohs(move), YELLOW);
		print_config(board);

		move_t op_move;
		recv(s, &op_move, sizeof(op_move), 0);
		//printf("Opponent did this: %d\n", op_move);

		do_move(board, op_move, RED);
		print_config(board);
	}
	close(s);
}
