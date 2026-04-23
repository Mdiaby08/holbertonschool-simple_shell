#ifndef SHELL_H
#define SHELL_H

/* ------------------ LIBRARIES ------------------ */
#include <stdio.h>      /* getline, printf */
#include <stdlib.h>     /* malloc, free, exit */
#include <unistd.h>     /* write, fork, execve, access */
#include <string.h>     /* strtok, strcmp, strlen */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* wait */

/* ------------------ PROTOTYPES ------------------ */

/* lire et nettoyer la ligne */
void remove_newline(char *line);

/* découper la ligne en mots */
char **split_line(char *line);

/* exécuter une commande (exit, env, PATH, fork...) */
void execute_command(char *line);

/* chercher une commande dans PATH */
char *find_path(char *command);

/* récupérer PATH */
char *get_path_value(void);

/* construire un chemin complet */
char *build_path(char *dir, char *command);

#endif
