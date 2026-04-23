#include "shell.h"

extern char **environ;

/* copie simple de chaîne */
static char *duplicate_string(char *str)
{
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;

    strcpy(copy, str);
    return copy;
}

/* récupère la valeur de PATH */
char *get_path_value(void)
{
    int i;

    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return environ[i] + 5;
    }

    return NULL;
}

/* construit un chemin complet : dir + "/" + command */
char *build_path(char *dir, char *command)
{
    size_t len = strlen(dir) + strlen(command) + 2;
    char *full_path = malloc(len);

    if (!full_path)
        return NULL;

    strcpy(full_path, dir);
    strcat(full_path, "/");
    strcat(full_path, command);

    return full_path;
}

/* cherche une commande dans PATH */
char *find_path(char *command)
{
    char *path_env, *path_copy, *dir, *full_path;

    /* commande vide → rien à faire */
    if (!command || command[0] == '\0')
        return NULL;

    /* si la commande contient un / → tester directement */
    if (strchr(command, '/'))
    {
        if (access(command, X_OK) == 0)
            return duplicate_string(command);
        return NULL;
    }

    /* récupérer PATH */
    path_env = get_path_value();
    if (!path_env || path_env[0] == '\0')
        return NULL;

    /* copie modifiable du PATH */
    path_copy = duplicate_string(path_env);
    if (!path_copy)
        return NULL;

    /* parcourir chaque dossier du PATH */
    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = build_path(dir, command);

        if (full_path && access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
