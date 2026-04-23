#include "shell.h"

extern char **environ;

char *find_path(char *command)
{
    char *path = NULL;
    char *path_copy, *dir;
    char full_path[1024];
    int i = 0;

/* Trouver PATH= dans environ */
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
return (strdup(full_path)); /* à free dans execute */
}
dir = strtok(NULL, ":");

}
free(path_copy);
return (NULL);
}
