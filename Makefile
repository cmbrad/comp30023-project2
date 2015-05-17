CC = gcc
CFLAGS = -Wall -std=c11
LDFLAGS = -lm -lpthread

S_SRC = game.c player.c server.c connect4.c
S_OBJ = game.o player.o server.o connect4.o

C_SRC = game.c player.c client.c connect4.c
C_OBJ = game.o player.o client.o connect4.o

all: client server

server: $(S_OBJ)
	$(CC) $(CFLAGS) -o server $(S_OBJ) $(LDFLAGS)

client: $(C_OBJ)
	$(CC) $(CFLAGS) -o client $(C_OBJ) $(LDFLAGS)

clean:
	rm $(OBJ)

clobber: clean
	rm $(EXE)

## Dependencies

game.o: game.c game.h player.h move.h
player.o: player.c player.h
connect4.o: connect4.c connect4.h

server.o: server.c game.h player.h

client.o: client.c game.h player.h move.h
