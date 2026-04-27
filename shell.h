#ifndef SHELL_H
#define SHELL_H

/* standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ; /* global array of "NAME=value" environment strings */

/* utils.c */
void	remove_newline(char *line);  /* replaces trailing '\n' with '\0' */
char	**split_line(char *line);    /* splits line into a word array (NULL-terminated) */

/* execute.c */
int	execute_command(char *line); /* tokenizes, resolves path, forks and runs */

/* path.c */
char	*find_path(char *command);   /* searches each PATH dir for the command */
char	*get_path_value(void);       /* returns the value of PATH from environ */
char	*build_path(char *dir, char *command); /* builds the string "dir/command" */

#endif /* SHELL_H */
