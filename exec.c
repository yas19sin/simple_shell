#include "shell.h"

/**
 * _unsetenv - Remove an environment variable.
 * @info: Structure containing potential arguments. Used to maintain a
 * constant function prototype.
 * @var: The string environment variable property.
 * Return: 1 on delete, 0 otherwise.
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
	{
		return (0);
	}

	while (node)
	{
		p = _starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = _delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}

	return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment
 * variable, or modify an existing one.
 * @info: Structure containing potential arguments.
 * Used to maintain a constant
 * function prototype.
 * @var: The string environment variable property.
 * @value: The string environment variable value.
 * Return: Always 0.
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
	{
		return (0);
	}

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
	{
		return (1);
	}

	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;

	while (node)
	{
		p = _starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}

	_add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;

	return (0);
}

/**
 * _clear_info - Initializes info_t struct.
 * @info: Struct address.
 */
void _clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * _set_info - Initializes info_t struct.
 * @info: Struct address.
 * @av: Argument vector.
 */
void _set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = _strtow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		_replace_alias(info);
		_replace_vars(info);
	}
}

/**
 * _free_info - Frees info_t struct fields.
 * @info: Struct address.
 * @all: True if freeing all fields.
 */
void _free_info(info_t *info, int all)
{
	_ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			_free_list(&(info->env));
		if (info->history)
			_free_list(&(info->history));
		if (info->alias)
			_free_list(&(info->alias));
		_ffree(info->environ);
		info->environ = NULL;
		_bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
