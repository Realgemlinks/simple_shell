#include "shell.h"

/**
 * _strlen - returns the length of a string
 * @s: the string whose length to check
 *
 * Return: integer length of string
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	while (*s++)
		i++;
	return (i);
}
/**
 * _strcspn - counts the number character that doesn't include rej
 * @src: string to parse
 * @rej: characters not needed
 * Return: count
 */
size_t _strcspn(const char *src, const char *rej)
{
	size_t count = 0;
	const char *s = src;
	const char *r;

	while (*s)
	{
		r = rej;
		while (*r)
		{
			if (*s == *r)
				return (count);
			r++;
		}
		s++;

		count++;
	}

	return (count);
}
