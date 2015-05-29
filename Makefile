CC = gcc
CFLAGS = -Wall -std=c99 
LDFLAGS = -lm -lpthread

S_SRC = game.c player.c server.c connect4.c log.c
S_OBJ = game.o player.o server.o connect4.o log.o

C_SRC = game.c player.c client.c connect4.c log.c
C_OBJ = game.o player.o client.o connect4.o log.o

all: client server

server: $(S_OBJ)
	$(CC) $(CFLAGS) -o server $(S_OBJ) $(LDFLAGS)

client: $(C_OBJ)
	$(CC) $(CFLAGS) -o client $(C_OBJ) $(LDFLAGS)

clean:
	rm $(S_OBJ) client.o

clobber: clean
	rm server client

## Dependencies

game.o: game.c game.h player.h move.h log.h
player.o: player.c player.h
connect4.o: connect4.c connect4.h
log.o: log.c log.h move.h

server.o: server.c game.h player.h log.h

client.o: client.c game.h player.h move.h
