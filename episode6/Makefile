CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: my_tcp_serveur_nonblock

my_tcp_serveur_nonblock: my_tcp_serveur_nonblock.o
	$(CC) $(CFLAGS) -o my_tcp_serveur_nonblock my_tcp_serveur_nonblock.o

my_tcp_serveur_nonblock.o: my_tcp_serveur_nonblock.c
	$(CC) $(CFLAGS) -c my_tcp_serveur_nonblock.c

clean:
	rm -f *.o

fclean: clean
	rm -f my_tcp_serveur_nonblock

re: fclean all

lclean: 
	rm -f *.txt

.PHONY: all clean fclean re lclean