#include <stdio.h>
#include <string.h>

/**
 * main - Entry point of the program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
const char *str = "Cisnotfun!\n";
size_t size = sizeof(char);
size_t count = strlen(str);

fwrite(str, size, count, stdout);

printf("Simple Shell Project!\n");

return (0);
}
