#include "shell.h"

/**
 * my_setenv - set environment variables
 * @data: contains infomation about our shell
 * Return: 1 if success
 *	0 if fail
 */
int my_setenv(shell_data *data)
{
	/*
	 * after a command typed by the user is parsed, argc holds the number
	 * of tokens in the command. if you passed only setenv,
	 * or setenv VARIABLE_NAME this function won't execute
	 * usage: setenv VARIABLE_NAME VALUE
	 */
	if (data->argc != 3)
	{
		eff_puts("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(data, data->argv[1], data->argv[2]))
		return (0);
	return (1);
}


/**
 * _unsetenv - this removes enviroment variable
 * @data: contains information about our shell
 * @var: environment variable to be removed
 * Return: 1 on sucess
 *	0 if failed. env_changed can only be 1 or 0
 */
int _unsetenv(shell_data *data, char *var)
{
	char *ptr;
	list_t *node = data->env;
	size_t j = 0;

	if (!node || !var)
		return (0);
	while (node)
	{
		ptr = str_str(node->data, var);
		/* if ptr is not null and first value of ptr is '='*/
		if (ptr && *ptr == '=')
		{
			/*
			 * if enviroment was successfully removed, this will on (1)
			 * this always us to make use of this information in another fuction
			 */
			data->env_changed = delete_nodeint(&(data->env), j);
			/*
			 * everything is restted, and the loop starts from the beginning
			 * this makes sure the env variable was really deleted
			 */
			j = 0;
			node = data->env;
			continue;
		}
		node = node->next;
		j++;
	}
	return (data->env_changed);
}

/**
 * setalias - this add a new alias to alias node
 * @data: contains information about our shell
 * @s: the string alias argv[i]
 * Return: 0 on success
 *	1 on failure
 */
int setalias(shell_data *data, char *s)
{
	char *ptr;

	ptr = _strchr(s, '=');
	if (!ptr)
		return (1);
	++ptr;

	/* this  checks if a user passed any val after '=' */
	if (!*ptr)
	{
		/*
		 * if the character after '=' is null (\0), or empty
		 * the function removes the alias by calling unsetalias
		 * with s as an argument.
		 */
		return (unsetalias(data, s));
	}
	/* Otherwise, it first removes any existing alias with the same name as s*/
	unsetalias(data, s);
	return (add_node_end(&(data->alias), s, 0) == NULL);

}

/**
 * unsetalias - removes a useer defined alias
 * @data: contains information about our shell
 * @s: the string alias
 * Return: 0 on success
 *	1 on failure
 */
int unsetalias(shell_data *data, char *s)
{
	int r;
	char *ptr;
	char c;

	ptr = _strchr(s, '=');
	if (!ptr)
		return (1);
	/* this saves the '=' character in c*/
	c = *ptr;
	/*
	 * this temporarily set '=' to '\0' so that when parsing the string
	 * to match a node, so it doesn't parse beyond '='
	 */
	*ptr = 0;

	r = delete_nodeint(&(data->alias),
			get_node_index(data->alias, find_alias(data->alias, s, -1)));

	/* set back ptr to '='*/
	*ptr = c;

	return (r);
}
