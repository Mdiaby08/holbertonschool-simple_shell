#include "shell.h"

/**
 * main - entry point for the simple shell
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line;
	size_t len;
	ssize_t nread;
	unsigned int count;
	int status;

	(void)argc;
	line = NULL;
	len = 0;
	count = 0;
	status = 0;
	while (1)
	{
		display_prompt();
		nread = read_line(&line, &len);
		if (nread == -1)
			break;
		if (line[0] == '\0')
			continue;
		count++;
		status = execute_command(line, argv[0], count);
	}
	free(line);
	return (status);
}
