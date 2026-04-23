#include "shell.h"

extern char **environ;

/*
*find_path - cherche une commande dans les dossiers du PATH
*Retourne un strdup() du chemin complet si trouvé
*Retourne NULL sinon
*/
char *find_path(char *command)
{
char *path = NULL, *copy, *dir;
char full[1024];
int i = 0;

/*Cherche la variable PATH= dans environ */
while (environ[i])
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path = environ[i] + 5; /* saute "PATH=" */
break;
}
i++;
}

if (!path)
return (NULL);

/* Copie modifiable du PATH */
copy = strdup(path);
if (!copy)
return (NULL);

/* Découpe PATH en dossiers */
dir = strtok(copy, ":");
while (dir)
{
/* Construit chemin complet : dossier + "/" + commande */
snprintf(full, sizeof(full), "%s/%s", dir, command);

/* Vérifie si exécutable */
if (access(full, X_OK) == 0)
{
free(copy);
return strdup(full); /* À libérer dans execute.c */
}

dir = strtok(NULL, ":");
}

free(copy);
return (NULL);
}
