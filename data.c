#include "shell.h"

/**
 * free_cmd_buf - free the buffer used to store the commands
 * @ptr: ptr to the buffer
 * Return: 1 if success
 *	0 if not
 */
int free_cmd_buf(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * free_str_array - frees pointer to pointer(array of strings)
 * @argv: array of strings
 */
void free_str_array(char **argv)
{
	char **curr = argv;

	if (argv == NULL)
		return;

	while (*curr != NULL)
	{
		free(*curr);
		*curr = NULL;
		curr++;
	}
	free(argv);
}

/**
 * free_data - this frees all the memory allocated in our shell
 * @data: contains information about our shell
 * @all: True(1) if all fields should be freed
 */
void free_data(shell_data *data, int all)
{
	free_str_array(data->argv);
	data->argv = NULL;
	data->path = NULL;
	if (all)
	{
		if (!data->cmd_buf)
			free(data->args);
		if (data->env)
			free_list(&(data->env));
		if (data->alias)
			free_list(&(data->alias));
		free_str_array(data->environ);
			data->environ = NULL;
		/* cmd_buf holds a pointer to buffer that was used to store the commands*/
		free_cmd_buf((void **)data->cmd_buf);
		if (data->readfd > 2)
			close(data->readfd);
		putchr(FLUSH);
	}
}

/**
 * clear_data - reset the data struct
 * @data: ponter to the data struct
 */
void clear_data(shell_data *data)
{
	data->argv = NULL;
	data->path = NULL;
	data->args = NULL;
	data->argc = 0;
}

/**
 * set_data - sets the data in our struct parameter
 * @data: contains information about our shell
 * @argv: command line arguments
 */
void set_data(shell_data *data, char **argv)
{
	int j = 0;

	data->fname = argv[0];
	if (data->args)
	{
		/* here we are tokenizing the commands with the space and tab delimiter */
		data->argv = _strtok(data->args, " \t");
		if (!data->argv)
		{
			data->argv =  malloc(sizeof(char *) * 2);
			if (data->argv)
			{
				data->argv[0] = _strdup(data->args);
				data->argv[1] = NULL;
			}
		}
		for (j = 0; data->argv && data->argv[j]; j++)
			;
		data->argc = j;

		expand_alias(data);
		expand_vars(data);
	}
}
