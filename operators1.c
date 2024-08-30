#include "shell.h"

/**
 * _remove_comments - Replaces the first instance of '#' with '\0'
 * @buf: Address of the string to modify
 * Return: Always 0
 */
void _remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
	}
}

/**
 * _strncpy - Copies a source string to a
 * destination buffer with a limit
 * @_dest: The destination buffer
 * @_src: The source string
 * @n: The maximum number of characters to copy
 * Return: Pointer to the destination buffer
 */
char *_strncpy(char *_dest, char *_src, int n)
{
	int _i, _j;
	char *_s = _dest;

	_i = 0;
	while (_src[_i] != '\0' && _i < n - 1)
	{
		_dest[_i] = _src[_i];
		_i++;
	}
	if (_i < n)
	{
		_j = _i;
		while (_j < n)
		{
			_dest[_j] = '\0';
			_j++;
		}
	}
	return (_s);
}

/**
 * _strncat - Concatenates two strings with a limit on te
 * number of bytes
 * @_dest: The first string
 * @_src: The second string
 * @n: The maximum number of bytes to use from the source
 *
 * Return: Pointer to the concatenated string
 */
char *_strncat(char *_dest, char *_src, int n)
{
	int _i, _j;
	char *_s = _dest;

	_i = 0;
	_j = 0;
	while (_dest[_i] != '\0')
		_i++;
	while (_src[_j] != '\0' && _j < n)
	{
		_dest[_i] = _src[_j];
		_i++;
		_j++;
	}
	if (_j < n)
		_dest[_i] = '\0';
	return (_s);
}

/**
 * _strchr - Locates a character in a string
 * @_s: The string to be parsed
 * @_c: The character to look for
 * Return: A pointer to the memory area in the string
 */
char *_strchr(char *_s, char _c)
{
	do {
		if (*_s == _c)
			return (_s);
	} while (*_s++ != '\0');

	return (NULL);
}

/**
 * _get_environ - Returns a string array copy of the environment.
 * @info: Structure containing potential arguments. Used to maintain a
 *        constant function prototype.
 * Return: The environment variables as a string array.
 */
char **_get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = _list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}
