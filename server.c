#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "game.h"
#include "player.h"
#include "targs.h"
#include "log.h"

#define MAX_GAMES 1024

int main (int argc, char *argv[])
{
	struct sockaddr_in server, client;
	socklen_t len;
	int s, new_s, server_port;

	FILE *log_file = fopen("log.txt", "ab+");

	log_write(log_file, "Started.");
	if(argc == 2) {
		server_port = atoi(argv[1]);
	}
	else {
		fprintf(stderr, "Usage :server portnumber\n");
		exit(EXIT_FAILURE);
	}
	printf("Server port %i\n", server_port);
	/* Building data structures for sockets */
	/* Identify two end points; one for the server and the other for the client when it connects to the server socket */
	memset (&server,0, sizeof (server));
	memset (&client,0, sizeof (client));
	/* Server socket initializations */
	/* AF_INET: specifies the connection to Internet. In our example we use 
	TCP port 5431 as the known server port; Any client need to connect to this port;
	INADDR_ANY specifies the server is willing to accept connections on any of the local host's IP addresses. */ 

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons (server_port); 

	/* Preliminary server steps: Setup: creation of passive open socket*/

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
		perror ("Error creating socket");
		exit (EXIT_FAILURE);
	}

	/* Bind the socket to local address */
	if (bind(s, (struct sockaddr *) &server, sizeof (server)) == -1)	{
		perror ("bind");
		exit (EXIT_FAILURE);
	}

	/* Sets the maximum number of pending connections to be allowed, in our case this number is 10 */

	if (listen(s, 5) == -1) {
		perror("listen");
		exit(1);
	}
	else {
		printf("Listening on port %d...\n", ntohs(server.sin_port));
	}
	/* The main loop of the program*/
	/* Wait for connection then receive and print text */

	// Active games.
	

	while (1) {
		len = sizeof(client);

		if ((new_s = accept(s, (struct sockaddr *)&client, &len)) == -1) {
			perror("Accept failed");
			exit(EXIT_FAILURE);
		} else {
			char *player_ip = malloc(sizeof(INET_ADDRSTRLEN));
			inet_ntop(AF_INET,&(client.sin_addr), player_ip, INET_ADDRSTRLEN);
			printf("connection accepted from client %s\n", player_ip);
		
			pthread_t thread0;
			printf("new_s=%d\n", new_s);

			player_t *player = player_create(new_s, player_ip);

			targs_t *targs = malloc(sizeof(*targs));
			targs->player = player;
			targs->log_file = log_file;

			pthread_create(&thread0, NULL, game_start, targs);
		// Pass connection off to a thread to handle.. 
		}
		//while (len = recv(new_s, &msg, sizeof(msg), 0)) { 
		//	printf("%s", msg);
		//}
		//printf("lost connection.");
		//close(new_s);
	}
	close(s);

	fclose(log_file);

	return 1;
}
