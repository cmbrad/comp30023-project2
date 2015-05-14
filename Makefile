CC = gcc
CFLAGS = -Wall -std=c11

SRC = game.c player.c server.c
OBJ = game.o player.o server.o
EXE = server

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm -lpthread

clean:
	rm $(OBJ)

clobber: clean
	rm $(EXE)

## Dependencies

game.o: game.c game.h player.h
player.o: player.c player.h
server.o: server.c game.h player.h
