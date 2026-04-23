#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

void display_prompt(void);
ssize_t read_line(char **line, size_t *len);
void remove_newline(char *line);
char **split_line(char *line);
void free_tokens(char **args);
char *get_path_value(void);
char *build_path(char *dir, char *command);
char *find_command(char *command);
int execute_command(char *line, char *program_name, unsigned int count);

#endif /* SHELL_H */
