#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
pid_t child = fork();
int status;

if (child == -1)
{
perror("fork");
return;
}

if (child == 0)
{
char *args[2];
args[0] = line;
args[1] = NULL;

if (execve(line, args, environ) == -1)
{
perror("./shell");
exit(1);
}
}
else
{
wait(&status);
}
}
