#include "shell.h"

void exit_cisnotfun() {
    exit(0);
}

int main() {
    char command[100];
    while (1) {
        printf("cisnotfun > ");
        fgets(command, sizeof(command), stdin);

        /* Remove newline character from the command*/
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
	  exit(0);
        } else {
	  /* Handle other commands*/

        }
    }

    return 0;
}