#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>

void send_sig(char *str);
void my_convert_bin(char *mot);

int PID_SERVEUR;

int main(int argc, char const *argv[])
{

    // regarde si il y a au moins 2 arguments, le PID du serveur et 1 ou plusieurs mots
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <PID_SERVEUR> <mot> <mot> ...\n", argv[0]);
        return 1;
    }

    // on vérifie que le le premier argument qui doit être le PID du serveur est bien un numéric
    const char *pid_serveur_test = argv[1];
    int i = 0;
    while (pid_serveur_test[i] != '\0')
    {
        if (!isdigit(pid_serveur_test[i]))
        {
            fprintf(stderr, "Le PID du serveur doit être de type numéric\n");
            return 1;
        }
        i++;
    }

    PID_SERVEUR = atoi(argv[1]);

    // on initialise à 2 car le premier paramètre est le PID du serveur
    i = 2;

    while (i < argc)
    {
        my_convert_bin((char *)argv[i]);
        i++;
    }

    return 0;
}

// convertie chaque lettre de chaque mot en binaire
void my_convert_bin(char *mot)
{

    int placement_bin;
    int bit;
    int lettre = 0;

    // s'occupe de séparer les mots en lettres
    while (mot[lettre] != '\0')
    {
        bit = 7;
        char lettre_bin[9] = "";

        // Transforme les lettres en binaire
        while (bit >= 0)
        {
            lettre_bin[7 - bit] = (mot[lettre] & (1 << bit)) ? '1' : '0';
            bit--;
        }

        lettre_bin[8] = '\0';

        // passe la chaine de binaire qui représente une lettre pour l'afficher
        // mettre ici la fonction qui permettrai d'envoyer les SIGUSR1 et SIGUSR2 au serveur
        send_sig(lettre_bin);

        lettre++;
    }
}

void send_sig(char *str)
{
    // boucle sur la chaine str
    while (*str != '\0')
    {
        printf("Envoi du signal : %c\n", *str);

        // en fonction de si le charactère est 1 ou 0 envoie un SIGUSR1 ou SIGUSR2 sur le pid du serveur
        if (*str == '0')
        {
            kill(PID_SERVEUR, SIGUSR1);
        }
        else if (*str == '1')
        {
            kill(PID_SERVEUR, SIGUSR2);
        }
        printf("Signal envoyé\n");

        str++;
    }
}