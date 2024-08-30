#include "shell.h"

/**
 * _myunsetenv - Removes an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myunsetenv(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);

	return (0);
}

/**
 * populate_env_list - Populates env linked list.
 * @info: Structure containing potential arguments. Used to maintain
 *                            constant function prototype.
 * Return: Always 0.
 */
int populate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		_add_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}

/**
 * _eputs - Prints an input string to stderr.
 * @str: The string to be printed.
 * Return: Nothing.
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
	{
		return;
	}

	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - Writes the character 'c' to stderr.
 * @c: The character to print.
 * Return: On success 1. On error, -1 is returned,
 * and errno is set appropriately.
 */
int _eputchar(char c)
{
	static int i;
	static char buf[_WRITE_BUFFER_SIZE];

	if (c == BUF_FLUSH || i >= _WRITE_BUFFER_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}

	if (c != BUF_FLUSH)
	{
		buf[i++] = c;
	}

	return (1);
}

/**
 * _putfd - Writes the character 'c' to the given file descriptor.
 * @c: The character to print.
 * @fd: The file descriptor to write to.
 * Return: On success 1. On error, -1 is returned,
 * and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static int i;
	static char buf[_WRITE_BUFFER_SIZE];

	if (c == BUF_FLUSH || i >= _WRITE_BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}

	if (c != BUF_FLUSH)
	{
		buf[i++] = c;
	}

	return (1);
}
