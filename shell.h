#ifndef SHELL_H
#define SHELL_H

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void prompt(char **av, char **env);
int main(void);
void display_prompt(void);
void execute_command(char *command);
void print_environment(void);

#endif /* SHELL_H */
