#include "shell.h"


/**
 * fork_cmd - calls the child process and execute the command
 * @data: contains information about shell
 */
void fork_cmd(shell_data *data)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* generates automatic error msg of what caused the fail */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(data->path, data->argv, get_environ(data)) == -1)
		{
			free_data(data, 1);
			if (errno == EACCES)
				exit(126);/* doesn't have "execute" permission for the file*/
			exit(1);
		}
	}
	else
	{
		wait(&(data->status));
		/*WIFEXITED() is a macro that checks whether the chld proc exited normally*/
		if (WIFEXITED(data->status))
		{
			/*WEXITSTATUS gets the exit status of the child proc and ret it as int.*/
			data->status = WEXITSTATUS(data->status);
			if (data->status == 126)
				print_err(data, "Permission denied\n");
		}
	}
}

/**
 * find_command - find a command in the PATH
 * @data: contains information about our shell
 */
void find_command(shell_data *data)
{
	int i, j;
	char *file_path = NULL;

	data->path = data->argv[0];
	 /* this flag was set in input_buf() and indicates*/
	 /* a line/input has been read and its valid*/
	if (data->linecount_flag == 1)
	{
		data->linecount++;
		data->linecount_flag = 0;
	}
	for (i = 0, j = 0; data->args[i]; i++)
	{
		/*Check if there are any args other than whitespace characters(" \t\n")*/
		/* If there aren't any, return without doing anything.*/
		if (!is_delimiter(data->args[i], " \t\n"))
			j++;
	}
	if (!j)
		return;
	/* this checks if the path exists from the env PATH variable*/
	file_path = find_path(data, _getenv(data, "PATH="), data->argv[0]);

	if (file_path)
	{
		data->path = file_path;
		fork_cmd(data);
	}
	else
	{
		if ((is_interactive(data) || _getenv(data, "PATH=")
			|| data->argv[0][0] == '/') && is_command(data, data->argv[0]))
			fork_cmd(data);
		else if (*(data->args) != '\n')
		{
			/* saves the exit status. 127 means command not found */
			data->status = 127;
			print_err(data, "not found\n");
		}
	}
}


/**
 * prompt - this gives the prompt to enter input
 * @data: to keep track of general data of the program
 * @argv: arguments from the main()
 * Return: 0 on success
 *	1 on error
 */

int prompt(shell_data *data, char **argv)
{
	ssize_t rd = 0;
	int builtin = 0;
	/* if there was an error reading the input from the user(-1)*/
	/* or the user typed 'exit'(-2) the loop will stop*/
	while (rd != -1 && builtin != -2)
	{
		clear_data(data);
		if (is_interactive(data))
		{
			/*write(STDIN_FILENO, "($) ", 4);*/
			putstr("($) ");
		} /*eff_putchr: this writes to the stderr any err*/
		eff_putchr(FLUSH);/*that occured previously b4 printing the new prompt*/
		rd = get_input(data);/*wondering how the new $ will be printed? checkout*/
		if (rd != -1)/*get_input(). putchr(FLUSH): this will print ($) again*/
		{
			set_data(data, argv);
			builtin = check_builtin(data);
			if (builtin == -1)
				find_command(data);
		}
		else if (is_interactive(data))
			putchr('\n');
		free_data(data, 0);
	}
	free_data(data, 1);
	if (!is_interactive(data) && data->status)
		exit(data->status);
	/* this tends to true when exit is called with no status code*/
	/* -2 is just a signal we choosed*/
	if (builtin == -2)
	{
		/* if exit status was passed, err_num in our datastruct will have the value*/
		/* if not, data->status is 0 by default*/
		if (data->err_num == -1)
			exit(data->status);
		exit(data->err_num);
	}
	return (builtin);
}
