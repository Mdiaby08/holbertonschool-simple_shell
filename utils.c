#include "shell.h"

/* enlève le '\n' final de getline */
void remove_newline(char *line)
{
line[strcspn(line, "\n")] = '\0';
}

/* découpe la ligne en mots (tokens) */
char **split_line(char *line)
{
char **args;
char *token;
int i = 0;

/* tableau de 64 mots max → suffisant pour simple_shell */
args = malloc(sizeof(char *) * 64);
if (!args)
return (NULL);

/* découper sur espace et tabulation */
token = strtok(line, " \t");
while (token)
{
args[i] = token;
i++;
token = strtok(NULL, " \t");
}

args[i] = NULL; /* fin du tableau */
return (args);
}
