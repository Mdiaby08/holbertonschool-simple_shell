#include "shell.h"

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
	int len;

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
	char *path_env, *path_copy, *dir, *full_path, *cmd_copy;

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
		{
			cmd_copy = malloc(strlen(command) + 1);
			if (cmd_copy == NULL)
				return (NULL);
			strcpy(cmd_copy, command);
			return (cmd_copy);
		}
		return (NULL);
	}

	path_env = get_path_value();
	if (path_env == NULL)
		return (NULL);

	path_copy = malloc(strlen(path_env) + 1);
	if (path_copy == NULL)
		return (NULL);

	strcpy(path_copy, path_env);
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
