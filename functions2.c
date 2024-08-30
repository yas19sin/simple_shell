#include "shell.h"

/**
 * _ffree - Frees a string of strings
 * @_pp: The string of strings
 */
void _ffree(char **_pp)
{
	char **_a = _pp;

	if (!_pp)
		return;
	while (*_pp)
		free(*_pp++);
	free(_a);
}

/**
 * _realloc - Reallocates a block of memory
 * @pnt: Pointer to the previous malloc'ated block
 * @size_old: Byte size of the previous block
 * @size_new: Byte size of the new block
 * Return: Pointer to the old block if successful, or NULL on failure
 */
void *_realloc(void *pnt, unsigned int size_old, unsigned int size_new)
{
	char *_p;

	if (!pnt)
		return (malloc(size_new));
	if (!size_new)
		return (free(pnt), NULL);
	if (size_new == size_old)
		return (pnt);

	_p = malloc(size_new);
	if (!_p)
		return (NULL);

	size_old = size_old < size_new ? size_old : size_new;
	while (size_old--)
		_p[size_old] = ((char *)pnt)[size_old];
	free(pnt);
	return (_p);
}

/**
 * hsh - Main shell loop
 * @info: The parameter & return info_t struct
 * @av: The argument vector from main()
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t _r = 0;
	int _builtin_ret = 0;

	while (_r != -1 && _builtin_ret != -2)
	{
		_clear_info(info);
		if (_interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		_r = _get_input(info);
		if (_r != -1)
		{
			_set_info(info, av);
			_builtin_ret = _find_builtin(info);
			if (_builtin_ret == -1)
				_find_cmd(info);
		}
		else if (_interactive(info))
			_putchar('\n');
		_free_info(info, 0);
	}
	_write_history(info);
	_free_info(info, 1);
	if (!_interactive(info) && info->status)
		exit(info->status);
	if (_builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (_builtin_ret);
}

/**
 * _find_builtin - Finds a builtin command
 * @info: The parameter & return info_t struct
 * Return: -1 if builtin not found,
 * 0 if builtin executed successfully,
 * 1 if builtin found but not successful,
 * -2 if builtin signals exit()
 */
int _find_builtin(info_t *info)
{
	int _i, _builtin_ret = -1;
	_builtin_table _builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}};

	for (_i = 0; _builtintbl[_i].type; _i++)
		if (_strcmp(info->argv[0], _builtintbl[_i].type) == 0)
		{
			info->line_count++;
			_builtin_ret = _builtintbl[_i].func(info);
			break;
		}
	return (_builtin_ret);
}
