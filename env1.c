#include "shell.h"

/**
 * get_environ - gets the whole environment variables
 * @data: contains information about shell
 * Return: array containing the env variables
 */
char **get_environ(shell_data *data)
{
	if (!data->environ || data->env_changed)
	{
		data->environ = list_to_strings(data->env);
		data->env_changed = 0;
	}

	return (data->environ);
}
