#include "shell.h"

/**
 * _strcpy - Copies a source string to a destination buffer
 * @_dest: The destination buffer
 * @_src: The source string
 * Return: Pointer to the destination buffer
 */
char *_strcpy(char *_dest, char *_src)
{
	int _i = 0;

	if (_dest == _src || _src == 0)
		return (_dest);
	while (_src[_i])
	{
		_dest[_i] = _src[_i];
		_i++;
	}
	_dest[_i] = 0;
	return (_dest);
}

/**
 * _strdup - Duplicates a source string and allocates memory for it
 * @_str: The source string to duplicate
 * Return: Pointer to the duplicated string
 */
char *_strdup(const char *_str)
{
	int _length = 0;
	char *_ret;

	if (_str == NULL)
		return (NULL);
	while (*_str++)
		_length++;
	_ret = malloc(sizeof(char) * (_length + 1));
	if (!_ret)
		return (NULL);
	for (_length++; _length--;)
		_ret[_length] = *--_str;
	return (_ret);
}

/**
 * _puts - Prints an input string to standard output
 * @_str: The string to be printed
 * Return: Nothing
 */
void _puts(char *_str)
{
	int _i = 0;

	if (!_str)
		return;
	while (_str[_i] != '\0')
	{
		_putchar(_str[_i]);
		_i++;
	}
}

/**
 * _putchar - Writes a character to standard output
 * @_c: The character to print
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char _c)
{
	static int _i;
	static char _buf[_WRITE_BUFFER_SIZE];

	if (_c == BUF_FLUSH || _i >= _WRITE_BUFFER_SIZE)
	{
		write(1, _buf, _i);
		_i = 0;
	}
	if (_c != BUF_FLUSH)
		_buf[_i++] = _c;
	return (1);
}

/**
 * _strtow - Splits a string into words using  delimiters
 * @string: The input string
 * @_delim: The delimiter string
 * Return: A pointer to an array of strings, or NULL on failure
 */
char **_strtow(char *string, char *_delim)
{
	int _j, i, _k, _m, _numwords = 0;
	char **_s;

	if (string == NULL || string[0] == 0)
		return (NULL);
	if (!_delim)
		_delim = " ";
	for (i = 0; string[i] != '\0'; i++)
	{
		if (!_is_delim(string[i], _delim) && (_is_delim(string[i + 1], _delim) ||
											  !string[i + 1]))
			_numwords++;
	}
	if (_numwords == 0)
		return (NULL);
	_s = malloc((1 + _numwords) * sizeof(char *));
	if (!_s)
		return (NULL);
	for (i = 0, _j = 0; _j < _numwords; _j++)
	{
		while (_is_delim(string[i], _delim))
			i++;
		_k = 0;
		while (!_is_delim(string[i + _k], _delim) &&
			   string[i + _k])
			_k++;
		_s[_j] = malloc((_k + 1) * sizeof(char));
		if (!_s[_j])
		{
			for (_k = 0; _k < _j; _k++)
				free(_s[_k]);
			free(_s);
			return (NULL);
		}
		for (_m = 0; _m < _k; _m++)
			_s[_j][_m] = string[i++];
		_s[_j][_m] = 0;
	}
	_s[_j] = NULL;
	return (_s);
}
