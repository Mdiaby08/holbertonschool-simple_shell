#include "shell.h"

char *read_line(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;

nread = getline(&line, &len, stdin);
if (nread == -1)
{
free(line);
return (NULL);
}

remove_newline(line);
trim_spaces(line);
return (line);
}
