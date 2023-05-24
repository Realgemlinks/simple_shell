#include "shell.h"

/**
 * main - Entry point of the program
 * @ac: Number of command line arguments
 * @av: Array of command line arguments
 * @env: Array of environment variables
 *
 * Return: Always 0 (Success)
 */
int main(int ac, char **av, char **env)
{
	if (ac == 1)
		prompt(av, env);

	return (0);
}
