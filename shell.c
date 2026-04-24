#include "shell.h"

/**
 * main - boucle principale du shell : affiche le prompt,
 *        lit une commande, l'exécute, recommence.
 * @argc: non utilisé
 * @argv: argv[0] sert de nom du shell dans les messages d'erreur
 * Return: statut de la dernière commande exécutée
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int last_status = 0;

	(void)argc;

	while (1)
	{
		display_prompt();

		nread = read_line(&line, &len); /* -1 = EOF → on quitte */
		if (nread == -1)
		{
			free(line);
			return (last_status);
		}

		last_status = execute_command(line, argv[0]);
	}

	free(line);
	return (last_status);
}
