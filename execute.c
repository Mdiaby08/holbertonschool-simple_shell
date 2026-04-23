#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
    pid_t child;
    int status;
    char **args;
    char *cmd_path = NULL;

    args = split_line(line);

    /* Si la commande contient un / */
    if (strchr(args[0], '/'))
    {
        if (access(args[0], X_OK) != 0)
        {
            write(STDERR_FILENO, "./hsh: 1: ", 11);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 12);
            free(args);
            exit(127);
        }
        cmd_path = args[0];
    }
    else
    {
        cmd_path = find_path(args[0]);
        if (!cmd_path)
        {
            write(STDERR_FILENO, "./hsh: 1: ", 11);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 12);
            free(args);
            exit(127);
        }
    }

    /* fork + execve */
    child = fork();
    if (child == -1)
    {
        perror("fork");
        if (cmd_path != args[0])
            free(cmd_path);
        free(args);
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

    free(args);
}
