#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
pid_t child = fork();
int status;
char *args[] = {line, NULL};

if (child == -1)
{
perror("fork");
return;
}

if (child == 0)
{
if (execve(line, args, environ) == -1)
char *args[2];
args[0] = line;
args[1] = NULL;
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
