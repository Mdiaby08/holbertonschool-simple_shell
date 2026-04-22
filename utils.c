#include "shell.h"

void remove_newline(char *line)
{
line[strcspn(line, "\n")] = '\0';
}

void trim_spaces(char *line)
{
    int i = 0;
    int j = strlen(line) - 1;

    while (line[i] == ' ' || line[i] == '\t')
        i++;

    while (j >= i && (line[j] == ' ' || line[j] == '\t'))
        j--;

    line[j + 1] = '\0';

    if (i > 0)
        memmove(line, line + i, j - i + 2);
}
