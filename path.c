#include "shell.h"

/**
 * duplicate_string - creates a copy of a string
 * @str: string to copy
 *
 * Return: allocated copy of str, or NULL on failure
 */
static char *duplicate_string(char *str)
{
	char *copy;

	copy = malloc(strlen(str) + 1);
	if (copy == NULL)
		return (NULL);

	strcpy(copy, str);
	return (copy);
}

/**
 * get_path_value - gets the PATH value from the environment
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
 * build_path - builds a full path from a directory and command
 * @dir: directory part
 * @command: command name
 *
 * Return: allocated full path, or NULL on failure
 */
char *build_path(char *dir, char *command)
{
	char *full_path;
	size_t len;

	len = strlen(dir) + strlen(command) + 2;
	full_path = malloc(sizeof(char) * len);
	if (full_path == NULL)
		return (NULL);

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, command);

	return (full_path);
}

/**
 * find_command - resolves a command path using PATH if needed
 * @command: command entered by the user
 *
 * Return: allocated resolved path, or NULL if not found
 */
char *find_command(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;

	if (command == NULL || command[0] == '\0')
		return (NULL);

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
			return (duplicate_string(command));
		return (NULL);
	}

	path_env = get_path_value();
	if (path_env == NULL || path_env[0] == '\0')
		return (NULL);

	path_copy = duplicate_string(path_env);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = build_path(dir, command);
		if (full_path != NULL && access(full_path, X_OK) == 0)
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
