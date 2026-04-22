#include "shell.h"

/**
 * display_prompt - prints the shell prompt in interactive mode
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}
