#include "shell.h"

void remove_newline(char *line)
{
    line[strcspn(line, "\n")] = '\0';
}

char **split_line(char *line)
{
    char **args = malloc(sizeof(char *) * 64);
    char *token;
    int i = 0;

    if (!args)
        return NULL;

    token = strtok(line, " \t");
    while (token)
    {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }

    args[i] = NULL;
    return args;
}
