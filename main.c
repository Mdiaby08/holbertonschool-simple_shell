#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
extern char **environ;
/**
 * main - simple shell entry point
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL;
    char *cmd;
    size_t len = 0;
    pid_t pid;
    char *argv[2];
    
    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
{
    if (isatty(STDIN_FILENO))
        printf("\n");
    break;
}

        /* remove newline */
        line[strcspn(line, "\n")] = '\0';
	cmd = line;

/* skip leading spaces */
while (*cmd == ' ')
    cmd++;

/* skip empty input */
if (*cmd == '\0')
    continue;

/* trailing spaces removal */
{
    int i = strlen(cmd) - 1;

    while (i >= 0 && cmd[i] == ' ')
    {
        cmd[i] = '\0';
        i--;
    }
}

        pid = fork();

        if (pid == 0)
        {
            argv[0] = cmd;

            argv[1] = NULL;

	execve(cmd, argv, environ);

            perror("./hsh");
            exit(1);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("fork");
        }
    }

    free(line);
    return (0);
}
