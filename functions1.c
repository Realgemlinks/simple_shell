#include "shell.h"

/**
 * dup_str - duplicates a string
 * @path: PATH string
 * @start: start index
 * @end: stop index
 * Return: buffer
 */
char *dup_str(char *path, int start, int end)
{
	static char buffer[WR_BUFFER_SIZE];
	int i = start, j = 0;

	while (i < end && j < WR_BUFFER_SIZE - 1)
	{
		if (path[i] != ':')
		{
			buffer[j] = path[i];
			j++;
		}
		i++;
	}
	buffer[j] = '\0';
	return (buffer);
}

/**
 * find_path - searches for the executable file specified by command
 * @data: contains information about our shell
 * @path: string containing the dirs to search separated by ':' PATH
 * @command: command to look for
 * Return: returns the path to the file
 */
char *find_path(shell_data *data, char *path, char *command)
{
	int j = 0;
	int cur = 0;
	char *pathstr;

	if (!path)
		return (NULL);
	/* checks if the command is in the current directory */
	if ((_strlen(command) > 2) && str_str(command, "./"))
	{
		if (is_command(data, command))
			return (command);
	}
	while (1)
	{
		/*checks whether the current char in path is a : or \0.If either of these*/
		/*conditions is true,it means that the cur dir str has ended,so we need to*/
		/*extract this substring and append the given cmd to it to form a complete*/
		/*path to the cmd file.*/

		if (path[j] == ':' || !path[j])
		{
			pathstr = dup_str(path, cur, j);
			if (!*pathstr)
				_strcat(pathstr, command);
			else
			{
				_strcat(pathstr, "/");
				_strcat(pathstr, command);
			}
			if (is_command(data, pathstr))
				return (pathstr);
			if (!path[j])
				break;
			cur = j;
		}
		j++;
	}
	return (NULL);
}



/**
 * replace_var - replaces var with val
 * @var: variable to replace
 * @val: its equivalent value
 *
 * Description: replaces variables like $?, $$ with their equivalent
 * values, exit status or $? and getpid for $$
 * Return: 1
 */
int replace_var(char **var, char *val)
{
	free(*var);
	*var = val;
	return (1);
}

/**
 * find_alias - find alias in the node if there is any
 * @node: node that contains information about alias in our program
 * @command: alias passed as command and to be searched for
 * @c: the next character after command to match
 *
 * Return: return a pointer to the node that matches the alias
 */
list_t *find_alias(list_t *node, char *command, char c)
{
	char *ptr = NULL;

	while (node)
	{
		ptr = str_str(node->data, command);
		if (ptr && ((c == -1) || (*ptr == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}
