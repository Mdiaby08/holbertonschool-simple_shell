#include "shell.h"
 
/**
 * main - entry point of the simple shell
 * @argc: argument count (unused)
 * @argv: argument vector, argv[0] is the program name
 *
 * Return: last command exit status
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int last_status = 0;
 
	(void)argc;
 
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
 
		nread = getline(&line, &len, stdin);
 
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			return (last_status);
		}
 
		remove_newline(line);
 
		last_status = execute_command(line, argv[0]);
	}
 
	free(line);
	return (last_status);
}
 