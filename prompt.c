#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND 10

void prompt(char **av, char **env)
{
    char *string = NULL;
    int i, j, status;
    size_t n = 0;
    ssize_t num_char;
    char *argv[MAX_COMMAND];
    pid_t baby_pid;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("simple shell project ");

        num_char = getline(&string, &n, stdin);

        if (num_char == -1)
        {
            free(string);
            exit(EXIT_FAILURE);
        }

        i = 0;
        while (string[i])
        {
            if (string[i] == '\n')
                string[i] = '\0';
            i++;
        }

        j = 0;
        argv[j] = strtok(string, " ");
        while (argv[j])
        {
            argv[++j] = strtok(NULL, " ");
        }
        baby_pid = fork();

        if (baby_pid == -1)
        {
            free(string);
            exit(EXIT_FAILURE);
        }
        else if (baby_pid == 0)
        {
            if (execve(argv[0], argv, env) == -1)
                printf("%s: No file or directory found\n", av[0]);
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);
        }
    }
}
