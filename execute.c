#include "shell.h"

/**
 * print_error - writes "prog: 1: cmd: not found\n" to stderr
 * @progname: the shell's argv[0]
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
 * handle_builtin - checks and runs built-in commands
 * @args: tokenized argument array
 *
 * Return: 1 if builtin handled, 0 otherwise
 */
static int handle_builtin(char **args)
{
	int i;

	if (strcmp(args[0], "exit") == 0)
	{
		free(args);
		exit(0);
	}
	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		free(args);
		return (1);
	}
	return (0);
}

/**
 * resolve_cmd - resolves the full path of a command
 * @args: tokenized argument array
 * @progname: shell argv[0] for error messages
 *
 * Return: path string, or NULL if not found
 */
static char *resolve_cmd(char **args, char *progname)
{
	char *cmd_path;

	if (strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) != 0)
		{
			print_error(progname, args[0]);
			return (NULL);
		}
		return (args[0]);
	}
	cmd_path = find_path(args[0]);
	if (!cmd_path)
		print_error(progname, args[0]);
	return (cmd_path);
}

/**
 * fork_and_exec - forks and executes a command
 * @cmd_path: full path to the executable
 * @args: argument array for execve
 * @progname: shell argv[0] for error messages
 *
 * Return: exit status of child process
 */
static int fork_and_exec(char *cmd_path, char **args, char *progname)
{
	pid_t child;
	int status;

	child = fork();
	if (child == -1)
	{
		perror("fork");
		return (1);
	}
	if (child == 0)
	{
		execve(cmd_path, args, environ);
		perror(progname);
		exit(1);
	}
	waitpid(child, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * execute_command - tokenizes and dispatches one command line
 * @line: raw input line
 * @progname: shell argv[0] for error messages
 *
 * Return: exit status of the executed command
 */
int execute_command(char *line, char *progname)
{
	char **args;
	char *cmd_path;
	int ret;

	args = split_line(line);
	if (!args || !args[0])
	{
		free(args);
		return (0);
	}
	if (handle_builtin(args))
		return (0);
	cmd_path = resolve_cmd(args, progname);
	if (!cmd_path)
	{
		free(args);
		return (127);
	}
	ret = fork_and_exec(cmd_path, args, progname);
	if (cmd_path != args[0])
		free(cmd_path);
	free(args);
	return (ret);
}
