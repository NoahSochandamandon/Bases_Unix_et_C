CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: my_tcp_serveur my_tcp_client

my_tcp_serveur: my_tcp_serveur.o
	$(CC) $(CFLAGS) -o my_tcp_serveur my_tcp_serveur.o

my_tcp_client: my_tcp_client.o
	$(CC) $(CFLAGS) -o my_tcp_client my_tcp_client.o

my_tcp_serveur.o: my_tcp_serveur.c
	$(CC) $(CFLAGS) -c my_tcp_serveur.c

my_tcp_client.o: my_tcp_client.c
	$(CC) $(CFLAGS) -c my_tcp_client.c

clean:
	rm -f *.o

fclean: clean
	rm -f my_tcp_serveur my_tcp_client

re: fclean all

lclean: 
	rm -f *.txt

.PHONY: all clean fclean re lclean