#include "shell.h"

/**
 * _replace_string - Replaces a string.
 * @old: Address of the old string.
 * @new: New string.
 * Return: 1 if replaced, 0 otherwise.
 */
int _replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

/**
 * _find_cmd - Finds a command in PATH
 * @info: The parameter & return info_t struct
 * Return: void
 */
void _find_cmd(info_t *info)
{
	char *_path = NULL;
	int _i, _k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (_i = 0, _k = 0; info->arg[_i]; _i++)
		if (!_is_delim(info->arg[_i], " \t\n"))
			_k++;
	if (!_k)
		return;

	_path = _find_path(info, _getenv(info, "PATH="),
					   info->argv[0]);
	if (_path)
	{
		info->path = _path;
		_fork_cmd(info);
	}
	else
	{
		if ((_interactive(info) || _getenv(info, "PATH=") ||
			 info->argv[0][0] == '/') &&
			_is_cmd(info, info->argv[0]))
			_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			_print_error(info, "not found\n");
		}
	}
}
