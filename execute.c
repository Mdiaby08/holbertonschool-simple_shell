#include "shell.h"

extern char **environ;

/**
 * execute_command - core execution function.
 *                   Tokenizes the line, handles built-in "env",
 *                   resolves the executable path, then fork+execve.
 * @line: raw input line (e.g. "ls -l /tmp")
 * Return: exit status of the command, 127 if not found, 0 for built-ins
 */
int execute_command(char *line)
{
	pid_t child;
	int status = 0;
	char **args;
	char *cmd_path = NULL;
	int i = 0;

	args = split_line(line); /* ["ls", "-l", "/tmp", NULL] */
	if (!args || !args[0])
	{
		free(args);
		return (0); /* empty line: nothing to do */
	}

	/* built-in env: print all environment variables, no fork needed */
	if (strcmp(args[0], "env") == 0)
	{
		while (environ[i])
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
			i++;
		}
		free(args);
		return (0);
	}

	/* resolve the executable path */
	if (strchr(args[0], '/')) /* absolute or relative path given directly */
	{
		if (access(args[0], X_OK) != 0) /* X_OK: is it executable? */
		{
			write(STDERR_FILENO, "./hsh: 1: ", 10);
			write(STDERR_FILENO, args[0], strlen(args[0]));
			write(STDERR_FILENO, ": not found\n", 12);
			free(args);
			return (127);
		}
		cmd_path = args[0]; /* path is valid, use it directly */
	}
	else
	{
		cmd_path = find_path(args[0]); /* search each directory in PATH */
		if (!cmd_path)
		{
			write(STDERR_FILENO, "./hsh: 1: ", 10);
			write(STDERR_FILENO, args[0], strlen(args[0]));
			write(STDERR_FILENO, ": not found\n", 12);
			free(args);
			return (127);
		}
	}

	child = fork(); /* duplicate the current process */
	if (child == -1)
	{
		perror("fork");
		if (cmd_path != args[0])
			free(cmd_path);
		free(args);
		return (1);
	}

	if (child == 0) /* child process: replace image with the command */
	{
		execve(cmd_path, args, environ);
		perror("./hsh"); /* execve only returns on failure */
		exit(1);
	}
	else /* parent process: wait for child to finish */
	{
		waitpid(child, &status, 0);
	}

	if (cmd_path != args[0]) /* find_path allocated it: must free */
		free(cmd_path);
	free(args);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status)); /* extract child's exit code (0-255) */
	return (1);
}
