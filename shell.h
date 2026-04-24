#ifndef SHELL_H
#define SHELL_H
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
 
extern char **environ; /* tableau global des variables d'environnement */
 
/* utils.c */
void		remove_newline(char *line);  /* retire le '\n' final */
char		**split_line(char *line);    /* découpe la ligne en mots */
 
/* read_line.c */
ssize_t		read_line(char **line, size_t *len); /* lit une ligne depuis stdin */
 
/* prompt.c */
void		display_prompt(void); /* affiche "$ " si mode interactif */
 
/* execute.c */
int		execute_command(char *line, char *progname); /* lance une commande */
 
/* path.c */
char		*find_path(char *command);   /* cherche la commande dans PATH */
char		*get_path_value(void);       /* retourne la valeur de PATH */
char		*build_path(char *dir, char *command); /* assemble "dir/commande" */
 
#endif /* SHELL_H */