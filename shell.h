#ifndef SHELL_H
#define SHELL_H

/* ------------------ LIBRARIES ------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* ------------------ PROTOTYPES ------------------ */
extern char **environ;

void remove_newline(char *line);
char **split_line(char *line);
int execute_command(char *line, char *progname);
char *find_path(char *command);
char *get_path_value(void);
char *build_path(char *dir, char *command);

#endif
