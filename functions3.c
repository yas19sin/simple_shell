#include "shell.h"

/**
 * _fork_cmd - Forks an exec thread to run a command
 * @info: The parameter & return info_t struct
 * Return: void
 */
void _fork_cmd(info_t *info)
{
	pid_t _child_pid;

	_child_pid = fork();
	if (_child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (_child_pid == 0)
	{
		if (execve(info->path, info->argv, _get_environ(info)) == -1)
		{
			_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				_print_error(info, "Permission denied\n");
		}
	}
}

/**
 * _strlen - Returns the length of a string
 * @s: The string whose length to check
 * Return: The length of the string
 */
int _strlen(char *s)
{
	int _i = 0;

	if (!s)
		return (0);

	while (*s++)
		_i++;
	return (_i);
}

/**
 * _strcmp - Performs a lexicographic comparison of two strings.
 * @s1: The first string
 * @s2: The second string
 * Return: Negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * _starts_with - Checks if the needle starts with the haystack
 * @haystack: String to search
 * @needle: The substring to find
 * Return: Address of the next character of haystack or NULL
 */
char *_starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
			return (NULL);
	return ((char *)haystack);
}

/**
 * _strcat - Concatenates two strings
 * @dest: The destination buffer
 * @src: The source buffer
 * Return: Pointer to the destination buffer
 */
char *_strcat(char *dest, char *src)
{
	char *_ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (_ret);
}
