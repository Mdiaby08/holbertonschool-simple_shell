#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - simple shell entry point
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    pid_t pid;
    char *argv[2];

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        /* remove newline */
        line[strcspn(line, "\n")] = '\0';

        pid = fork();

        if (pid == 0)
        {
            argv[0] = line;
            argv[1] = NULL;

            execve(line, argv, NULL);

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
