#include "shell.h"

/**
 * printalias - print all user defined alias in shell
 * @node: node of the alias to print
 * Return: 1 on success
 *	0 on fail
 */
int printalias(list_t *node)
{
	char *ptr = NULL;
	char *b = NULL;

	if (node)
	{
		ptr = _strchr(node->data, '=');

		/*
		 * here the memory address of the two pointers are been
		 * compared
		 */
		for (b = node->data; b <= ptr; b++)
			putchr(*b);
		putchr('\'');
		putstr(ptr + 1);
		putstr("'\n");
		return (0);
	}
	return (1);
}


/**
 * my_alias - this handles the alias builtin call
 * @data: contains infomation about shell
 * Return: Always 0
 */
int my_alias(shell_data *data)
{
	int j = 0;
	char *ptr = NULL;
	list_t *node = NULL;

	if (data->argc == 1)
	{
		node = data->alias;
		while (node)
		{
			printalias(node);
			node = node->next;
		}
		return (0);
	}
	for (j = 1; data->argv[j]; j++)
	{
		ptr = _strchr(data->argv[j], '=');
		if (ptr)
			setalias(data, data->argv[j]);
		else
			printalias(find_alias(data->alias, data->argv[j], '='));
	}

	return (0);
}

/**
 * my_exit - Handles the exit builtin call
 * @data: contains information about our shell(shell_data)
 * Return: -2 if exit was passed solo or with a valid number
 *		1 if not a valid number/status
 */
int my_exit(shell_data *data)
{
	int status_chk;
	/*
	 * this parameter is used to check if the exit was passed with a
	 * status/argument
	 */

	if (data->argv[1])
	{
		/*erratoi converts the status passed with the exit call if any to an int*/
		status_chk = erratoi(data->argv[1]);
		if (status_chk == -1)
		{
			data->status = 2;
			/*
			 * the range of valid exit status values is between 0 and 255
			 *  A value outside of this range is considered an illegal
			 *  or invalid exit status
			 */
			print_err(data, "Illegal number: ");
			eff_puts(data->argv[1]);
			eff_putchr('\n');
			return (1);
		}
		data->err_num = erratoi(data->argv[1]);
		return (-2);
	}
	data->err_num = -1;
	return (-2);
}

/**
 * my_cd - handles the change directory builtin
 * @data: contains information about our shell
 * Return: Always 0
 */
int my_cd(shell_data *data)
{
	char *cwd, *new_dir, buf[WR_BUFFER_SIZE];
	int ch_dir;
	/* returns a pointer to buf if it successfully retrieves the cwd */
	cwd = getcwd(buf, WR_BUFFER_SIZE);

	if (!data->argv[1])
	{
		new_dir = _getenv(data, "HOME=");
		if (!new_dir)
			ch_dir = chdir((new_dir = _getenv(data, "PWD=")) ? new_dir :  "/");
		else
			ch_dir = chdir(new_dir);
	}
	else if (_strcmp(data->argv[1], "-") == 0)
	{
		if (!_getenv(data, "OLDPWD="))
		{
			putstr(cwd);
			putchr('\n');
			return (1);
		}
		putstr(_getenv(data, "OLDPWD="));
		putchr('\n');
		ch_dir = chdir((new_dir = _getenv(data, "OLDPWD=")) ? new_dir : "/");
	}
	else
		ch_dir = chdir(data->argv[1]);
	if (ch_dir == -1)
	{
		print_err(data, "can't cd to ");
		eff_puts(data->argv[1]), eff_putchr('\n');
	}
	else
	{
		_setenv(data, "OLDPWD", _getenv(data, "PWD="));
		_setenv(data, "PWD", getcwd(buf, WR_BUFFER_SIZE));
	}
	return (0);
}





/**
 * check_builtin - this checks if the command passed is a shell builtin
 * @data: contains information about our shell environment
 * Return: 0 on success
 *		1 on fail
 */
int check_builtin(shell_data *data)
{
	int j;
	int builtin_ret = -1;
	builtin_t arr[] = {
		{"cd", my_cd},
		{"env", my_env},
		{"exit", my_exit},
		{"alias", my_alias},
		{"setenv", my_setenv},
		{"unsetenv", my_unsetenv},
		{NULL, NULL}
	};

	for (j = 0; arr[j].type; j++)
	{
		if (_strcmp(data->argv[0], arr[j].type) == 0)
		{
			data->linecount++;
			builtin_ret = arr[j].func(data);
			break;
		}

	}
	return (builtin_ret);
}
