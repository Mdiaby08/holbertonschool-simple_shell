#include "shell.h"

extern char **environ;

void execute_command(char *line)
{
pid_t child = fork();
int status;
char **args;

if (child == -1)
{
perror("fork");
return;
}

if (child == 0)
{
args = split_line(line);

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
args = split_line(line);

if (strchr(args[0], '/'))
{
if (access(args[0], X_OK) != 0)
{
perror("./hsh");
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
return;
}
}
}
