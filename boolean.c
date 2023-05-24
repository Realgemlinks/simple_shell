#include "shell.h"

/**
 * is_command - checks if the cammand/file is valid/executable
 * @data: contains info about our shell
 * @file_path: path to the command/file
 * Return: 1 if true 0 if not
 */
int is_command(shell_data *data, char *file_path)
{
	struct stat sb;

	(void)data;

	/*
	 * stat is used to retrieve information about a file
	 * and stored in the sb struct
	 */
	if (!file_path || stat(file_path, &sb))
		return (0);

	/*
	 * st_mode is a field in the struct stat ds that is used to store the
	 * mode/type of a file and its permissions.
	 * S_IFREG is a macro with the flag 0100000 and used to check if a
	 * file is a regular file
	 */
	if (sb.st_mode & S_IFREG)
		return (1);

	return (0);
}


/**
 * is_interactive - checks if shell is interactive
 * @data: data struct
 *
 * Description: isatty return 1 if input is comming from stdin
 * and 0 if not. also we are also making sure arguments passed
 * from the cmd line are not above 2
 *
 * Return: 1 if in interactive
 *		0 if not
 */
int is_interactive(shell_data *data)
{
	return (isatty(STDIN_FILENO) && data->readfd <= 2);
}


/**
 * is_chain - checks if the buffer contains chain of commands
 * @data: contains info about our shell
 * @buffer: buffer of commands
 * @ptr: pointer to the current position in the buffer
 *
 * Description: chain commands are commands that contains either
 * '&&', '||' or ';'
 *
 * Return: 1 if success
 *		0 if fail
 */
int is_chain(shell_data *data, char *buffer, size_t *ptr)
{
	size_t pt = *ptr;

	if (buffer[pt] == '|' && buffer[pt + 1] == '|')
	{
		/* replace the '||' logical operator with null */
		buffer[pt] = 0;
		pt++;
		data->cmd_buf_type = CMD_OR;
	}

	else if (buffer[pt] == '&' && buffer[pt + 1] == '&')
	{
		buffer[pt] = 0;
		pt++;
		data->cmd_buf_type = CMD_AND;
	}
	else if (buffer[pt] == ';')
	{
		buffer[pt] = 0;
		data->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);

	*ptr = pt;
	return (1);
}


/**
 * is_delimiter - checks for delimiter
 * @d: chararcter to check
 * @delimiter: string to parse
 * Return: 1 if found
 *		0 if fail
 */
int is_delimiter(char d, char *delimiter)
{
	while (*delimiter)
	{
		if (*delimiter++ == d)
			return (1);
	}
	return (0);
}
