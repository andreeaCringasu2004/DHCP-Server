CC = gcc
CFLAGS = -Wall
SRC := Server/server.c Client/client.c
OBJ = $(SRC:.c=.o)

all: server client

server: Server/server.c
	$(CC) $(CFLAGS) $< -o $@

client: Client/client.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) server client