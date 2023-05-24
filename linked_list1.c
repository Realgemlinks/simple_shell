#include "shell.h"

/**
 * free_list - free a list and its nodes
 * @head_adr: address of the head node pointer
 */
void free_list(list_t **head_adr)
{
	if (!head_adr)
		return;
	while (*head_adr)
	{
		list_t *temp = *head_adr;
		*head_adr = (*head_adr)->next;
		free(temp->data);
		free(temp);
	}
}

/**
 * list_len - calculates the length of a list
 * @head: first node(pointer)
 * Return: the calculated length
 */
size_t list_len(const list_t *head)
{
	size_t j = 0;

	while (head)
	{
		head = head->next;
		j++;
	}

	return (j);
}
