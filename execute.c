#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
pid_t child;
int status;
char **args;
char *cmd_path = NULL;

args = split_line(line);

/* Trouver le chemin */
if (strchr(args[0], '/'))
{
if (access(args[0], X_OK) != 0)
{
perror("./hsh");
free(args);
return;
}
cmd_path = args[0];
}
else
{
cmd_path = find_path(args[0]);
if (!cmd_path)
{
perror("./hsh");
free(args);
return;
}
}

/* fork */
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

/* free */
if (cmd_path != args[0])
free(cmd_path);
free(args);
}
