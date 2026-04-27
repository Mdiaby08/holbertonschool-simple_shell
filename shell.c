#include "shell.h"

/**
 * main - main shell loop: print prompt, read line, execute, repeat.
 *        Exits on EOF (Ctrl+D) or built-in "exit".
 *
 * Return: exit status of the last executed command
 */
int main(void)
{
	char *line = NULL;   /* buffer managed by getline (auto-allocated) */
	size_t len = 0;      /* buffer size, updated by getline */
	ssize_t nread;       /* chars read; -1 means EOF or error */
	int last_status = 0; /* keeps the last command exit code */

	while (1)
	{
		if (isatty(STDIN_FILENO)) /* only print prompt in interactive mode */
			write(STDOUT_FILENO, "$ ", 2);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* EOF (Ctrl+D): exit cleanly */
		{
			free(line);
			exit(last_status);
		}

		remove_newline(line); /* strip trailing '\n' left by getline */

		if (strcmp(line, "exit") == 0) /* built-in exit handled here */
		{
			free(line);
			exit(last_status);
		}

		last_status = execute_command(line);
	}

	free(line);
	return (0);
}
