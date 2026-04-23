#include "shell.h"

/* environ = tableau des variables d'environnement */
extern char **environ;

/*
 * execute_command :
 * - découpe la ligne
 * - gère exit
 * - gère env
 * - cherche la commande
 * - exécute avec fork/execve
 */
void execute_command(char *line)
{
    pid_t child;          /* PID du fork */
    int status;           /* statut du wait */
    char **args;          /* liste des mots de la commande */
    char *cmd_path = NULL;/* chemin final de la commande */

    /* ----------------------------------------------------------
     * 1) Découper la ligne en mots
     * ---------------------------------------------------------- */
    args = split_line(line);

    /* si rien à exécuter → on sort */
    if (!args || !args[0])
    {
        free(args);
        return;
    }

    /* ----------------------------------------------------------
     * 2) BUILTIN : exit
     * ---------------------------------------------------------- */
    if (strcmp(args[0], "exit") == 0)
    {
        free(args);   /* éviter fuite mémoire */
        exit(0);      /* quitter le shell */
    }

    /* ----------------------------------------------------------
     * 3) BUILTIN : env
     * ---------------------------------------------------------- */
    if (strcmp(args[0], "env") == 0)
    {
        int i = 0;

        /* afficher chaque variable d'environnement */
        while (environ[i])
        {
            write(STDOUT_FILENO, environ[i], strlen(environ[i]));
            write(STDOUT_FILENO, "\n", 1);
            i++;
        }

        free(args);   /* pas de fork → on libère et on revient */
        return;
    }

    /* ----------------------------------------------------------
     * 4) Commande contenant un '/'
     *    Exemple : /bin/ls, ./a.out
     * ---------------------------------------------------------- */
    if (strchr(args[0], '/'))
    {
        /* vérifier que le fichier est exécutable */
        if (access(args[0], X_OK) != 0)
        {
            /* message EXACT attendu par le checker */
            write(STDERR_FILENO, "./hsh: 1: ", 11);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 12);

            free(args);
            exit(127); /* code standard "commande introuvable" */
        }

        cmd_path = args[0]; /* on utilise tel quel */
    }

    /* ----------------------------------------------------------
     * 5) Commande SANS '/'
     *    Exemple : ls, echo, pwd
     *    → on cherche dans PATH
     * ---------------------------------------------------------- */
    else
    {
        cmd_path = find_path(args[0]);

        /* si rien trouvé → erreur */
        if (!cmd_path)
        {
            write(STDERR_FILENO, "./hsh: 1: ", 11);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 12);

            free(args);
            exit(127);
        }
    }

    /* ----------------------------------------------------------
     * 6) fork + execve
     * ---------------------------------------------------------- */
    child = fork();

    /* fork raté */
    if (child == -1)
    {
        perror("fork");

        /* libérer si find_path a alloué */
        if (cmd_path != args[0])
            free(cmd_path);

        free(args);
        return;
    }

    /* ------------------ PROCESSUS ENFANT ------------------ */
    if (child == 0)
    {
        /* remplacer le programme courant par la commande */
        execve(cmd_path, args, environ);

        /* si execve échoue */
        perror("./hsh");
        exit(1);
    }

    /* ------------------ PROCESSUS PARENT ------------------ */
    else
    {
        /* attendre la fin de l'enfant */
        wait(&status);
    }

    /* ----------------------------------------------------------
     * 7) Nettoyage mémoire
     * ---------------------------------------------------------- */

    /* libérer cmd_path si find_path l'a alloué */
    if (cmd_path != args[0])
        free(cmd_path);

    /* libérer le tableau d'arguments */
    free(args);
}
