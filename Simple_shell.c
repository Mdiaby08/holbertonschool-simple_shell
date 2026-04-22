#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * main - Simple shell 0.1
 *
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t child;
    int status;

    while (1)
    {
        /* Afficher le prompt seulement en mode interactif */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        nread = getline(&line, &len, stdin);

        /* Gestion de Ctrl+D */
        if (nread == -1)
        {
            free(line);
            return (0);
        }

        /* Retirer le \n */
        line[strcspn(line, "\n")] = '\0';

        /* Ignorer ligne vide */
        if (line[0] == '\0')
            continue;

        child = fork();
        if (child == -1)
        {
            perror("fork");
            free(line);
            exit(1);
        }

        if (child == 0)
        {
            /* Enfant : exécuter la commande */
            if (execve(line, NULL, environ) == -1)
            {
                perror("./shell");
                exit(1);
            }
        }
        else
        {
            /* Parent : attendre l'enfant */
            wait(&status);
        }
    }

    free(line);
    return (0);
}
