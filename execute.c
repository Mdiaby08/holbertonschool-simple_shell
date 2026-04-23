#include "shell.h"

/**
 * execute_command - forks and executes a command if it exists
 * @line: command line entered by the user
 * @program_name: shell program name for error reporting
 * @count: command counter
 *
 * Return: command exit status
 */
int execute_command(char *line, char *program_name, unsigned int count)
{
	pid_t child;
	int status;
	char **args;
	char *command_path;

	args = split_line(line);
	if (args == NULL || args[0] == NULL)
	{
		free_tokens(args);
		return (0);
	}

	command_path = find_command(args[0]);
	if (command_path == NULL)
	{
		fprintf(stderr, "%s: %u: %s: not found\n",
			program_name, count, args[0]);
		free_tokens(args);
		return (127);
	}

	child = fork();
	if (child == -1)
	{
		perror(program_name);
		free(command_path);
		free_tokens(args);
		return (1);
	}

	if (child == 0)
	{
		if (execve(command_path, args, environ) == -1)
		{
			perror(program_name);
			free(command_path);
			free_tokens(args);
			exit(127);
		}
	}
	else
	{
		wait(&status);
	}

	free(command_path);
	free_tokens(args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
