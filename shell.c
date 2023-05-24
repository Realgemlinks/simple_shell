#include "shell.h"

/**
 * main - Entry point of the shell program
 *
 * Return: 0 on success
 */
int main(void)
{
    char *line;
    char **args;
    int status;

    do {
        prompt();  // Display the shell prompt
        line = _getline();  // Read a line of input from the user
        args = parse_cmd(line);  // Parse the input line into arguments

        status = execute_cmd(args);  // Execute the command

        free(line);
        free(args);
    } while (status);

    return 0;
}

/**
 * execute_cmd - Execute the command provided by the user
 * @args: Array of command arguments
 *
 * Return: 1 to continue shell loop, 0 to exit shell
 */
int execute_cmd(char **args)
{
    if (args[0] == NULL) {
        // Empty command, continue shell loop
        return 1;
    }

    if (check_builtin(args)) {
        // Built-in command, handle it
        return handle_builtin(args);
    }

    // Non-built-in command, execute it using fork and exec
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("fork");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
