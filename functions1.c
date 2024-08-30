#include "shell.h"

/**
 * _bfree - Frees a pointer and NULLs the address
 * @_ptr: Address of the pointer to free
 * Return: 1 if freed, otherwise 0.
 */
int _bfree(void **_ptr)
{
	if (_ptr && *_ptr)
	{
		free(*_ptr);
		*_ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * _is_cmd - Checks if a file is an executable command
 * @info: The info_t struct
 * @path: The file path
 * Return: 1 if it's a valid command, 0 otherwise
 */
int _is_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * _dup_chars - Duplicates characters
 * @pathstr: The PATH string
 * @start: Starting index
 * @stop: Stopping index
 * Return: Pointer to the new buffer
 */
char *_dup_chars(char *pathstr, int start, int stop)
{
	static char _buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			_buf[k++] = pathstr[i];
	_buf[k] = 0;
	return (_buf);
}

/**
 * _find_path - Finds a command in the PATH string
 * @info: The info_t struct
 * @pathstr: The PATH string
 * @cmd: The command to find
 * Return: The full path of the command if found, or NULL
 */
char *_find_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && _starts_with(cmd, "./"))
	{
		if (_is_cmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = _dup_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (_is_cmd(info, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}

/**
 * _memset - Fills memory with a constant byte
 * @_s: The pointer to the memory area
 * @_b: The byte to fill *s with
 * @_n: The amount of bytes to be filled
 * Return: A pointer to the memory area s
 */
char *_memset(char *_s, char _b, unsigned int _n)
{
	unsigned int _i;

	for (_i = 0; _i < _n; _i++)
		_s[_i] = _b;
	return (_s);
}
