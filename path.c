#include "shell.h"

char *find_path(char *command)
{
extern char **environ;
char *path = NULL, *copy, *dir;
char full[1024];
int i = 0;

while (environ[i])
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path = environ[i] + 5;
break;
}
i++;
}

if (!path)
return (NULL);

copy = strdup(path);
if (!copy)
return (NULL);

dir = strtok(copy, ":");
while (dir)
{
snprintf(full, sizeof(full), "%s/%s", dir, command);
if (access(full, X_OK) == 0)
{
free(copy);
return strdup(full);
}
dir = strtok(NULL, ":");
}

free(copy);
return (NULL);
}
