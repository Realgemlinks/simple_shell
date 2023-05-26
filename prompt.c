#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

#define MAX_COMMAND_LENGTH 100

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		printf("cisnotfun$ ");
}

/**
 * execute_command - Executes the given command in a new process
 * @command: The command to execute
 */
void execute_command(char *command)
{
	pid_t son_pid = fork();


	if (son_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (son_pid == 0)
	{
		/* Son process */
		char *args[2];
		args[0] = command;
		args[1] = NULL;
		execvp(command, args);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		int status;
		waitpid(son_pid, &status, 0);
	}
}

/**
 * print_environment - Prints the current environment
 */
void print_environment(void)
{
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * main - Entry point of the program
 *
 * Return: Always 0 (Success)
 */

{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();

		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			printf("\n");
			break; /* EOF encountered */
		}

		/* Remove newline character from the command */
		command[strcspn(command, "\n")] = '\0';

		if (strlen(command) == 0)
		{
			continue; /* Empty command, prompt again */
		}

		if (strcmp(command, "exit") == 0)
		{
			break; /* Exit the shell */
		}
		else if (strcmp(command, "env") == 0)
		{
			print_environment(); /* Print the current environment */
		}
		else
		{
			execute_command(command);
		}
	}

	return (0);
}
