#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/* utils.c */
void remove_newline(char *line);
char **split_line(char *line);

/* execute.c */
int execute_command(char *line);

/* path.c */
char *find_path(char *command);
char *get_path_value(void);
char *build_path(char *dir, char *command);

#endif
