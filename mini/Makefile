CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: server client

server: server.o
	$(CC) $(CFLAGS) -o server server.o

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o

fclean: clean
	rm -f server client

lclean: 
	rm -f *.txt

.PHONY: all clean fclean re lclean
