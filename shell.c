#include "shell.h"

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			return (0);
		}

		remove_newline(line);

		if (strcmp(line, "exit") == 0)
		{
			free(line);
			exit(0);
		}

		execute_command(line);
	}

	free(line);
	return (0);
}
