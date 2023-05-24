#include "shell.h"

/**
 * _getline - gets input from user
 * @data: contains information about our shell
 * @buffer: buffer to store the input
 * @len: len of the input
 *
 * Description: want to touch on little something
 * in "l = c ? 1 + (unsigned int)(c - buf) : length;" in the code below
 * arithmetic operations btw pointers yields a value of type
 * ptrdiff_t, which is also a signed integer type, we need to cast
 * (c - buf) to usigned int to make sure ther is no -ve no.
 *
 * Return: number of characters read
 */

int _getline(shell_data *data, char **buffer, size_t *len)
{
	static size_t i, length;
	static char buf[RD_BUFFER_SIZE];
	size_t nl;
	ssize_t rd = 0, buf_size = 0;
	char *ptr = NULL, *new_p = NULL, *c;

	ptr = *buffer;
	/* buffer is not NULL and len is not 0 */
	if (ptr && len)
		buf_size = *len;
	if (i == length)
		i = length = 0;

	rd = read_line(data, buf, &length);
	if (rd == -1 || (rd == 0 && length == 0))
		return (-1);
	c =  _strchr(buf + i, '\n');
	/* If a '\n' character is found, nl is set to the position of the*/
	/*'\n' + one. to account for the newline character itself.*/
	/*If a newline character is not found, nl is set to length*/
	/*(c-buf) is casted to unsigned int because pointer diff returns*/
	/*a special type called ptrdiff_t*/
	nl = c ? 1 + (unsigned int)(c - buf) : length;

	new_p = _realloc(ptr, buf_size, buf_size ? buf_size + nl : nl + 1);
	if (!new_p)
		return (ptr ? free(ptr), -1 : -1);
	if (buf_size)
		_strncat(new_p, buf + i, nl - i);
	else
		_strncpy(new_p, buf + i, nl - i + 1);
	buf_size += nl - i;
	i = nl;
	ptr = new_p;

	if (len)
		*len = buf_size;

	*buffer = ptr;
	return (buf_size);
}


/**
 * check_comments - checks for the '#' character
 * @buffer: buffer to parse for the character
 *
 * Description: it looks for a '#' character that appears
 * at the beginning of a line or after a space, and removes
 * everything from that character onwards.
 */

void check_comments(char *buffer)
{
	/*int j = 0;*/
	int j;

	/*while (buffer[j])*/
	for (j = 0; buffer[j] != '\0'; j++)
	{
		/*the if condition makes sure we are removing a comment and not a part of a*/
		/*command. So the command must come after a space*/
		/*!j makes sure the loop breaks if '#' was found at the beginning*/
		if (buffer[j] == '#' && (!j || buffer[j - 1] == ' '))
		{
			buffer[j] = '\0';
			break;
		}
	}
}

/**
 * sigint_handler - this checks for control c command
 * @sigint: constant val that gives signal interrut command
 *
 * Description: we want our code to exit with only the exit command
 * of EOF command(cntrl d)
 */
void sigint_handler(int sigint)
{
	(void)sigint;
	putstr("\n($) ");
	putchr(FLUSH);
}

/**
 * input_buf - gets the command from the user
 * @data: has all the information about our shell
 * @buffer: buffer to store the commands
 * @len: num of chars read will be stored here
 * Return: rd, num of chars sucessfully read
 */
ssize_t input_buf(shell_data *data, char **buffer, size_t *len)
{
	ssize_t rd = 0;
	size_t length = 0;

	/* if (*len == 0)*/
	if (!*len)
	{
		/*
		 * calling free(NULL) does nothing, on first call to
		 * input_buf, buf will be NULL because static char pointers
		 * are initialized to NULL
		 */
		free(*buffer);
		*buffer = NULL;
		/*SIGINT will forcefully terminate our program, leaving behind*/
		/*unfinished work or corrupting data. we dont want that*/
		signal(SIGINT, sigint_handler);

		rd = _getline(data, buffer, &length);
		if (rd > 0)
		{
			/*buffer[_strcspn(*buffer, "\n")] = '\0';*/
			if ((*buffer)[rd - 1] == '\n')
			{
				(*buffer)[rd - 1] = '\0';
				rd--;
			}
			/* if  on, count line of input */
			data->linecount_flag = 1;
			check_comments(*buffer);
			*len = rd;
			/* a pointer to the commands is stored in our data struct*/
			data->cmd_buf = buffer;
		}
	}
	return (rd);
}


/**
 * get_input - gets input from user
 * @data: contains information about our shell
 * Return: len of input read
 */
ssize_t get_input(shell_data *data)
{
	/* hold the chain of commands separated by semicolons */
	static char *buffer;
	static size_t i, j, len;
	char **buf = &(data->args), *ptr;
	ssize_t rd = 0;

	putchr(FLUSH);
	rd = input_buf(data, &buffer, &len);
	if (rd == -1)
		return (-1);
	if (len)
	{
		j = i;
		ptr = buffer + i;
		check_chain(data, buffer, &j, i, len);
		/*
		 * checks for chains (&&, || and ;)
		 * we dont want to be iterating the array, because if there
		 * was an '#' and its replaced by '\0' previously, we wouldn't
		 * get pass that so we will keep incrementing j and check for other commands
		 */
		while (j < len)
		{
			if (is_chain(data, buffer, &j))
				break;
			j++;
		}
		i = j + 1;
		if (i >= len)
		{
			len = i = 0;
			data->cmd_buf_type = CMD_NORM;
		}

		/* ptr points to the next commands */
		*buf = ptr;
		return (_strlen(ptr));
	}
	*buf = buffer;
	return (rd);
}
