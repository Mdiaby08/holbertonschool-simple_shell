#include "shell.h"

/**
 * read_line - reads one command line from standard input
 * @line: address of the getline buffer pointer
 * @len: address of the getline buffer size
 *
 * Return: number of read characters, or -1 on EOF/error
 */
ssize_t read_line(char **line, size_t *len)
{
	ssize_t nread;

	nread = getline(line, len, stdin);
	if (nread == -1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		return (-1);
	}

	remove_newline(*line);

	return (nread);
}
