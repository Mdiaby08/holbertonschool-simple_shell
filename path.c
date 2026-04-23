#include "shell.h"

/**
 * duplicate_string - duplicates a string
 * @str: string to copy
 *
 * Return: pointer to new copy, or NULL on failure
 */
static char *duplicate_string(char *str)
{
	char *copy = malloc(strlen(str) + 1);

	if (!copy)
		return (NULL);
	strcpy(copy, str);
	return (copy);
}

/**
 * get_path_value - returns the value of PATH from environ
 *
 * Return: pointer to PATH value, or NULL if not found
 */
char *get_path_value(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}
	return (NULL);
}

/**
 * build_path - builds a full path from dir and command
 * @dir: directory component
 * @command: command name
 *
 * Return: allocated string "dir/command", or NULL on failure
 */
char *build_path(char *dir, char *command)
{
	size_t len = strlen(dir) + strlen(command) + 2;
	char *full_path = malloc(len);

	if (!full_path)
		return (NULL);
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, command);
	return (full_path);
}

/**
 * find_path - finds the full path of a command by searching PATH
 * @command: command name to look for
 *
 * Return: allocated full path string, or NULL if not found
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;

	if (!command || command[0] == '\0')
		return (NULL);
	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (duplicate_string(command));
		return (NULL);
	}
	path_env = get_path_value();
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	path_copy = duplicate_string(path_env);
	if (!path_copy)
		return (NULL);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = build_path(dir, command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
