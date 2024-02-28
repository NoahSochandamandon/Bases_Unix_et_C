CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: serveur client

serveur: serveur.o
	$(CC) $(CFLAGS) -o serveur serveur.o

client: client.o
	$(CC) $(CFLAGS) -o client client.o

serveur.o: serveur.c
	$(CC) $(CFLAGS) -c serveur.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o

fclean: clean
	rm -f serveur client

re: fclean all

lclean: 
	rm -f *.txt

.PHONY: all clean fclean re lclean