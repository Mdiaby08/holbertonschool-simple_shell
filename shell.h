#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void display_prompt(void);
char *read_line(void);
void remove_newline(char *line);
void execute_command(char *line);
void trim_spaces(char *line);

#endif
