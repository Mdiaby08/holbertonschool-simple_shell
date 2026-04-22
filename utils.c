#include "shell.h"

void remove_newline(char *line)
{
line[strcspn(line, "\n")] = '\0';
}
