#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * prompt - Implements the prompt functionality
 * @av: Array of command line arguments
 * @env: Array of environment variables
 */
void prompt(char **av, char **env)
{
	/* Implementation of the prompt function */
}

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
	/* Check if the program is run without any command line arguments */
	if (ac == 1)
	{
		/* Call the prompt function */
		prompt(av, env);
	}

	return (0);
}
