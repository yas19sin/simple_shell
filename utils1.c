#include "shell.h"

/**
 * _myexit - Exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 * Return: Exits with a given exit status (0) if info->argv[0] != "exit".
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit argument */
	{
		exitcheck = _erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			_print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mycd - Changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 * Return: Always 0
 */
int _mycd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenv(info, "HOME=");
		if (!dir)
			chdir_ret = chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		_print_error(info, "can't cd to ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhistory - Displays the history list,
 * one command per line, preceded
 * with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myhistory(info_t *info)
{
	_print_list(info->history);
	return (0);
}

/**
 * _unset_alias - Unsets an alias.
 * @info: The parameter struct.
 * @str: The string alias.
 * Return: Always 0 on success, 1 on error.
 */
int _unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
	{
		return (1);
	}
	c = *p;
	*p = 0;
	ret = _delete_node_at_index(&(info->alias),
								_get_node_index(info->alias,
												_node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * _set_alias - Sets an alias to a string.
 * @info: The parameter struct.
 * @str: The string alias.
 * Return: Always 0 on success, 1 on error.
 */
int _set_alias(info_t *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
	{
		return (1);
	}
	if (!*++p)
	{
		return (_unset_alias(info, str));
	}

	_unset_alias(info, str);
	return (_add_node_end(&(info->alias), str, 0) == NULL);
}
