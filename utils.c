#include "shell.h"

/**
 * remove_newline - replaces the trailing '\n' with '\0'.
 *                  strcspn returns the index of the first '\n' found.
 * @line: string modified in place
 * Return: void
 */
void remove_newline(char *line)
{
	line[strcspn(line, "\n")] = '\0';
}

/**
 * split_line - splits a string into a NULL-terminated word array.
 *              strtok splits on spaces and tabs, modifying the string.
 *              execve requires the array to end with NULL.
 * @line: string modified in place by strtok
 * Return: array of word pointers, NULL if malloc fails
 */
char **split_line(char *line)
{
	char **args = malloc(sizeof(char *) * 64); /* 64 tokens max */
	char *token;
	int i = 0;

	if (!args)
		return (NULL);

	token = strtok(line, " \t");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " \t"); /* NULL = continue in same string */
	}

	args[i] = NULL; /* marks end of array, required by execve */
	return (args);
}
