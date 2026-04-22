#include "shell.h"

int main(void)
{
char *line;

while (1)
{
display_prompt();
line = read_line();
if (!line)
return (0);

if (line[0] != '\0')
execute_command(line);

free(line);
}

return (0);
}

