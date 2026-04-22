#include "shell.h"

/**
 * remove_newline - removes the trailing newline from a line
 * @line: input line
 */
void remove_newline(char *line)
{
	line[strcspn(line, "\n")] = '\0';
}

/**
 * split_line - splits a line into tokens
 * @line: input line
 *
 * Return: array of tokens, or NULL on failure
 */
char **split_line(char *line)
{
	char **args;
	char *token;
	int i;

	args = malloc(sizeof(char *) * 64);
	if (args == NULL)
		return (NULL);

	token = strtok(line, " \t");
	i = 0;
	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	return (args);
}

/**
 * free_tokens - frees a token array
 * @args: token array to free
 */
void free_tokens(char **args)
{
	free(args);
}
