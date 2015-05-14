CC = gcc
CFLAGS = -Wall -std=c11
LDLIBS = -lm -lpthread

S_SRC = game.c player.c server.c
S_OBJ = game.o player.o server.o

C_SRC = game.c player.c client.c
C_OBJ = game.o player.o client.o

all: client server

server: $(S_OBJ)
	$(CC) $(CFLAGS) -o server $(S_OBJ)

client: $(C_OBJ)
	$(CC) $(CFLAGS) -o client $(C_OBJ)

clean:
	rm $(OBJ)

clobber: clean
	rm $(EXE)

## Dependencies

game.o: game.c game.h player.h
player.o: player.c player.h
server.o: server.c game.h player.h

client.o: client.c game.h player.h
