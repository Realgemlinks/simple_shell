#ifndef SHELL_H
#define SHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

void prompt(char **av);

/* Other function declarations */

int main(int argc, char *argv[]);
void display_prompt(void);
void execute_command(char *command);
void print_environment(void);

#endif /* SHELL_H */
