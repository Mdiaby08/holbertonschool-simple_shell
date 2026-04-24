#include "shell.h"
/**
*remove_newline - enlève le '\n' final de getline
*
*@line: chaîne modifiée en place
*
*Return: void
*/

void remove_newline(char *line)
{
line[strcspn(line, "\n")] = '\0';
}

/**
*split_line - splits a string into an array of words using strtok.
*Separators: space and tab.
*The array is terminated by NULL (required by execve).
*@line: string modified in place by strtok
*Return: array of pointers to the words, NULL if malloc fails
*/
char **split_line(char *line)
{
char **args;
char *token;
int i = 0;

args = malloc(sizeof(char *) * 64);
if (!args)
return (NULL);

token = strtok(line, " \t");
while (token)
{
args[i] = token;
i++;
token = strtok(NULL, " \t");
}

args[i] = NULL;
return (args);
}
