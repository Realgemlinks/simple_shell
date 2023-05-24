#include "shell.h"

/**
 * list_to_strings - converts a list to a string
 * @head: pointer to the head node
 *
 * Description: This function takes a linked list of strings and
 * returns an array of strings that contains all the strings/data
 * from the linked list.
 * Return: array of strings of the list->data
 */
char **list_to_strings(list_t *head)
{
	/*calculates the number of strings in the linked list*/
	list_t *node = head;
	size_t i = list_len(head), j;
	char **string_array;
	char *string_copy;

	if (!head || !i)
		return (NULL);
	string_array = malloc(sizeof(char *) * (i + 1));
	if (!string_array)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		string_copy = malloc(_strlen(node->data) + 1);
		if (!string_copy)
		{
			for (j = 0; j < i; j++)
				free(string_array[j]);
			free(string_array);
			return (NULL);
		}
		/*pointer to the new string*/
		string_copy = _strcpy(string_copy, node->data);
		string_array[i] = string_copy;
	}
	string_array[i] = NULL;
	return (string_array);
}

/**
 * get_node_index - gets the index of a particular node
 * @head: pointer to the head of the node
 * @node: node to return its index
 * Return: 1 if success
 *	-1 is fail
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t j = 0;

	while (head)
	{
		/*
		 * pointer comparison
		 * here it checks if head and node are pointing to the same type of data.
		 */
		if (head == node)
			return (j);
		head = head->next;
		j++;
	}
	return (-1);
}

/**
 * delete_nodeint - deletes node at a particular index
 * @head: ponter to start of the node
 * @indx: index to be deleted
 * Return: 0 on sucess
 *		1 on delete
 */
int delete_nodeint(list_t **head, unsigned int indx)
{
	list_t *prev, *node;
	unsigned int j = 0;

	if (!head || !*head)
		return (0);

	/* if index is 0 this deletes the first node*/
	if (!indx)
	{
		node = *head;
		*head = (*head)->next;
		free(node->data);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (j == indx)
		{
			/*
			 * this assigns adress of the node after our node to the node before it
			 * so other nodes after our wont be deleted too
			 */
			prev->next = node->next;
			free(node->data);
			free(node);
			return (1);
		}
		j++;
		prev = node;
		node = node->next;
	}
	return (0);
}


/**
 * print_list_data - prints the  data part of the list
 * @node: poiter to the head(first node)
 * Return: size of the list
 */
size_t print_list_data(const list_t *node)
{
	size_t j = 0;

	while (node)
	{
		putstr(node->data ? node->data : "(nil)");
		putstr("\n");
		node = node->next;
		j++;
	}
	return (j);
}


/**
 * add_node_end - add to node the end of the list
 * @head: head of the list
 * @str: str parameter of the list_t struct
 * @num: num parameter of the list_t struct
 * Return: pointer to the new struct
 */

list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new, *node;

	if (!head)
		return (NULL);

	node = *head;
	new = malloc(sizeof(list_t));
	if (!new)
		return (NULL);
	/*
	 * here _memset is used to set all the bytes of the new_node struct to zero.
	 * In C, the value 0 is often used as a null or empty value.
	 * When used to initialize a struct, it will be implicitly cast to
	 * the appropriate type based on the field's declaration.
	 */
	_memset((void *)new, 0, sizeof(list_t));
	new->num = num;
	if (str)
	{
		/* If you do this new->data = str, new->data will point to the original str*/
		/* strdup ensures that a new copy of the string is created in memory,*/
		/* which can be safely manipulated without affecting the original string.*/
		new->data = _strdup(str);
		if (!new->data)
		{
			free(new);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new;
	}
	else
		*head = new;
	return (new);
}

/*
 * list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *ptr, *new;

	new = malloc(sizeof(list_t));
	if (new == NULL)
		return (NULL);

	_memset((void *)new, 0, sizeof(list_t));

	new->num = num;
	new->data = _strdup(str);
	if (new->data == NULL)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;



	if (*head == NULL)
		*head = new;
	else
	{
		ptr = *head;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = new;
	}
	return (new);
}
