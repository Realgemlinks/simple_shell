#include "shell.h"

/**
 * _puts - prints a string to the output stream
 * @str: string to write
 * Return: len of string
 */
int _puts(char *str)
{
	int len;

	len = _strlen(str);
	write(STDOUT_FILENO, str, len);
	write(STDOUT_FILENO, "\n", 1);

	return (len);
}

/**
 * putstr - prints string
 * @str: string to print
 *
 * Description: this is different from the std puts and also
 * not a custom implementation of puts. this and putchr helps reduce the
 * number of system calls and helps make our program a better one
 */

void putstr(char *str)
{
	int j = 0;

	while (str[j])
	{
		putchr(str[j]);
		j++;
	}
}

/**
 * putchr - prints character
 * @chr: character to print
 * Return: 1
 */
int putchr(char chr)
{
	static int j;
	static char buffer[WR_BUFFER_SIZE];

	if (chr == FLUSH || j >= WR_BUFFER_SIZE)
	{
		write(STDOUT_FILENO, buffer, j);
		j = 0;
	}

	if (chr != FLUSH)
		buffer[j++] = chr;

	return (1);
}
