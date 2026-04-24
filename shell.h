#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* tableau global des variables d'environnement */
extern char **environ;

/* utils.c */
void remove_newline(char *line);      /* retire le '\n' final */
char **split_line(char *line);        /* découpe la ligne en mots */

/* execute.c */
void execute_command(char *line);     /* exécute une commande */

/* path.c */
char *find_path(char *command);       /* cherche la commande dans PATH */
char *get_path_value(void);           /* récupère la valeur de PATH */
char *build_path(char *dir, char *command); /* construit "dir/commande" */

#endif /* SHELL_H */

