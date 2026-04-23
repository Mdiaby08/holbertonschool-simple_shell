#include "shell.h"

/*
 * main :
 * - boucle infinie du shell
 * - affiche le prompt si interactif
 * - lit une ligne
 * - exécute la commande
 */
int main(void)
{
    char *line = NULL;   /* ligne tapée par l'utilisateur */
    size_t len = 0;      /* taille du buffer getline */
    ssize_t nread;       /* nombre de caractères lus */

    while (1)
    {
        /* afficher le prompt seulement si terminal interactif */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        /* lire une ligne (getline alloue si line == NULL) */
        nread = getline(&line, &len, stdin);

        /* si CTRL+D → quitter proprement */
        if (nread == -1)
        {
            free(line);
            return 0;
        }

        /* enlever le '\n' final */
        remove_newline(line);

        /* exécuter la commande */
        execute_command(line);
    }

    free(line);
    return 0;
}
