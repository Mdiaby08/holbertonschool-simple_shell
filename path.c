#include "shell.h"

char *find_path(char *command)
{
char *path = getenv("PATH");
char *path_copy, *dir;
char full_path[1024];

if (!path)
return (NULL);

path_copy = strdup(path);
if (!path_copy)
return (NULL);

dir = strtok(path_copy, ":");
while (dir)
{
snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

if (access(full_path, X_OK) == 0)
{
free(path_copy);
return (strdup(full_path));
}

dir = strtok(NULL, ":");
}

free(path_copy);
return (NULL);
}
