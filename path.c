#include "shell.h"

extern char **environ;

static char *duplicate_string(char *str)
{
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;
    strcpy(copy, str);
    return copy;
}

char *get_path_value(void)
{
    int i;

    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return environ[i] + 5;
    }
    return NULL;
}

char *build_path(char *dir, char *command)
{
    size_t len = strlen(dir) + strlen(command) + 2;
    char *full = malloc(len);

    if (!full)
        return NULL;

    strcpy(full, dir);
    strcat(full, "/");
    strcat(full, command);

    return full;
}

char *find_path(char *command)
{
    char *path_env = get_path_value();
    char *copy, *dir, *full;

    if (!path_env)
        return NULL;

    copy = duplicate_string(path_env);
    if (!copy)
        return NULL;

    dir = strtok(copy, ":");
    while (dir)
    {
        full = build_path(dir, command);
        if (full && access(full, X_OK) == 0)
        {
            free(copy);
            return full;
        }
        free(full);
        dir = strtok(NULL, ":");
    }

    free(copy);
    return NULL;
}
