#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
    pid_t child;
    int status;
    char **args;
    char *cmd_path = NULL;

    args = split_line(line);

    /* Si la commande contient un / -> tester directement */
    if (strchr(args[0], '/'))
    {
        if (access(args[0], X_OK) != 0)
        {
            perror("./hsh");
            return; /* NE PAS fork si la commande n'existe pas */
        }
        cmd_path = args[0];
    }
    else
    {
        /* Sinon -> chercher dans PATH */
        cmd_path = find_path(args[0]);
        if (!cmd_path)
        {
            perror("./hsh");
            return; /* NE PAS fork si la commande n'existe pas */
        }
    }

    /* Exécuter */
    child = fork();
    if (child == -1)
    {
        perror("fork");
        return;
    }

    if (child == 0)
    {
        execve(cmd_path, args, environ);
        perror("./hsh");
        exit(1);
    }
    else
    {
        wait(&status);
    }

    if (cmd_path != args[0])
        free(cmd_path);
}
