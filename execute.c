#include "shell.h"

/* environ = tableau des variables d'environnement */
extern char **environ;

/**
 * print_error - prints "progname: 1: cmd: not found\n" to stderr
 * @progname: argv[0] of the shell
 * @cmd: the command that was not found
 */
static void print_error(char *progname, char *cmd)
{
	write(STDERR_FILENO, progname, strlen(progname));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, cmd, strlen(cmd));
	write(STDERR_FILENO, ": not found\n", 12);
}

/**
 * execute_command - parses and runs one command line
 * @line: the raw input line
 * @progname: argv[0] used for error messages
 *
 * Return: exit status of the command (0 on success, 127 if not found)
 */
int execute_command(char *line, char *progname)
{
	pid_t child;
	int status;
	char **args;
	char *cmd_path = NULL;

	/* 1) tokenize */
	args = split_line(line);
	if (!args || !args[0])
	{
		free(args);
		return (0);
	}

	/* 2) builtin: exit */
	if (strcmp(args[0], "exit") == 0)
	{
		free(args);
		exit(0);
	}

	/* 3) builtin: env */
	if (strcmp(args[0], "env") == 0)
	{
		int i = 0;

		while (environ[i])
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
			i++;
		}
		free(args);
		return (0);
	}

	/* 4) command with '/' — use directly */
	if (strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) != 0)
		{
			print_error(progname, args[0]);
			free(args);
			return (127);
		}
		cmd_path = args[0];
	}
	else
	{
		/* 5) search PATH */
		cmd_path = find_path(args[0]);
		if (!cmd_path)
		{
			print_error(progname, args[0]);
			free(args);
			return (127);
		}
	}

	/* 6) fork + execve */
	child = fork();
	if (child == -1)
	{
		perror("fork");
		if (cmd_path != args[0])
			free(cmd_path);
		free(args);
		return (1);
	}

	if (child == 0)
	{
		execve(cmd_path, args, environ);
		perror(progname);
		exit(1);
	}
	else
	{
		waitpid(child, &status, 0);
	}

	/* 7) cleanup */
	if (cmd_path != args[0])
		free(cmd_path);
	free(args);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
