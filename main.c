#include "shell.h"

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
	write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * execute_command - creates a child process and executes a command
 * @line: command entered by the user
 * @program_name: name of the shell program
 *
 * Return: 0 on success, 1 on failure
 */
int execute_command(char *line, char *program_name)
{
	pid_t pid;
	int status;
	char *argv[2];

	pid = fork();
	if (pid == -1)
	{
		perror(program_name);
		return (1);
	}

	if (pid == 0)
	{
		argv[0] = line;
		argv[1] = NULL;
		if (execve(line, argv, environ) == -1)
		{
			perror(program_name);
			exit(127);
		}
	}
	else
	{
		wait(&status);
	}

	return (0);
}

/**
 * main - simple shell entry point
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

	(void)argc;
	line = NULL;
	len = 0;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		line[strcspn(line, "\n")] = '\0';
		if (line[0] == '\0')
			continue;

		execute_command(line, argv[0]);
	}

	free(line);
	return (0);
}
