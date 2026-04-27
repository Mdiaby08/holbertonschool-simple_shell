#include "shell.h"

extern char **environ;

/**
 * duplicate_string - copies a string into a new allocated buffer.
 *                    Replacement for strdup (not allowed by Holberton).
 * @str: string to copy
 * Return: new allocated copy, NULL if malloc fails
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
 * get_path_value - scans environ[] for "PATH=..." and returns its value.
 *                  environ entries have the format "NAME=value".
 * Return: pointer to the value part of PATH, NULL if not set
 */
char *get_path_value(void)
{
	int i;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5); /* skip "PATH=" prefix */
	}
	return (NULL);
}

/**
 * build_path - allocates and builds the string "dir/command".
 * @dir: directory (e.g. "/usr/bin")
 * @command: command name (e.g. "ls")
 * Return: new string "/usr/bin/ls", NULL if malloc fails
 */
char *build_path(char *dir, char *command)
{
	size_t len = strlen(dir) + strlen(command) + 2; /* +2: '/' and '\0' */
	char *full = malloc(len);

	if (!full)
		return (NULL);
	strcpy(full, dir);
	strcat(full, "/");
	strcat(full, command);
	return (full);
}

/**
 * find_path - searches each directory in PATH for an executable command.
 *             access(X_OK) checks the file exists and is executable.
 *             strtok works on a copy since it modifies the string.
 * @command: command name to locate (e.g. "ls")
 * Return: allocated full path on success, NULL if not found
 */
char *find_path(char *command)
{
	char *path_env = get_path_value();
	char *copy, *dir, *full;

	if (!path_env)
		return (NULL);

	copy = duplicate_string(path_env); /* copy: strtok will modify it */
	if (!copy)
		return (NULL);

	dir = strtok(copy, ":");
	while (dir)
	{
		full = build_path(dir, command);
		if (full && access(full, X_OK) == 0) /* found and executable */
		{
			free(copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":"); /* move to next directory */
	}

	free(copy);
	return (NULL); /* command not found in any PATH directory */
}
