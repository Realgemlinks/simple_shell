#include "shell.h"

/**
 * _myenv - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}
/**
 * _setenv - updates and env or add a new one
 * @data: contains information about our shell
 * @var: environmental variables
 * @val: value to update
 * Return: 0 on success
 * 1 on failure
 */
int _setenv(shell_data *data, char *var, char *val)
{
	char *buffer = NULL, *ptr;
	list_t *node;

	if (!var || !val)
		return (0);

	/* 2 is added to account for the '=' and the null terminator (\0)*/
	buffer = malloc(_strlen(var) + _strlen(val) + 2);
	if (!buffer)
		return (1);
	_strcpy(buffer, var);
	_strcat(buffer, "=");
	_strcat(buffer, val);
	node = data->env;

	while (node)
	{
		ptr = str_str(node->data, var);
		if (ptr && *ptr == '=')
		{
			free(node->data);
			node->data = buffer;
			data->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(data->env), buffer, 0);
	free(buffer);
	data->env_changed = 1;
	return (0);
}
/**
 * set_env_list - saves the parent enviroment
 * @data: data->env points to the node head
 * Return: 0
 */
int set_env_list(shell_data *data)
{
	size_t j;
	list_t *node = NULL;

	for (j = 0; environ[j]; j++)
	{
		add_node_end(&node, environ[j], 0);
	}

	data->env = node;
	return (0);
}

