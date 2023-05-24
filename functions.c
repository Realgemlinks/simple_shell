#include "shell.h"

/**
 * expand_alias - this checks if the cmd passed is an alias andtries to
 * expand it
 * @data: contains info about our shell
 * Return: 0 if success
 *		1 if fail
 */
int expand_alias(shell_data *data)
{
	int j;
	list_t *node;
	char *ptr;

	/*
	 * 10 here limits the number of alias replacements to prevent
	 * infinite loops or other unexpected behavior in cases when the
	 * alias is an alias to another command or even to the previous command
	 */
	for (j = 0; j < 10; j++)
	{
		node = find_alias(data->alias, data->argv[0], '=');
		if (!node)
			return (0);
		free(data->argv[0]);
		ptr = _strchr(node->data, '=');
		if (!ptr)
			return (0);
		ptr = _strdup(ptr + 1);
		if (!ptr)
			return (0);
		data->argv[0] = ptr;
	}
	return (1);
}


/**
 * expand_vars - replaces the variables with their equivalent values
 * @data: contains information about our shell
 * Return: 0
 */
int expand_vars(shell_data *data)
{
	int j = 0;
	list_t *node;

	for (j = 0; data->argv[j]; j++)
	{
		if (data->argv[j][0] != '$' || !data->argv[j][1])
			continue;
		if (!_strcmp(data->argv[j], "$?"))
		{
			replace_var(&(data->argv[j]), _strdup(itoa(data->status, 10, 0)));
			continue;
		}
		if (!_strcmp(data->argv[j], "$$"))
		{
			replace_var(&(data->argv[j]), _strdup(itoa(getpid(), 10, 0)));
			continue;
		}
		/*
		 * here the find alias function was use if the string immediately
		 * after '$' is an environment variable, complete list of the
		 * environ is passed from or data struct
		 */
		node = find_alias(data->env, &data->argv[j][1], '=');
		if (node)
		{
			replace_var(&(data->argv[j]), _strdup(_strchr(node->data, '=') + 1));
			continue;
		}
		replace_var(&data->argv[j], _strdup(""));
	}
	return (0);
}


/**
 * check_chain - checks if the buffer is a chain of commands
 * @data: contains information about our shell
 * @buffer: has the commands
 * @ptr: address of current position in the buffer
 * @i: starting position in the buffer
 * @len: length of buffer
 */
void check_chain(shell_data *data, char *buffer, size_t *ptr, size_t i,
		size_t len)
{
	size_t j = *ptr;

	if (data->cmd_buf_type == CMD_AND)
	{
		/*
		 * data->status contains information about the last
		 * executed command
		 */
		if (data->status)
		{
			/*
			 * terminate the current command string at the position
			 * of the semicolon or the end of the buffer.
			 */
			buffer[i] = 0;
			/*
			 * indicate that the current command has ended and the
			 * iterator should be set to the end of the buffer
			 */
			j = len;
		}
	}
	if (data->cmd_buf_type == CMD_OR)
	{
		if (!data->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}

	*ptr = j;
}



/**
 * eff_puts - calls eff_puchr with each character
 * @str: string to print
 */
void eff_puts(char *str)
{
	int j = 0;

	while (str[j])
	{
		eff_putchr(str[j]);
		j++;
	}
}

/**
 * eff_putchr - prints characters to stderr
 * @chr: character to print
 *
 * Description: Efficient putchar. this function reduces the number of write
 * system calls by istoring the characters in a buffer and printing it all
 * at once. Reducing the number of system calls helps increase efficiency and
 * performance of the program. the function uses the macro FLUSH to
 * flush the buffer and giving it signal to write out its content
 *
 * Return: 1
 */
int eff_putchr(char chr)
{
	static int j;
	static char buffer[WR_BUFFER_SIZE];

	if (chr == FLUSH || j >= WR_BUFFER_SIZE)
	{
		write(2, buffer, j);
		j = 0;
	}

	if (chr != FLUSH)
	{
		buffer[j++] = chr;
	}

	return (1);
}
