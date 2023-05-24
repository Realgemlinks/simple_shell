#include "shell.h"

/**
 * erratoi - this converts the status codes for exit to int
 * @str: status/char
 * Return: converted number
 */
int erratoi(char *str)
{
	int j = 0;
	unsigned long int res = 0;

	/* in cases when something like this are passed exit +2 this skips it */
	if (*str == '+')
		str++;
	for (j = 0; str[j] != '\0'; j++)
	{

		if (str[j] >= '0' && str[j] <= '9')
		{
			res *= 10;
			res += (str[j] - '0');
			if (res > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (res);
}


/**
 * put_num - print number to any file descriptor
 * @num: number to print
 * @fd: file descriptor
 * Return: length of numbers printed
 */
int put_num(int num, int fd)
{
	int (*print_num)(char) = putchr;
	int j, cnt = 0;
	unsigned int _abs, cur;

	if (fd == STDERR_FILENO)
	{
		print_num = eff_putchr;
	}
	/* makes space for '-' if the number is a negative number*/
	if (num < 0)
	{
		_abs = -num;
		print_num('-');
		cnt++;
	}
	else
		_abs = num;
	cur = _abs;

	/* this accomates number up to this number*/
	for (j = 1000000000; j > 1; j /= 10)
	{
		if (_abs / j)
		{
			print_num('0' + cur / j);
			cnt++;
		}
		cur %= j;
	}
	print_num('0' + cur);
	cnt++;

	return (cnt);
}


/**
 * print_err - this prints error message in a formated way
 * @data: contains infomation about our shell
 * @str: error message to print
 */
void print_err(shell_data *data, char *str)
{
	eff_puts(data->fname);
	eff_puts(": ");
	put_num(data->linecount, STDERR_FILENO);
	eff_puts(": ");
	eff_puts(data->argv[0]);
	eff_puts(": ");
	eff_puts(str);
}
